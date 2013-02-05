#ifndef iproc_TSupplierCompWrap_included
#define iproc_TSupplierCompWrap_included


// Qt includes
#include <QtCore/QSet>
#include <QtCore/QElapsedTimer>

// ACF includes
#include "istd/IChangeable.h"
#include "istd/TChangeNotifier.h"
#include "imod/IModel.h"
#include "imod/TModelWrap.h"
#include "imod/CMultiModelObserverBase.h"
#include "icomp/CComponentBase.h"
#include "ibase/TLoggerCompWrap.h"
#include "ibase/CMessageContainer.h"

// ACF-Solutions includes
#include "iproc/ISupplier.h"


namespace iproc
{


/**
	Standard implementation of interface iproc::ISupplier with preparation for component implementation.
	During component initialization you should call \c RegisterSupplierInput for all suppliers used by this component as a input.
*/
template <class Product>
class TSupplierCompWrap:
			public ibase::CLoggerComponentBase,
			virtual public ISupplier,
			virtual public istd::IChangeable,
			virtual protected ibase::IMessageConsumer
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef Product ProductType;

	enum MessageId
	{
		MI_DURATION_TIME = 0x077a1b
	};

	I_BEGIN_BASE_COMPONENT(TSupplierCompWrap);
		I_REGISTER_INTERFACE(ISupplier);
		I_ASSIGN(m_diagnosticNameAttrPtr, "DiagnosticName", "Name of this supplier for diagnostic, if it is not set, no diagnostic log message will be send", false, "");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set describing model parameter used to produce results", false, "ParamsSet");
		I_ASSIGN_TO(m_paramsSetModelCompPtr, m_paramsSetCompPtr, false);
		I_ASSIGN_MULTI_0(m_additionalTriggerInputsCompPtr, "AdditionalTriggerInputs", "Additional observed trigger inputs, the supplier will be invalidated if some input signal the change", false);
	I_END_COMPONENT;

	TSupplierCompWrap();

	// reimplemented (iproc::ISupplier)
	virtual int GetWorkStatus() const;
	virtual void InvalidateSupplier();
	virtual void EnsureWorkInitialized();
	virtual void EnsureWorkFinished();
	virtual void ClearWorkResults();
	virtual const ibase::IMessageContainer* GetWorkMessages() const;
	virtual iprm::IParamsSet* GetModelParametersSet() const;

protected:
	class Timer
	{
	public:
		Timer(const TSupplierCompWrap* parentPtr, const QString& measuredFeatureName);
		~Timer();

	private:
		QElapsedTimer m_timer;
		const TSupplierCompWrap* m_parentPtr;
		QString m_measuredFeatureName;
	};

	/**
		Called if the new work should be initialized.
		Default implementation do nothing. It is dedicated to be overridden.
	*/
	virtual bool InitializeWork();

	/**
		Called if the supplier parameters was changed.
		Default implementation do nothing. It is dedicated to be overridden.
	*/
	virtual void OnParametersChanged();

	/**
		Get current work product, if work was done correctly.
	*/
	const Product* GetWorkProduct() const;

	/**
		Register supplier input.
		Changes of supplier input will force this supplier invalidate.
		All registered inputs will be unregistered during component destruction (OnComponentDestryed method).
	*/
	virtual void RegisterSupplierInput(imod::IModel* modelPtr, ISupplier* supplierPtr = NULL);
	/**
		Unregister supplier input.
		Changes of this input will no more invalidate this supplier.
	*/
	virtual void UnregisterSupplierInput(imod::IModel* modelPtr);

	/**
		Add ibase::CMessage to the internal message container (also from const functions).
	*/
	virtual void AddMessage(const ibase::CMessage* messagePtr) const;

	// abstract methods
	/**
		Produce single object.
		\return	work status. \sa iproc::WorkStatus
	*/
	virtual int ProduceObject(Product& result) const = 0;

