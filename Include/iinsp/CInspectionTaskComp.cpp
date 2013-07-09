/********************************************************************************
**
**	Copyright (c) 2007-2011 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF-Solutions Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org, write info@imagingtools.de or contact
**	by Skype to ACF_infoline for further information about the ACF-Solutions.
**
********************************************************************************/


#include "iinsp/CInspectionTaskComp.h"


// ACF includes
#include "istd/CGeneralTimeStamp.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iinsp
{


CInspectionTaskComp::CInspectionTaskComp()
:	m_isStatusKnown(false),
	m_resultCategory(IC_NONE),
	m_messageContainer(this),
	m_productChangeNotifier(NULL, CF_SUPPLIER_RESULTS | CF_MODEL)
{
	// Only processing time message allowed:
	m_messageContainer.SetMaxMessageCount(1);
}


// reimplemented (iinsp::IInspectionTask)

int CInspectionTaskComp::GetSubtasksCount() const
{
	return m_subtasks.size();
}


iproc::ISupplier* CInspectionTaskComp::GetSubtask(int subtaskIndex) const
{
	return m_subtasks[subtaskIndex];
}


iprm::IParamsSet* CInspectionTaskComp::GetGeneralParameters() const
{
	return 	m_generalParamsCompPtr.GetPtr();
}


// reimplemented (iser::ISerializable)

bool CInspectionTaskComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	if (*m_serializeSuppliersAttrPtr){
		static iser::CArchiveTag taskListTag("SubtaskList", "List of inspection subtasks");
		static iser::CArchiveTag taskTag("Subtask", "Single subtask");

		int subtasksCount = m_subtasksCompPtr.GetCount();

		retVal = retVal && archive.BeginMultiTag(taskListTag, taskTag, subtasksCount);

		if (!retVal || (!archive.IsStoring() && (subtasksCount != m_subtasksCompPtr.GetCount()))){
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


// reimplemented (iproc::ISupplier)

int CInspectionTaskComp::GetWorkStatus() const
{
	int retVal = WS_INVALID;
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


void CInspectionTaskComp::InvalidateSupplier()
{
	int inspectionsCount = m_subtasksCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		iproc::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->InvalidateSupplier();
		}
	}

	// additional suppliers
	int addSuppliersCount = m_additionalSupppliersCompPtr.GetCount();
	for (int i = 0; i < addSuppliersCount; ++i){
		iproc::ISupplier* supplierPtr = m_additionalSupppliersCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->InvalidateSupplier();
		}
	}
}


void CInspectionTaskComp::EnsureWorkInitialized()
{
	m_productChangeNotifier.SetPtr(this);

	int inspectionsCount = m_subtasksCompPtr.GetCount();

	// set change notifier for each input supplier
	for (int i = 0; i < inspectionsCount; ++i){
		iproc::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			m_subtaskNotifiers[supplierPtr].SetPtr(supplierPtr);
		}
	}

	// set change notifier for additional suppliers
	int addSuppliersCount = m_additionalSupppliersCompPtr.GetCount();
	for (int i = 0; i < addSuppliersCount; ++i){
		iproc::ISupplier* supplierPtr = m_additionalSupppliersCompPtr[i];
		if (supplierPtr != NULL){
			m_subtaskNotifiers[supplierPtr].SetPtr(supplierPtr);
		}
	}

	// delegate the work initialization to each supplier
	for (int i = 0; i < inspectionsCount; ++i){
		iproc::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->EnsureWorkInitialized();
		}
	}

	// additional suppliers
	for (int i = 0; i < addSuppliersCount; ++i){
		iproc::ISupplier* supplierPtr = m_additionalSupppliersCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->EnsureWorkInitialized();
		}
	}
}


void CInspectionTaskComp::EnsureWorkFinished()
{
	istd::CGeneralTimeStamp timer;

	int inspectionsCount = m_subtasksCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		iproc::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->EnsureWorkFinished();
		}
	}

	// additional suppliers
	int addSuppliersCount = m_additionalSupppliersCompPtr.GetCount();
	for (int i = 0; i < addSuppliersCount; ++i){
		iproc::ISupplier* supplierPtr = m_additionalSupppliersCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->EnsureWorkInitialized();
		}
	}

	if (m_diagnosticNameAttrPtr.IsValid()){
		istd::TSmartPtr<const istd::IInformationProvider> messagePtr(new ilog::CMessage(
				istd::IInformationProvider::IC_INFO,
				0,
				QObject::tr("Processing took %1 ms").arg(timer.GetElapsed() * 1000),
				*m_diagnosticNameAttrPtr));
		
		m_messageContainer.AddMessage(messagePtr);
	}

	m_subtaskNotifiers.clear();
	m_productChangeNotifier.Reset();
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

	// additional suppliers
	int addSuppliersCount = m_additionalSupppliersCompPtr.GetCount();
	for (int i = 0; i < addSuppliersCount; ++i){
		iproc::ISupplier* supplierPtr = m_additionalSupppliersCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->ClearWorkResults();
		}
	}

	m_productChangeNotifier.Reset();
}


