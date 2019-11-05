#include <iinsp/CSupplierCompBase.h>


// Qt includes
#include <QtCore/QSet>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <ilog/CMessage.h>


namespace iinsp
{


static const istd::IChangeable::ChangeSet s_supplierResultsSet(iinsp::ISupplier::CF_SUPPLIER_RESULTS);


CSupplierCompBase::CSupplierCompBase()
:	m_inputsObserver(this),
	m_paramsObserver(this),
	m_paramsSetPtr(NULL),
	m_lastWorkState(WS_INVALID),
	m_isInputValid(false),
	m_areParametersValid(false)
{
}


// reimplemented (iinsp::ISupplier)

int CSupplierCompBase::GetWorkStatus() const
{
	return m_workStatus.GetSupplierState();
}


imod::IModel* CSupplierCompBase::GetWorkStatusModel() const
{
	return &(const_cast<CSupplierCompBase*>(this))->m_workStatus;
}


void CSupplierCompBase::InvalidateSupplier()
{
	if (m_isInputValid){
		m_isInputValid = false;

		if (!m_productChangeNotifierPtr.IsValid()){
			m_productChangeNotifierPtr.SetPtr(new istd::CChangeNotifier(this, &s_supplierResultsSet));
		}
	}
}


void CSupplierCompBase::EnsureWorkInitialized()
{
	int status = m_workStatus.GetSupplierState();
	if ((status < WS_INIT) || (status > WS_LOCKED)){
		m_workStatus.SetSupplierState(WS_LOCKED);

		for (int i = 0; i <= MTC_LAST; ++i){
			m_messageContainers[i].ClearMessages();
		}

		// distribute initializing to input...
		for (		InputSuppliersMap::ConstIterator inputSupplierIter = m_inputSuppliersMap.constBegin();
					inputSupplierIter != m_inputSuppliersMap.constEnd();
					++inputSupplierIter){
			ISupplier* supplierPtr = inputSupplierIter.value();

			supplierPtr->EnsureWorkInitialized();
		}

		if (!m_productChangeNotifierPtr.IsValid()){
			m_productChangeNotifierPtr.SetPtr(new istd::CChangeNotifier(this, &s_supplierResultsSet));
		}

		if (!m_areParametersValid){
			OnParametersChanged();

			m_areParametersValid = true;
		}

		if (InitializeWork()){
			m_workStatus.SetSupplierState(WS_INIT);
		}
		else{
			m_workStatus.SetSupplierState(WS_FAILED);
		}

		m_productChangeNotifierPtr.Reset();
	}
}


void CSupplierCompBase::EnsureWorkFinished()
{
	if ((!m_isInputValid || (m_lastWorkState <= WS_INIT)) && (m_workStatus.GetSupplierState() == WS_INIT)){
		m_workStatus.SetSupplierState(WS_LOCKED);

		m_lastWorkState = ProcessWorkOutput();
		Q_ASSERT(m_lastWorkState >= WS_OK);	// No initial states are possible
	}

	m_isInputValid = true;
	m_workStatus.SetSupplierState(m_lastWorkState);
}


void CSupplierCompBase::ClearWorkResults()
{
	if (m_workStatus.GetSupplierState() == WS_LOCKED){
		return;
	}

	m_isInputValid = false;
	m_workStatus.SetSupplierState(WS_INVALID);
	m_lastWorkState = WS_INVALID;
	
	for (int i = 0; i <= MTC_LAST; ++i){
		m_messageContainers[i].ClearMessages();
	}

	m_productChangeNotifierPtr.Reset();
}


const ilog::IMessageContainer* CSupplierCompBase::GetWorkMessages(int containerType) const
{
	if (CSupplierCompBase::IsMessageContainerSupported(containerType)){
		return &m_messageContainers[containerType];
	}
	else{
		return NULL;
	}
}


iprm::IParamsSet* CSupplierCompBase::GetModelParametersSet() const
{
	return m_paramsSetPtr;
}


// protected methods

bool CSupplierCompBase::InitializeWork()
{
	return true;
}


void CSupplierCompBase::OnParametersChanged()
{
}


void CSupplierCompBase::RegisterSupplierInput(imod::IModel* modelPtr, ISupplier* supplierPtr)
{
	Q_ASSERT(modelPtr != NULL);

	if (!modelPtr->IsAttached(&m_inputsObserver)){
		if (!modelPtr->AttachObserver(&m_inputsObserver)){
			qDebug("Supplier input cannot be observed");

			return;
		}
	}
	else{
		qDebug("Supplier input already under observation");
	}

	if (supplierPtr != NULL){
		m_inputSuppliersMap[modelPtr] = supplierPtr;
	}
}


void CSupplierCompBase::UnregisterSupplierInput(imod::IModel* modelPtr)
{
	Q_ASSERT(modelPtr != NULL);
	if (m_inputsObserver.IsModelAttached(modelPtr)){
		modelPtr->DetachObserver(&m_inputsObserver);
	}

	m_inputSuppliersMap.remove(modelPtr);
}


QString CSupplierCompBase::GetDiagnosticName() const
{
	if (m_diagnosticNameAttrPtr.IsValid()){
		return *m_diagnosticNameAttrPtr;
	}
	else{
		return "";
	}
}


bool CSupplierCompBase::IsMessageContainerSupported(int containerType) const
{
	return (containerType >= 0) && (containerType <= MTC_LAST);
}


void CSupplierCompBase::AddMessage(const istd::IInformationProvider* messagePtr, int containerType) const
{
	Q_ASSERT(messagePtr != NULL);

	if (IsMessageContainerSupported(containerType)){
		m_messageContainers[containerType].AddMessage(istd::TSmartPtr<const istd::IInformationProvider>(messagePtr));
	}
	else{
		delete messagePtr;
	}
}


// reimplemented (icomp::CComponentBase)

void CSupplierCompBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_paramsSetPtr = m_paramsSetCompPtr.GetPtr();

