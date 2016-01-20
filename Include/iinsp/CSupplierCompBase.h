#ifndef iinsp_CSupplierCompBase_included
#define iinsp_CSupplierCompBase_included


// ACF includes
#include "istd/CGeneralTimeStamp.h"
#include "imod/IModel.h"
#include "imod/TModelWrap.h"
#include "imod/CMultiModelObserverBase.h"
#include "icomp/CComponentBase.h"
#include "ilog/TLoggerCompWrap.h"
#include "ilog/CMessageContainer.h"

// ACF-Solutions includes
#include "iinsp/ISupplier.h"


namespace iinsp
{


/**
	Base implementation of interface iinsp::ISupplier for iinsp::TSupplierCompWrap.
*/
class CSupplierCompBase:
			public ilog::CLoggerComponentBase,
			virtual public ISupplier,
			virtual protected ilog::IMessageConsumer
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	/**
		ID of messages.
		\sa istd::IInformationProvider
	*/
	enum MessageId
	{
		/**
			The message contains product calculation time.
		*/
		MI_DURATION_TIME = 0x077a1b,

		/**
			The message describes main geometrical result.
		*/
		MI_GEOMETRICAL_RESULT,

		/**
			The message describes status of the supplier results.
			\sa istd::IInformationProvider
		*/
		MI_SUPPLIER_RESULTS_STATUS,

		/**
			Id for intermediate results
		*/ 
		MI_INTERMEDIATE
	};

	I_BEGIN_BASE_COMPONENT(CSupplierCompBase);
		I_REGISTER_INTERFACE(ISupplier);
		I_REGISTER_SUBELEMENT(TaskLog);
		I_REGISTER_SUBELEMENT_INTERFACE(TaskLog, ilog::IMessageConsumer, ExtractTaskLog);
		I_REGISTER_SUBELEMENT_INTERFACE(TaskLog, istd::IPolymorphic, ExtractTaskLog);
		I_REGISTER_SUBELEMENT(TempMessages);
		I_REGISTER_SUBELEMENT_INTERFACE(TempMessages, ilog::IMessageConsumer, ExtractTempMessages);
		I_REGISTER_SUBELEMENT_INTERFACE(TempMessages, istd::IPolymorphic, ExtractTempMessages);
		I_ASSIGN(m_diagnosticNameAttrPtr, "DiagnosticName", "Name of this supplier for diagnostic, if it is not set, no diagnostic log message will be send", false, "");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set describing model parameter used to produce results", false, "ParamsSet");
		I_ASSIGN_TO(m_paramsSetModelCompPtr, m_paramsSetCompPtr, false);
		I_ASSIGN_MULTI_0(m_additionalTriggerInputsCompPtr, "AdditionalTriggerInputs", "Additional observed trigger inputs, the supplier will be invalidated if some input signal the change", false);
	I_END_COMPONENT;

	CSupplierCompBase();

	// reimplemented (iinsp::ISupplier)
	virtual int GetWorkStatus() const;
	virtual void InvalidateSupplier();
	virtual void EnsureWorkInitialized();
	virtual void ClearWorkResults();
	virtual const ilog::IMessageContainer* GetWorkMessages(int messageType) const;
	virtual iprm::IParamsSet* GetModelParametersSet() const;

protected:
	class Timer
	{
	public:
		Timer(const CSupplierCompBase* parentPtr, const QString& measuredFeatureName);
		~Timer();

	private:
		istd::CGeneralTimeStamp m_timer;
		const CSupplierCompBase* m_parentPtr;
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
		Get supplier name used for diagnostic reasons.
	*/
	virtual QString GetDiagnosticName() const;

	/**
		Add ilog::CMessage to the internal message container (also from const functions).
		\param	messagePtr	pointer to the new message object. It cannot be NULL.
	*/
	virtual void AddMessage(const ilog::CMessage* messagePtr) const;

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const MessagePtr& messagePtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	int m_workStatus;
	istd::TDelPtr<istd::CChangeNotifier> m_productChangeNotifierPtr;

private:
	class InputsObserver: public imod::CMultiModelObserverBase
	{
	public:
		InputsObserver(CSupplierCompBase* parentPtr);

		using imod::CMultiModelObserverBase::EnsureModelsDetached;

	protected:
		// reimplemented (imod::CMultiModelObserverBase)
		virtual void BeforeUpdate(imod::IModel* modelPtr);

		CSupplierCompBase& m_parent;
	};

	class ParamsObserver: public imod::CMultiModelObserverBase
	{
	public:
		ParamsObserver(CSupplierCompBase* parentPtr);

		using imod::CMultiModelObserverBase::EnsureModelsDetached;

	protected:
		// reimplemented (imod::CMultiModelObserverBase)
		virtual void BeforeUpdate(imod::IModel* modelPtr);
		virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

		CSupplierCompBase& m_parent;
	};

	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractTaskLog(CSupplierCompBase& component)
	{
		return &component;
	}

	template <class InterfaceType>
	static InterfaceType* ExtractTempMessages(CSupplierCompBase& component)
	{
		return &component.m_tempMessageContainer;
	}

	I_ATTR(QString, m_diagnosticNameAttrPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_MULTIREF(imod::IModel, m_additionalTriggerInputsCompPtr);

	InputsObserver m_inputsObserver;
	ParamsObserver m_paramsObserver;

	typedef QMap<imod::IModel*, ISupplier*> InputSuppliersMap;
	InputSuppliersMap m_inputSuppliersMap;

	imod::TModelWrap<ilog::CMessageContainer> m_tempMessageContainer;
	mutable imod::TModelWrap<ilog::CMessageContainer> m_messageContainer;

	iprm::IParamsSet* m_paramsSetPtr;

	bool m_areParametersValid;
};


} // namespace iinsp


#endif // !iinsp_CSupplierCompBase_included