	// reimplemented (ibase::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const MessagePtr& messagePtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	class InputsObserver: public imod::CMultiModelObserverBase
	{
	public:
		InputsObserver(TSupplierCompWrap<Product>* parentPtr);

		using imod::CMultiModelObserverBase::EnsureModelsDetached;

	protected:
		// reimplemented (imod::CMultiModelObserverBase)
		virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

		TSupplierCompWrap<Product>& m_parent;
	};

	class ParamsObserver: public imod::CMultiModelObserverBase
	{
	public:
		ParamsObserver(TSupplierCompWrap<Product>* parentPtr);

		using imod::CMultiModelObserverBase::EnsureModelsDetached;

	protected:
		// reimplemented (imod::CMultiModelObserverBase)
		virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

		TSupplierCompWrap<Product>& m_parent;
	};

	I_ATTR(QString, m_diagnosticNameAttrPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_MULTIREF(imod::IModel, m_additionalTriggerInputsCompPtr);

	istd::TDelPtr<Product> m_productPtr;
	int m_workStatus;

	InputsObserver m_inputsObserver;
	ParamsObserver m_paramsObserver;

	istd::CChangeNotifier m_productChangeNotifier;

	typedef QMap<imod::IModel*, ISupplier*> InputSuppliersMap;
	InputSuppliersMap m_inputSuppliersMap;

	mutable imod::TModelWrap<ibase::CMessageContainer> m_messageContainer;

	bool m_areParametersValid;
};


// public methods

template <class Product>
TSupplierCompWrap<Product>::TSupplierCompWrap()
:	m_workStatus(WS_INVALID),
	m_inputsObserver(this),
	m_paramsObserver(this),
	m_productChangeNotifier(NULL, CF_SUPPLIER_RESULTS | CF_MODEL),
	m_areParametersValid(false)
{
}


// reimplemented (iproc::ISupplier)

template <class Product>
int TSupplierCompWrap<Product>::GetWorkStatus() const
{
	return m_workStatus;
}


template <class Product>
void TSupplierCompWrap<Product>::InvalidateSupplier()
{
	if (m_workStatus >= WS_OK){
		m_productChangeNotifier.SetPtr(this);

		m_workStatus = WS_INVALID;
	}
}


template <class Product>
void TSupplierCompWrap<Product>::EnsureWorkInitialized()
{
	if (m_workStatus < WS_INIT){
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


template <class Product>
void TSupplierCompWrap<Product>::EnsureWorkFinished()
{
	if (m_workStatus == WS_INIT){
		m_workStatus = WS_LOCKED;

		if (!m_productPtr.IsValid()){
			m_productPtr.SetPtr(new Product());
		}

		m_workStatus = ProduceObject(*m_productPtr);
		Q_ASSERT(m_workStatus >= WS_OK);	// No initial states are possible

		m_productChangeNotifier.Reset();
	}
}


template <class Product>
void TSupplierCompWrap<Product>::ClearWorkResults()
{
	if (m_workStatus == WS_LOCKED){
		return;
	}

	m_workStatus = WS_INVALID;
	
	m_messageContainer.ClearMessages();

	m_productPtr.Reset();

	m_productChangeNotifier.Reset();
}


template <class Product>
const ibase::IMessageContainer* TSupplierCompWrap<Product>::GetWorkMessages() const
{
	return &m_messageContainer;
}


template <class Product>
iprm::IParamsSet* TSupplierCompWrap<Product>::GetModelParametersSet() const
{
	return m_paramsSetCompPtr.GetPtr();
}


// protected methods

template <class Product>
bool TSupplierCompWrap<Product>::InitializeWork()
{
	return true;
}


template <class Product>
void TSupplierCompWrap<Product>::OnParametersChanged()
{
}


template <class Product>
const Product* TSupplierCompWrap<Product>::GetWorkProduct() const
{
	const_cast< TSupplierCompWrap<Product>* >(this)->EnsureWorkFinished();

	if (m_workStatus <= WS_OK){
		return m_productPtr.GetPtr();
	}
	else{
		return NULL;
	}
}


template <class Product>
void TSupplierCompWrap<Product>::RegisterSupplierInput(imod::IModel* modelPtr, ISupplier* supplierPtr)
{
	Q_ASSERT(modelPtr != NULL);

	modelPtr->AttachObserver(&m_inputsObserver);

	if (supplierPtr != NULL){
		m_inputSuppliersMap[modelPtr] = supplierPtr;
	}
}


template <class Product>
void TSupplierCompWrap<Product>::UnregisterSupplierInput(imod::IModel* modelPtr)
{
	Q_ASSERT(modelPtr != NULL);
	if (m_inputsObserver.IsModelAttached(modelPtr)){
		modelPtr->DetachObserver(&m_inputsObserver);
	}

	m_inputSuppliersMap.remove(modelPtr);
}


template <class Product>
void TSupplierCompWrap<Product>::AddMessage(const ibase::CMessage* messagePtr) const
{
	Q_ASSERT(messagePtr != NULL);

	m_messageContainer.AddMessage((const istd::TSmartPtr<const istd::IInformationProvider>)messagePtr);
}


// reimplemented (ibase::IMessageConsumer)
template <class Product>
bool TSupplierCompWrap<Product>::IsMessageSupported(
			int messageCategory,
			int messageId,
			const istd::IInformationProvider* messagePtr) const
{
	return m_messageContainer.IsMessageSupported(messageCategory, messageId, messagePtr);
}


template <class Product>
void TSupplierCompWrap<Product>::AddMessage(const MessagePtr& messagePtr)
{
	m_messageContainer.AddMessage(messagePtr);
}


// reimplemented (icomp::CComponentBase)

template <class Product>
void TSupplierCompWrap<Product>::OnComponentCreated()
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


template <class Product>
void TSupplierCompWrap<Product>::OnComponentDestroyed()
{
	m_inputsObserver.EnsureModelsDetached();
	m_paramsObserver.EnsureModelsDetached();

	m_productChangeNotifier.Abort();

	BaseClass::OnComponentDestroyed();
}


// public methods of embedded class Timer

template <class Product>
TSupplierCompWrap<Product>::Timer::Timer(const TSupplierCompWrap* parentPtr, const QString& measuredFeatureName)
:	m_parentPtr(parentPtr), m_measuredFeatureName(measuredFeatureName)
{
	m_timer.start();
}


template <class Product>
TSupplierCompWrap<Product>::Timer::~Timer()
{
	if ((m_parentPtr != NULL) && (m_parentPtr->m_diagnosticNameAttrPtr.IsValid())){
		MessagePtr messagePtr(new ibase::CMessage(
					istd::IInformationProvider::IC_INFO,
					0,
					QObject::tr("%1 took %2 ms").arg(m_measuredFeatureName).arg(m_timer.elapsed()),
					*m_parentPtr->m_diagnosticNameAttrPtr));
		m_parentPtr->m_messageContainer.AddMessage(messagePtr);
	}
}


// public methods of embedded class InputsObserver

template <class Product>
TSupplierCompWrap<Product>::InputsObserver::InputsObserver(TSupplierCompWrap<Product>* parentPtr)
:	m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class InputsObserver

// reimplemented (imod::CMultiModelObserverBase)

template <class Product>
void TSupplierCompWrap<Product>::InputsObserver::BeforeUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.InvalidateSupplier();
}


// public methods of embedded class ParamsObserver

template <class Product>
TSupplierCompWrap<Product>::ParamsObserver::ParamsObserver(TSupplierCompWrap<Product>* parentPtr)
:	m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class ParamsObserver

// reimplemented (imod::CMultiModelObserverBase)

template <class Product>
void TSupplierCompWrap<Product>::ParamsObserver::BeforeUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.InvalidateSupplier();
}


template <class Product>
void TSupplierCompWrap<Product>::ParamsObserver::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.m_areParametersValid = false;
}


} // namespace iproc


#endif // !iproc_TSupplierCompWrap_included


