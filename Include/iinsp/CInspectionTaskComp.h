#ifndef iinsp_CInspectionTaskComp_included
#define iinsp_CInspectionTaskComp_included


// Qt includes
#include <QMap>

// ACF includes
#include "istd/IInformationProvider.h"
#include "istd/TChangeNotifier.h"
#include "imod/IModel.h"
#include "imod/TModelWrap.h"
#include "imod/CMultiModelBridgeBase.h"
#include "ibase/IMessageContainer.h"
#include "ibase/TLoggerCompWrap.h"

// ACF-Solutions includes
#include "iinsp/IInspectionTask.h"


namespace iinsp
{


/**
	Standard component implementation of interface iinsp::IInspectionTask.
	Serializing of this object serialize all inspection parameters.
*/
class CInspectionTaskComp:
			public ibase::CLoggerComponentBase,
			virtual public IInspectionTask,
			virtual public iproc::ISupplier,
			virtual public istd::IInformationProvider,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef imod::CMultiModelBridgeBase BaseClass2;

	enum MessageId
	{
		MI_BAD_PARAMS_COUNT = 0x5af30,
		MI_NO_SUBTASK
	};

	I_BEGIN_COMPONENT(CInspectionTaskComp);
		I_REGISTER_INTERFACE(IInspectionTask);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(iproc::ISupplier);
		I_REGISTER_INTERFACE(ibase::IMessageContainer);
		I_REGISTER_INTERFACE(istd::IInformationProvider);
		I_REGISTER_SUBELEMENT(Parameters);
		I_REGISTER_SUBELEMENT_INTERFACE_T(Parameters, iprm::IParamsSet, ExtractParameters);
		I_REGISTER_SUBELEMENT_INTERFACE_T(Parameters, iser::ISerializable, ExtractParameters);
		I_REGISTER_SUBELEMENT_INTERFACE_T(Parameters, istd::IChangeable, ExtractParameters);
		I_REGISTER_SUBELEMENT_INTERFACE_T(Parameters, imod::IModel, ExtractParameters);
		I_ASSIGN_MULTI_0(m_subtasksCompPtr, "Subtasks", "List of subtasks (suppliers)", true);
		I_ASSIGN_TO(m_subtaskModelsCompPtr, m_subtasksCompPtr, true);
		I_ASSIGN_TO(m_subtaskInspectionCompPtr, m_subtasksCompPtr, false);
		I_ASSIGN_TO(m_subtaskInfoProviderCompPtr, m_subtasksCompPtr, false);
		I_ASSIGN_MULTI_0(m_additionalSupppliersCompPtr, "AdditionalSuppliers", "Set of suppliers will be initialized and controlled together with subtasks, but they will be not listed as subtasks", false)
		I_ASSIGN(m_serializeSuppliersAttrPtr, "SerializeSuppliers", "If it is true, parameters of suppliers will be serialized", true, true);
		I_ASSIGN(m_reduceHierarchyAttrPtr, "ReduceHierarchy", "If it is true, sub inspection tasks will be rolled out", true, false);
		I_ASSIGN(m_generalParamsCompPtr, "GeneralParams", "Optional general parameter set, it will be always serialized", false, "GeneralParams");
		I_ASSIGN_TO(m_generalParamsModelCompPtr, m_generalParamsCompPtr, true);
	I_END_COMPONENT;

	CInspectionTaskComp();

	// reimplemented (iinsp::IInspectionTask)
	virtual int GetSubtasksCount() const;
	virtual iproc::ISupplier* GetSubtask(int subtaskIndex) const;
	virtual iprm::IParamsSet* GetGeneralParameters() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (iproc::ISupplier)
	virtual int GetWorkStatus() const;
	virtual void InvalidateSupplier();
	virtual void EnsureWorkInitialized();
	virtual void EnsureWorkFinished();
	virtual void ClearWorkResults();
	virtual const ibase::IMessageContainer* GetWorkMessages() const;
	virtual iprm::IParamsSet* GetModelParametersSet() const;

	// reimplemented (istd::IInformationProvider)
	virtual QDateTime GetInformationTimeStamp() const;
	virtual InformationCategory GetInformationCategory() const;
	virtual int GetInformationId() const;
	virtual QString GetInformationDescription() const;
	virtual QString GetInformationSource() const;
	virtual int GetInformationFlags() const;

protected:
	void EnsureStatusKnown();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	class MessageContainer: virtual public ibase::IMessageContainer
	{
	public:
		MessageContainer(CInspectionTaskComp* parentPtr);

		// reimplemented (ibase::IMessageContainer)
		virtual int GetWorstCategory() const;
		virtual Messages GetMessages() const;
		virtual void ClearMessages();

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

	private:
		CInspectionTaskComp* m_parentPtr;
	};

	class Parameters:
				public imod::CMultiModelBridgeBase,
				virtual public iprm::IParamsSet
	{
	public:
		Parameters();

		void SetParent(CInspectionTaskComp* parentPtr);

		// reimplemented (iprm::IParamsSet)
		virtual Ids GetParamIds(bool editableOnly = false) const;
		virtual const iser::ISerializable* GetParameter(const QByteArray& id) const;
		virtual iser::ISerializable* GetEditableParameter(const QByteArray& id);

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

	private:
		CInspectionTaskComp* m_parentPtr;
	};

	I_MULTIREF(iproc::ISupplier, m_subtasksCompPtr);
	I_MULTIREF(imod::IModel, m_subtaskModelsCompPtr);
	I_MULTIREF(IInspectionTask, m_subtaskInspectionCompPtr);
	I_MULTIREF(istd::IInformationProvider, m_subtaskInfoProviderCompPtr);
	I_MULTIREF(iproc::ISupplier, m_additionalSupppliersCompPtr);
	I_ATTR(bool, m_serializeSuppliersAttrPtr);
	I_ATTR(bool, m_reduceHierarchyAttrPtr);
	I_REF(iprm::IParamsSet, m_generalParamsCompPtr);
	I_REF(imod::IModel, m_generalParamsModelCompPtr);

	typedef QVector<iproc::ISupplier*> Suppliers;
	Suppliers m_subtasks;

	typedef imod::TModelWrap<Parameters> ParametersModel;
	mutable ParametersModel m_parameters;

	template <class InterfaceType>
	static InterfaceType* ExtractParameters(CInspectionTaskComp& component)
	{
		return &component.m_parameters;
	}

	bool m_isStatusKnown;

	QDateTime m_resultTypeStamp;
	InformationCategory m_resultCategory;
	QString m_resultDescription;

	MessageContainer m_messageContainer;

	istd::CChangeNotifier m_productChangeNotifier;

	typedef QMap<iproc::ISupplier*, istd::CChangeNotifier> SubtaskNotifiers;
	SubtaskNotifiers m_subtaskNotifiers;
};


} // namespace iinsp


#endif // !iinsp_CInspectionTaskComp_included


