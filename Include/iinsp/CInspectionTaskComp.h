// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iinsp_CInspectionTaskComp_included
#define iinsp_CInspectionTaskComp_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <istd/IInformationProvider.h>
#include <istd/CChangeNotifier.h>
#include <imod/IModel.h>
#include <imod/TModelWrap.h>
#include <imod/CModelUpdateBridge.h>
#include <imod/CMultiModelObserverBase.h>
#include <ilog/CMessageContainer.h>
#include <ilog/TLoggerCompWrap.h>

// ACF-Solutions includes
#include <iinsp/IInspectionTask.h>


namespace iinsp
{


/**
	Standard component implementation of interface iinsp::IInspectionTask.
	Serializing of this object serialize all inspection parameters.

	\note This implementation contains several aspects of the data model:
	1. The component itself represents the data model of the information status of the whole inspection.
	2. The sub-component \c Parameters represents the parameter of all sub-tasks.
	3. Working status of the whole inspection provided by the method GetWorkStatus.
*/
class CInspectionTaskComp:
			public ilog::CLoggerComponentBase,
			virtual public IInspectionTask,
			virtual public iinsp::ISupplier,
			virtual public istd::IInformationProvider,
			protected imod::CModelUpdateBridge
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;
	typedef imod::CModelUpdateBridge BaseClass2;

	enum MessageId
	{
		MI_BAD_PARAMS_COUNT = 0x5af30,
		MI_NO_SUBTASK,
		MI_LOCAL
	};

	I_BEGIN_COMPONENT(CInspectionTaskComp);
		I_REGISTER_INTERFACE(IInspectionTask);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(iinsp::ISupplier);
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
		I_ASSIGN(m_diagnosticNameAttrPtr, "DiagnosticName", "Name of this supplier for diagnostic, if it is not set, no diagnostic log message will be send", false, "");
		I_ASSIGN(m_supportTempMessagesAttrPtr, "SupportTempMessages", "If enabled cumulation of temporary messages will be supported", true, false);
		I_ASSIGN(m_selfTaskCompPtr, "SelfTask", "This task implementation", false, "SelfTask");
	I_END_COMPONENT;

	CInspectionTaskComp();

	// reimplemented (iinsp::IInspectionTask)
	virtual int GetSubtasksCount() const override;
	virtual iinsp::ISupplier* GetSubtask(int subtaskIndex) const override;
	virtual iprm::IParamsSet* GetGeneralParameters() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (iinsp::ISupplier)
	virtual iinsp::ISupplier::WorkStatus GetWorkStatus() const override;
	virtual imod::IModel* GetWorkStatusModel() const override;
	virtual void InvalidateSupplier() override;
	virtual void EnsureWorkInitialized() override;
	virtual void EnsureWorkFinished() override;
	virtual void ClearWorkResults() override;
	virtual const ilog::IMessageContainer* GetWorkMessages(MessageContainerType messageType) const override;
	virtual iprm::IParamsSet* GetModelParametersSet() const override;

	// reimplemented (istd::IInformationProvider)
	virtual QDateTime GetInformationTimeStamp() const override;
	virtual InformationCategory GetInformationCategory() const override;
	virtual int GetInformationId() const override;
	virtual QString GetInformationDescription() const override;
	virtual QString GetInformationSource() const override;
	virtual int GetInformationFlags() const override;

protected:
	void EnsureStatusKnown();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) override;

protected:
	class MessageContainer: virtual public ilog::CMessageContainer
	{
	public:
		typedef ilog::CMessageContainer BaseClass;

		MessageContainer(CInspectionTaskComp* parentPtr, iinsp::ISupplier::MessageContainerType containerType);

		// reimplemented (ilog::IMessageContainer)
		virtual istd::IInformationProvider::InformationCategory GetWorstCategory() const override;
		virtual Messages GetMessages() const override;
		virtual void ClearMessages() override;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive) override;

	private:
		CInspectionTaskComp* m_parentPtr;
		iinsp::ISupplier::MessageContainerType m_containerType;
	};

	class Parameters:
			virtual public iprm::IParamsSet,
			public imod::CModelUpdateBridge
	{
	public:
		Parameters();

		void SetParent(CInspectionTaskComp* parentPtr);

		// reimplemented (iprm::IParamsSet)
		virtual Ids GetParamIds(bool editableOnly = false) const override;
		virtual const iser::ISerializable* GetParameter(const QByteArray& id) const override;
		virtual iser::ISerializable* GetEditableParameter(const QByteArray& id) override;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive) override;

		// reimplemented (istd::IChangeable)
		virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;
		
		// reimplemented (istd::IChangeable)
		virtual bool CopyFrom(const istd::IChangeable& object, istd::IChangeable::CompatibilityMode mode = CM_WITHOUT_REFS) override;

	private:
		CInspectionTaskComp* m_parentPtr;
	};

	class TaskStatusObserver: public imod::CMultiModelObserverBase
	{
	public:
		typedef imod::CMultiModelObserverBase BaseClass;

		TaskStatusObserver(CInspectionTaskComp* parentPtr);

		// reimplemented (imod::IObserver)
		virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) override;

	private:
		CInspectionTaskComp* m_parentPtr;
	};

	class Status: virtual public istd::IChangeable
	{
	public:
		Status();

		iinsp::ISupplier::WorkStatus GetSupplierState() const;
		void SetSupplierState(iinsp::ISupplier::WorkStatus state);

	private:
		iinsp::ISupplier::WorkStatus m_state;
	};

	typedef imod::TModelWrap<Status> StatusModel;

	I_MULTIREF(iinsp::ISupplier, m_subtasksCompPtr);
	I_MULTIREF(imod::IModel, m_subtaskModelsCompPtr);
	I_MULTIREF(IInspectionTask, m_subtaskInspectionCompPtr);
	I_MULTIREF(istd::IInformationProvider, m_subtaskInfoProviderCompPtr);
	I_MULTIREF(iinsp::ISupplier, m_additionalSupppliersCompPtr);
	I_ATTR(bool, m_serializeSuppliersAttrPtr);
	I_ATTR(bool, m_reduceHierarchyAttrPtr);
	I_REF(iprm::IParamsSet, m_generalParamsCompPtr);
	I_REF(imod::IModel, m_generalParamsModelCompPtr);
	I_ATTR(QString, m_diagnosticNameAttrPtr);
	I_ATTR(bool, m_supportTempMessagesAttrPtr);
	I_REF(iinsp::ISupplier, m_selfTaskCompPtr);

	typedef QVector<iinsp::ISupplier*> Suppliers;
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

	MessageContainer m_resultMessages;
	MessageContainer m_tempContainer;

	istd::IChangeable::ChangeSet m_supplierResultsChangeSet;
	typedef istd::TDelPtr<istd::CChangeNotifier> NotifierPtr;

	typedef QMap<iinsp::ISupplier*, NotifierPtr> SubtaskNotifiers;
	SubtaskNotifiers m_subtaskNotifiers;

	TaskStatusObserver m_subTaskStatusObserver;

	StatusModel m_workStatus;
};


} // namespace iinsp


#endif // !iinsp_CInspectionTaskComp_included


