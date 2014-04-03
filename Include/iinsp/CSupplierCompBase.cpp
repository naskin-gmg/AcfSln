#include "iinsp/CSupplierCompBase.h"


// Qt includes
#include <QtCore/QSet>

// ACF includes
#include "istd/TChangeNotifier.h"


namespace iinsp
{


CSupplierCompBase::CSupplierCompBase()
:	m_workStatus(WS_INVALID),
	m_productChangeNotifier(NULL, CF_SUPPLIER_RESULTS | CF_MODEL),
	m_inputsObserver(this),
	m_paramsObserver(this),
	m_areParametersValid(false)
{
}


// reimplemented (iinsp::ISupplier)

int CSupplierCompBase::GetWorkStatus() const
{
	return m_workStatus;
}


void CSupplierCompBase::InvalidateSupplier()
{
	if (m_workStatus >= WS_OK){
		m_productChangeNotifier.SetPtr(this);

		m_workStatus = WS_INVALID;
	}
}


void CSupplierCompBase::EnsureWorkInitialized()
{
	if (m_workStatus < WS_INIT){
		m_tempMessageContainer.ClearMessages();
		m_messageContainer.ClearMessages();

		// distribute initializing to input...
		for (		InputSuppliersMap::ConstIterator inputSupplierIter = m_inputSuppliersMap.constBegin();
					inputSupplierIter != m_inputSuppliersMap.constEnd();
					++inputSupplierIter){
			ISupplier* supplierPtr = inputSupplierIter.value();

			supplierPtr->EnsureWorkInitialized();
		}

		m_productChangeNotifier.SetPtr(this);

		if (!m_areParametersValid){
			OnParametersChanged();

			m_areParametersValid = true;
		}

		if (InitializeWork()){
			m_workStatus = WS_INIT;
		}
		else{
			m_workStatus = WS_CRITICAL;
		}
	}
}


void CSupplierCompBase::ClearWorkResults()
{
	if (m_workStatus == WS_LOCKED){
		return;
	}

	m_workStatus = WS_INVALID;
	
	m_tempMessageContainer.ClearMessages();
	m_messageContainer.ClearMessages();

	m_productChangeNotifier.Reset();
}


const ilog::IMessageContainer* CSupplierCompBase::GetWorkMessages(int messageType) const
{
	switch (messageType){
	case WMT_TEMP:
		return &m_tempMessageContainer;

	case WMT_RESULTS:
		return &m_messageContainer;

	default:
		return NULL;
	}
}


iprm::IParamsSet* CSupplierCompBase::GetModelParametersSet() const
{
	return m_paramsSetCompPtr.GetPtr();
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

	modelPtr->AttachObserver(&m_inputsObserver);

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


void CSupplierCompBase::AddMessage(const ilog::CMessage* messagePtr) const
{
	Q_ASSERT(messagePtr != NULL);

	m_messageContainer.AddMessage(istd::TSmartPtr<const istd::IInformationProvider>(messagePtr));
}


// reimplemented (ilog::IMessageConsumer)

bool CSupplierCompBase::IsMessageSupported(
			int messageCategory,
			int messageId,
			const istd::IInformationProvider* messagePtr) const
{
	return m_messageContainer.IsMessageSupported(messageCategory, messageId, messagePtr);
}


void CSupplierCompBase::AddMessage(const MessagePtr& messagePtr)
{
	m_messageContainer.AddMessage(messagePtr);
}


// reimplemented (icomp::CComponentBase)

void CSupplierCompBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_productChangeNotifier.Reset();

	if (m_paramsSetModelCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(&m_paramsObserver);
	}

	int inputsCount = m_additionalTriggerInputsCompPtr.GetCount();
	for (int i = 0; i < inputsCount; ++i){
		imod::IModel* inputModelPtr = m_additionalTriggerInputsCompPtr[i];
		if (inputModelPtr != NULL){
			RegisterSupplierInput(inputModelPtr);
		}
	}

	m_workStatus = WS_INVALID;
}


void CSupplierCompBase::OnComponentDestroyed()
{
	m_inputsObserver.EnsureModelsDetached();
	m_paramsObserver.EnsureModelsDetached();

	m_productChangeNotifier.Abort();

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
			MessagePtr messagePtr(new ilog::CMessage(
						istd::IInformationProvider::IC_NONE,
						MI_DURATION_TIME,
						QObject::tr("%1 took %2 ms").arg(m_measuredFeatureName).arg(m_timer.GetElapsed() * 1000),
						diagnosticName));
			m_parentPtr->m_messageContainer.AddMessage(messagePtr);
		}
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

void CSupplierCompBase::InputsObserver::BeforeUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
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

void CSupplierCompBase::ParamsObserver::BeforeUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.InvalidateSupplier();
}


void CSupplierCompBase::ParamsObserver::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.m_areParametersValid = false;
}


} // namespace iinsp


