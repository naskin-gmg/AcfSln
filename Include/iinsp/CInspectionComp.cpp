#include "iinsp/CInspectionComp.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iinsp
{


// reimplemented (iinsp::IInspection)

int CInspectionComp::GetTasksCount() const
{
	return m_inspectionsCompPtr.GetCount();
}


IInspectionTask* CInspectionComp::GetTask(int subtaskIndex) const
{
	I_ASSERT(subtaskIndex >= 0);
	I_ASSERT(subtaskIndex < m_inspectionsCompPtr.GetCount());

	return m_inspectionsCompPtr[subtaskIndex];
}


iprm::IParamsSet* CInspectionComp::GetInspectionParams() const
{
	return m_generalParamsCompPtr.GetPtr();
}


// reimplemented (iinsp::IInspectionTask)

int CInspectionComp::GetSubtasksCount() const
{
	int retVal = 0;

	int inspectionsCount = m_inspectionsCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		const IInspectionTask* taskPtr = m_inspectionsCompPtr[i];
		if (taskPtr != NULL){
			retVal += taskPtr->GetSubtasksCount();
		}
	}

	return retVal;
}


iproc::ISupplier* CInspectionComp::GetSubtask(int subtaskIndex) const
{
	int inspectionsCount = m_inspectionsCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		const IInspectionTask* taskPtr = m_inspectionsCompPtr[i];
		if (taskPtr != NULL){
			int subtasksCount = taskPtr->GetSubtasksCount();
			if (subtaskIndex < subtasksCount){
				return taskPtr->GetSubtask(subtaskIndex);
			}

			subtaskIndex -= subtasksCount;
			I_ASSERT(subtaskIndex >= 0);
		}
	}

	return NULL;
}


iprm::IParamsSet* CInspectionComp::GetTaskParams() const
{
	return m_generalParamsCompPtr.GetPtr();
}


// reimplemented (iser::ISerializable)

bool CInspectionComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag taskListTag("TaskList", "List of inspection tasks");
	static iser::CArchiveTag taskTag("Task", "Single task");

	int subtasksCount = m_inspectionsCompPtr.GetCount();

	retVal = retVal && archive.BeginMultiTag(taskListTag, taskTag, subtasksCount);

	if (!retVal || (!archive.IsStoring() && subtasksCount != m_inspectionsCompPtr.GetCount())){
		SendWarningMessage(MI_BAD_TASKS_COUNT, "Bad number of tasks to serialize");
		return false;
	}

	for (int i = 0; i < subtasksCount; ++i){
		retVal = retVal && archive.BeginTag(taskTag);

		IInspectionTask* taskPtr = m_inspectionsCompPtr[i];
		if (taskPtr == NULL){
			SendCriticalMessage(MI_NO_TASK, "No task connected");
			return false;
		}

		retVal = retVal && taskPtr->Serialize(archive);

		retVal = retVal && archive.EndTag(taskTag);
	}

	retVal = retVal && archive.EndTag(taskListTag);

	if (m_generalParamsCompPtr.IsValid()){
		static iser::CArchiveTag generalParamsTag("GeneralParams", "General inspection parameters");

		retVal = retVal && archive.BeginTag(generalParamsTag);
		retVal = retVal && m_generalParamsCompPtr->Serialize(archive);
		retVal = retVal && archive.EndTag(generalParamsTag);
	}

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CInspectionComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int modelsCount = m_inspectionModelsCompPtr.GetCount();

	for (int i = 0; i < modelsCount; ++i){
		imod::IModel* modelPtr = m_inspectionModelsCompPtr[i];
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
		}
	}
}


void CInspectionComp::OnComponentDestroyed()
{
	int modelsCount = m_inspectionModelsCompPtr.GetCount();

	for (int i = 0; i < modelsCount; ++i){
		imod::IModel* modelPtr = m_inspectionModelsCompPtr[i];
		if ((modelPtr != NULL) && modelPtr->IsAttached(this)){
			modelPtr->DetachObserver(this);
		}
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iinsp