const ilog::IMessageContainer* CInspectionTaskComp::GetWorkMessages() const
{
	return &m_messageContainer;
}


iprm::IParamsSet* CInspectionTaskComp::GetModelParametersSet() const
{
	return &m_parameters;
}


// reimplemented (istd::IInformationProvider)

QDateTime CInspectionTaskComp::GetInformationTimeStamp() const
{
	const_cast<CInspectionTaskComp*>(this)->EnsureStatusKnown();

	return m_resultTypeStamp;
}


istd::IInformationProvider::InformationCategory CInspectionTaskComp::GetInformationCategory() const
{
	const_cast<CInspectionTaskComp*>(this)->EnsureStatusKnown();

	return m_resultCategory;
}


int CInspectionTaskComp::GetInformationId() const
{
	return -1;
}


QString CInspectionTaskComp::GetInformationDescription() const
{
	const_cast<CInspectionTaskComp*>(this)->EnsureStatusKnown();

	return m_resultDescription;
}


QString CInspectionTaskComp::GetInformationSource() const
{
	return QObject::tr("Inspection task");
}


int CInspectionTaskComp::GetInformationFlags() const
{
	return ITF_SYSTEM;
}


// protected methods

void CInspectionTaskComp::EnsureStatusKnown()
{
	if (!m_isStatusKnown){
		m_resultTypeStamp = QDateTime::currentDateTime();
		m_resultCategory = IC_INFO;
		m_resultDescription = "";

		int infoSubtasksCount = m_subtasksCompPtr.GetCount();
		int subtasksCount = m_subtasksCompPtr.GetCount();
		for (int i = 0; i < subtasksCount; ++i){
			const iproc::ISupplier* supplierPtr = m_subtasksCompPtr[i];
			if (supplierPtr != NULL){
				int workStatus = supplierPtr->GetWorkStatus();

				if (workStatus < WS_OK){	// no result was calculated for this subtask
					m_resultCategory = IC_NONE;
					continue;
				}

				if (i < infoSubtasksCount){
					const istd::IInformationProvider* infoProviderPtr = m_subtaskInfoProviderCompPtr[i];
					if (infoProviderPtr != NULL){
						InformationCategory category = infoProviderPtr->GetInformationCategory();
						if (category > m_resultCategory){
							m_resultCategory = category;
							m_resultDescription = infoProviderPtr->GetInformationDescription();
						}
					}
				}

				if (workStatus >= WS_ERROR){
					InformationCategory supplierResult = (workStatus >= WS_CRITICAL)?
								IC_CRITICAL:
								IC_ERROR;
					if (supplierResult > m_resultCategory){
						m_resultCategory = supplierResult;
						m_resultDescription = "";
					}
				}
			}
		}

		m_isStatusKnown = true;
	}
}


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

	m_subtasks.clear();

	int subtasksCount = m_subtasksCompPtr.GetCount();
	for (int i = 0; i < subtasksCount; ++i){
		if (*m_reduceHierarchyAttrPtr){
			if (i < m_subtaskInspectionCompPtr.GetCount()){
				IInspectionTask* inspectionTaskPtr = m_subtaskInspectionCompPtr[i];
				if (inspectionTaskPtr != NULL){
					int childTasksCount = inspectionTaskPtr->GetSubtasksCount();
					for (int childIndex = 0; childIndex < childTasksCount; ++childIndex){
						iproc::ISupplier* taskPtr = inspectionTaskPtr->GetSubtask(childIndex);

						m_subtasks.push_back(taskPtr);
					}
					continue;
				}
			}
		}

		iproc::ISupplier* taskPtr = m_subtasksCompPtr[i];

		m_subtasks.push_back(taskPtr);
	}

	m_isStatusKnown = false;

	m_parameters.SetParent(this);
}


void CInspectionTaskComp::OnComponentDestroyed()
{
	m_subtaskNotifiers.clear();
	m_subtasks.clear();

	EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}



// reimplemented (imod::IObserver)

void CInspectionTaskComp::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	m_isStatusKnown = false;

	BaseClass2::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// public methods of embedded class MessageContainer