	if ((m_paramsSetPtr != NULL) && m_paramsSetModelCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(&m_paramsObserver);
	}

	int inputsCount = m_additionalTriggerInputsCompPtr.GetCount();
	for (int i = 0; i < inputsCount; ++i){
		imod::IModel* inputModelPtr = m_additionalTriggerInputsCompPtr[i];
		if (inputModelPtr != NULL){
			RegisterSupplierInput(inputModelPtr);
		}
	}

	m_workStatus.SetSupplierState(WS_INVALID);
	m_lastWorkState = WS_INVALID;

	m_productChangeNotifierPtr.Reset();
}


void CSupplierCompBase::OnComponentDestroyed()
{
	m_inputsObserver.EnsureModelsDetached();
	m_paramsObserver.EnsureModelsDetached();

	if (m_productChangeNotifierPtr.IsValid()){
		m_productChangeNotifierPtr->Abort();
		m_productChangeNotifierPtr.Reset();
	}

	m_paramsSetPtr = NULL;

	BaseClass::OnComponentDestroyed();
}


// public methods of embedded class Timer

CSupplierCompBase::Timer::Timer(const CSupplierCompBase* parentPtr, const QString& measuredFeatureName)
:	m_parentPtr(parentPtr), m_measuredFeatureName(measuredFeatureName)
{
	m_timer.Start();
}


CSupplierCompBase::Timer::~Timer()
{
	if (m_parentPtr != NULL){
		QString diagnosticName = m_parentPtr->GetDiagnosticName();
		if (!diagnosticName.isEmpty()){
			ilog::CMessage* messagePtr = new ilog::CMessage(
						istd::IInformationProvider::IC_NONE,
						MI_DURATION_TIME,
						QObject::tr("%1 took %2 ms").arg(m_measuredFeatureName).arg(m_timer.GetElapsed() * 1000),
						diagnosticName);
			m_parentPtr->AddMessage(messagePtr, MCT_RESULTS);
		}
	}
}


// public methods of embedded class Status

CSupplierCompBase::Status::Status()
:	m_state(ISupplier::WS_INVALID)
{
}


int CSupplierCompBase::Status::GetSupplierState() const
{
	return m_state;
}


void CSupplierCompBase::Status::SetSupplierState(int state)
{
	if (m_state != state){
		istd::CChangeNotifier changeNotifier(this);

		m_state = state;
	}
}


// public methods of embedded class InputsObserver

CSupplierCompBase::InputsObserver::InputsObserver(CSupplierCompBase* parentPtr)
:	m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class InputsObserver

// reimplemented (imod::CMultiModelObserverBase)

void CSupplierCompBase::InputsObserver::BeforeUpdate(imod::IModel* /*modelPtr*/)
{
	m_parent.InvalidateSupplier();
}


// public methods of embedded class ParamsObserver

CSupplierCompBase::ParamsObserver::ParamsObserver(CSupplierCompBase* parentPtr)
:	m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class ParamsObserver

// reimplemented (imod::CMultiModelObserverBase)

void CSupplierCompBase::ParamsObserver::BeforeUpdate(imod::IModel* /*modelPtr*/)
{
	m_parent.InvalidateSupplier();
}


void CSupplierCompBase::ParamsObserver::AfterUpdate(imod::IModel* /*modelPtr*/, const ChangeSet& /*changeSet*/)
{
	m_parent.m_areParametersValid = false;
}


} // namespace iinsp


