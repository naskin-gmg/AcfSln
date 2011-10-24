#include "iinsp/CInspectionTaskComp.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iinsp
{


// reimplemented (iinsp::IInspectionTask)

int CInspectionTaskComp::GetSubtasksCount() const
{
	return m_subtasksCompPtr.GetCount();
}


iproc::ISupplier* CInspectionTaskComp::GetSubtask(int subtaskIndex) const
{
	return m_subtasksCompPtr[subtaskIndex];
}


iprm::IParamsSet* CInspectionTaskComp::GetTaskParams() const
{
	return m_generalParamsCompPtr.GetPtr();
}


// reimplemented (iser::ISerializable)

bool CInspectionTaskComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	if (*m_serialzeSuppliersAttrPtr){
		static iser::CArchiveTag taskListTag("SubtaskList", "List of inspection subtasks");
		static iser::CArchiveTag taskTag("Subtask", "Single subtask");

		int subtasksCount = m_subtasksCompPtr.GetCount();

		retVal = retVal && archive.BeginMultiTag(taskListTag, taskTag, subtasksCount);

		if (!retVal || (!archive.IsStoring() && subtasksCount != m_subtasksCompPtr.GetCount())){
			SendWarningMessage(MI_BAD_PARAMS_COUNT, "Bad number of parameter to serialize");
			return false;
		}

		for (int i = 0; i < subtasksCount; ++i){
			retVal = retVal && archive.BeginTag(taskTag);

			iproc::ISupplier* taskPtr = m_subtasksCompPtr[i];
			if (taskPtr == NULL){
				SendCriticalMessage(MI_NO_SUBTASK, "No subtask connected");
				return false;
			}

			iprm::IParamsSet* paramsSetPtr = taskPtr->GetModelParametersSet();
			if (paramsSetPtr != NULL){
				retVal = retVal && paramsSetPtr->Serialize(archive);
			}

			retVal = retVal && archive.EndTag(taskTag);
		}

		retVal = retVal && archive.EndTag(taskListTag);
	}

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

void CInspectionTaskComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int modelsCount = m_subtaskModelsCompPtr.GetCount();

	for (int i = 0; i < modelsCount; ++i){
		imod::IModel* modelPtr = m_subtaskModelsCompPtr[i];
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
		}
	}
}


void CInspectionTaskComp::OnComponentDestroyed()
{
	int modelsCount = m_subtaskModelsCompPtr.GetCount();

	for (int i = 0; i < modelsCount; ++i){
		imod::IModel* modelPtr = m_subtaskModelsCompPtr[i];
		if ((modelPtr != NULL) && modelPtr->IsAttached(this)){
			modelPtr->DetachObserver(this);
		}
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iinsp


