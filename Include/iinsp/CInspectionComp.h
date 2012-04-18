#ifndef iinsp_CInspectionComp_included
#define iinsp_CInspectionComp_included


// ACF includes
#include "imod/IModel.h"
#include "imod/CMultiModelBridgeBase.h"
#include "ibase/TLoggerCompWrap.h"

// IACF includes
#include "iinsp/IInspectionTask.h"
#include "iinsp/IInspection.h"


namespace iinsp
{


/**
	Standard component implementation of interface iinsp::IInspectionTask.
*/
class CInspectionComp:
			public ibase::CLoggerComponentBase,
			virtual public IInspection,
			virtual public IInspectionTask,
			virtual public istd::IInformationProvider,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_INSPECTION_DONE = 0x5af40,
		MI_BAD_TASKS_COUNT,
		MI_NO_TASK
	};

	I_BEGIN_COMPONENT(CInspectionComp);
		I_REGISTER_INTERFACE(IInspection);
		I_REGISTER_INTERFACE(IInspectionTask);
		I_REGISTER_INTERFACE(iproc::ISupplier);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN_MULTI_0(m_inspectionsCompPtr, "InspectionTasks", "List of subinspections", true);
		I_ASSIGN_TO(m_inspectionModelsCompPtr, m_inspectionsCompPtr, false);
		I_ASSIGN(m_generalParamsCompPtr, "GeneralParams", "Optional general parameter set, it will be always serialized", false, "GeneralParams");
	I_END_COMPONENT;

	// reimplemented (iinsp::IInspection)
	virtual int GetTasksCount() const;
	virtual IInspectionTask* GetTask(int subtaskIndex) const;
	virtual iprm::IParamsSet* GetInspectionParams() const;

	// reimplemented (iinsp::IInspectionTask)
	virtual int GetSubtasksCount() const;
	virtual iproc::ISupplier* GetSubtask(int subtaskIndex) const;

	// reimplemented (iproc::ISupplier)
	virtual void InvalidateSupplier();
	virtual void EnsureWorkFinished();
	virtual void ClearWorkResults();
	virtual int GetWorkStatus() const;
	virtual iprm::IParamsSet* GetModelParametersSet() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IInformationProvider)
	virtual QDateTime GetInformationTimeStamp() const;
	virtual InformationCategory GetInformationCategory() const;
	virtual int GetInformationId() const;
	virtual QString GetInformationDescription() const;
	virtual QString GetInformationSource() const;
	virtual int GetInformationFlags() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_MULTIREF(IInspectionTask, m_inspectionsCompPtr);
	I_MULTIREF(imod::IModel, m_inspectionModelsCompPtr);
	I_REF(iprm::IParamsSet, m_generalParamsCompPtr);

	bool m_isWorkFinished;
	int m_workStatus;

	QDateTime m_timeStamp;
	InformationCategory m_informationCategory;
	int m_informationId;
	QString m_informationDescription;
	QString m_informationSource;
	int m_informationFlags;
};


} // namespace iinsp


#endif // !iinsp_CInspectionComp_included


