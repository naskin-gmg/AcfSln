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
			protected imod::CMultiModelBridgeBase
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_BAD_TASKS_COUNT = 0x5af40,
		MI_NO_TASK
	};

	I_BEGIN_COMPONENT(CInspectionComp);
		I_REGISTER_INTERFACE(IInspectionTask);
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
	virtual iprm::IParamsSet* GetTaskParams() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_MULTIREF(IInspectionTask, m_inspectionsCompPtr);
	I_MULTIREF(imod::IModel, m_inspectionModelsCompPtr);
	I_REF(iprm::IParamsSet, m_generalParamsCompPtr);
};


} // namespace iinsp


#endif // !iinsp_CInspectionComp_included