CInspectionTaskComp::MessageContainer::MessageContainer(CInspectionTaskComp* parentPtr)
:	m_parentPtr(parentPtr)
{
}


// reimplemented (ilog::IMessageContainer)

int CInspectionTaskComp::MessageContainer::GetWorstCategory() const
{
	m_parentPtr->EnsureStatusKnown();

	return m_parentPtr->m_resultCategory;
}


ilog::IMessageContainer::Messages CInspectionTaskComp::MessageContainer::GetMessages() const
{
	ilog::IMessageContainer::Messages retVal = BaseClass::GetMessages();

	int subtasksCount = m_parentPtr->m_subtasksCompPtr.GetCount();
	for (int i = 0; i < subtasksCount; ++i){
		const iproc::ISupplier* supplierPtr = m_parentPtr->m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			const ilog::IMessageContainer* containerPtr = supplierPtr->GetWorkMessages();

			if (containerPtr != NULL){
				retVal += containerPtr->GetMessages();
			}
		}
	}

	return retVal;
}


void CInspectionTaskComp::MessageContainer::ClearMessages()
{
	BaseClass::ClearMessages();
}


// reimplemented (iser::ISerializable)

bool CInspectionTaskComp::MessageContainer::Serialize(iser::IArchive& archive)
{
	if (!archive.IsStoring()){
		return false;
	}

	bool retVal = true;

	int subtasksCount = m_parentPtr->m_subtasksCompPtr.GetCount();
	for (int i = 0; i < subtasksCount; ++i){
		iproc::ISupplier* supplierPtr = m_parentPtr->m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			ilog::IMessageContainer* containerPtr = const_cast<ilog::IMessageContainer*>(supplierPtr->GetWorkMessages());

			if (containerPtr != NULL){
				retVal = containerPtr->Serialize(archive) && retVal;
			}
		}
	}

	return retVal;
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

			if (m_parentPtr->m_generalParamsModelCompPtr.IsValid()){
				m_parentPtr->m_generalParamsModelCompPtr->AttachObserver(this);
			}

			if (*m_parentPtr->m_serializeSuppliersAttrPtr){
				int subtasksCount = m_parentPtr->m_subtasksCompPtr.GetCount();
				for (int i = 0; i < subtasksCount; ++i){
					const iproc::ISupplier* subtaskPtr = m_parentPtr->m_subtasksCompPtr[i];
					if (subtaskPtr != NULL){
						imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(subtaskPtr->GetModelParametersSet());
						if (modelPtr != NULL && !modelPtr->IsAttached(this)){
							modelPtr->AttachObserver(this);
						}
					}
				}
			}
		}
	}
}


// reimplemented (iprm::IParamsSet)

iprm::IParamsSet::Ids CInspectionTaskComp::Parameters::GetParamIds(bool editableOnly) const
{
	iprm::IParamsSet::Ids retVal;

	if (m_parentPtr != NULL){
		if (m_parentPtr->m_generalParamsCompPtr.IsValid()){
			retVal += m_parentPtr->m_generalParamsCompPtr->GetParamIds(editableOnly);
		}

		if (*m_parentPtr->m_serializeSuppliersAttrPtr){
			int subtasksCount = m_parentPtr->m_subtasksCompPtr.GetCount();
			for (int i = 0; i < subtasksCount; ++i){
				const iproc::ISupplier* subtaskPtr = m_parentPtr->m_subtasksCompPtr[i];
				if (subtaskPtr != NULL){
					iprm::IParamsSet* paramsSetPtr = subtaskPtr->GetModelParametersSet();
					if (paramsSetPtr != NULL){
						retVal += paramsSetPtr->GetParamIds(editableOnly);
					}
				}
			}
		}
	}

	return retVal;
}


const iser::ISerializable* CInspectionTaskComp::Parameters::GetParameter(const QByteArray& id) const
{
	if (m_parentPtr != NULL){
		if (m_parentPtr->m_generalParamsCompPtr.IsValid()){
			const iser::ISerializable* paramPtr = m_parentPtr->m_generalParamsCompPtr->GetParameter(id);
			if (paramPtr != NULL){
				return paramPtr;
			}
		}

		if (*m_parentPtr->m_serializeSuppliersAttrPtr){
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

		if (*m_parentPtr->m_serializeSuppliersAttrPtr){
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
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CInspectionTaskComp::Parameters::Serialize(iser::IArchive& archive)
{
	if (m_parentPtr != NULL){
		return m_parentPtr->CInspectionTaskComp::Serialize(archive);
	}

	return false;
}


} // namespace iinsp


