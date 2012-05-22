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


// reimplemented (iproc::ISupplier)

void CInspectionTaskComp::InvalidateSupplier()
{
	int inspectionsCount = m_subtasksCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		iproc::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->InvalidateSupplier();
		}
	}
}


void CInspectionTaskComp::EnsureWorkFinished()
{
	int inspectionsCount = m_subtasksCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		iproc::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->EnsureWorkFinished();
		}
	}
}


void CInspectionTaskComp::ClearWorkResults()
{
	int inspectionsCount = m_subtasksCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		iproc::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->ClearWorkResults();
		}
	}
}


int CInspectionTaskComp::GetWorkStatus() const
{
	int retVal = WS_NONE;
	int inspectionsCount = m_subtasksCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		const iproc::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			int workStatus = supplierPtr->GetWorkStatus();
			if (workStatus > retVal){
				retVal = workStatus;
			}
		}
	}

	return retVal;
}


iprm::IParamsSet* CInspectionTaskComp::GetModelParametersSet() const
{
	return &m_parameters;
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

	m_parameters.SetParent(this);
}


void CInspectionTaskComp::OnComponentDestroyed()
{
	m_parameters.SetParent(NULL);

	EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


// public methods of embedded class Parameters

CInspectionTaskComp::Parameters::Parameters()
:	m_parentPtr(NULL)
{
}


void CInspectionTaskComp::Parameters::SetParent(CInspectionTaskComp* parentPtr)
{
	if (parentPtr != m_parentPtr){
		if (m_parentPtr != NULL){
			EnsureModelsDetached();

			m_parentPtr = NULL;
		}

		if (parentPtr != NULL){
			m_parentPtr = parentPtr;

			int subtasksCount = m_parentPtr->m_subtasksCompPtr.GetCount();

			if (m_parentPtr->m_generalParamsModelCompPtr.IsValid()){
				m_parentPtr->m_generalParamsModelCompPtr->AttachObserver(this);
			}

			for (int i = 0; i < subtasksCount; ++i){
				const iproc::ISupplier* subtaskPtr = m_parentPtr->m_subtasksCompPtr[i];
				if (subtaskPtr != NULL){
					imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(subtaskPtr->GetModelParametersSet());
					if (modelPtr != NULL){
						modelPtr->AttachObserver(this);
					}
				}
			}
		}
	}
}


// reimplemented (iprm::IParamsSet)

const iser::ISerializable* CInspectionTaskComp::Parameters::GetParameter(const QByteArray& id) const
{
	if (m_parentPtr != NULL){
		if (m_parentPtr->m_generalParamsCompPtr.IsValid()){
			const iser::ISerializable* paramPtr = m_parentPtr->m_generalParamsCompPtr->GetParameter(id);
			if (paramPtr != NULL){
				return paramPtr;
			}
		}

		int subtasksCount = m_parentPtr->m_subtasksCompPtr.GetCount();

		for (int i = 0; i < subtasksCount; ++i){
			const iproc::ISupplier* subtaskPtr = m_parentPtr->m_subtasksCompPtr[i];
			if (subtaskPtr != NULL){
				iprm::IParamsSet* paramsSetPtr = subtaskPtr->GetModelParametersSet();
				if (paramsSetPtr != NULL){
					const iser::ISerializable* paramPtr = paramsSetPtr->GetParameter(id);
					if (paramPtr != NULL){
						return paramPtr;
					}
				}
			}
		}
	}

	return NULL;
}


iser::ISerializable* CInspectionTaskComp::Parameters::GetEditableParameter(const QByteArray& id)
{
	if (m_parentPtr != NULL){
		if (m_parentPtr->m_generalParamsCompPtr.IsValid()){
			iser::ISerializable* paramPtr = m_parentPtr->m_generalParamsCompPtr->GetEditableParameter(id);
			if (paramPtr != NULL){
				return paramPtr;
			}
		}

		int subtasksCount = m_parentPtr->m_subtasksCompPtr.GetCount();

		for (int i = 0; i < subtasksCount; ++i){
			iproc::ISupplier* subtaskPtr = m_parentPtr->m_subtasksCompPtr[i];
			if (subtaskPtr != NULL){
				iprm::IParamsSet* paramsSetPtr = subtaskPtr->GetModelParametersSet();
				if (paramsSetPtr != NULL){
					iser::ISerializable* paramPtr = paramsSetPtr->GetEditableParameter(id);
					if (paramPtr != NULL){
						return paramPtr;
					}
				}
			}
		}
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CInspectionTaskComp::Parameters::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	if (m_parentPtr != NULL){
		if (*m_parentPtr->m_serializeSuppliersAttrPtr){
			static iser::CArchiveTag taskListTag("SubtaskList", "List of inspection subtasks");
			static iser::CArchiveTag taskTag("Subtask", "Single subtask");

			int subtasksCount = m_parentPtr->m_subtasksCompPtr.GetCount();

			retVal = retVal && archive.BeginMultiTag(taskListTag, taskTag, subtasksCount);

			if (!retVal || (!archive.IsStoring() && (subtasksCount != m_parentPtr->m_subtasksCompPtr.GetCount()))){
				m_parentPtr->SendWarningMessage(MI_BAD_PARAMS_COUNT, "Bad number of parameter to serialize");
				return false;
			}

			for (int i = 0; i < subtasksCount; ++i){
				retVal = retVal && archive.BeginTag(taskTag);

				iproc::ISupplier* taskPtr = m_parentPtr->m_subtasksCompPtr[i];
				if (taskPtr == NULL){
					m_parentPtr->SendCriticalMessage(MI_NO_SUBTASK, "No subtask connected");
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

		if (m_parentPtr->m_generalParamsCompPtr.IsValid()){
			static iser::CArchiveTag generalParamsTag("GeneralParams", "General inspection parameters");

			retVal = retVal && archive.BeginTag(generalParamsTag);
			retVal = retVal && m_parentPtr->m_generalParamsCompPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(generalParamsTag);
		}
	}
	else{
		retVal = false;
	}

	return retVal;
}


} // namespace iinsp


