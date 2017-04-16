#include <iinsp/CInspectionTaskComp.h>


// ACF includes
#include <istd/CGeneralTimeStamp.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iinsp/IEnableableSupplier.h>


namespace iinsp
{


CInspectionTaskComp::CInspectionTaskComp()
:	BaseClass2(this),
	m_isStatusKnown(false),
	m_resultCategory(IC_NONE),
	m_resultMessages(this, ISupplier::MCT_RESULTS),
	m_tempContainer(this, ISupplier::MCT_TEMP),
	m_supplierResultsChangeSet(CF_SUPPLIER_RESULTS),
	m_subTaskStatusObserver(this)
{
	// Only processing time message allowed:
	m_resultMessages.SetMaxMessageCount(1);
}


// reimplemented (iinsp::IInspectionTask)

int CInspectionTaskComp::GetSubtasksCount() const
{
	return m_subtasks.size();
}


iinsp::ISupplier* CInspectionTaskComp::GetSubtask(int subtaskIndex) const
{
	return m_subtasks[subtaskIndex];
}


iprm::IParamsSet* CInspectionTaskComp::GetGeneralParameters() const
{
	return m_generalParamsCompPtr.GetPtr();
}


// reimplemented (iser::ISerializable)

bool CInspectionTaskComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag taskListTag("SubtaskList", "List of inspection subtasks", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag taskTag("Subtask", "Single subtask", iser::CArchiveTag::TT_GROUP, &taskListTag);
	static iser::CArchiveTag generalParamsTag("GeneralParams", "General inspection parameters", iser::CArchiveTag::TT_GROUP);

	bool retVal = true;

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

	if (*m_serializeSuppliersAttrPtr){
		int subtasksCount = m_subtasksCompPtr.GetCount();

		retVal = retVal && archive.BeginMultiTag(taskListTag, taskTag, subtasksCount);

		if (!retVal || (!archive.IsStoring() && (subtasksCount != m_subtasksCompPtr.GetCount()))){
			SendWarningMessage(MI_BAD_PARAMS_COUNT, "Bad number of parameter to serialize");
			return false;
		}

		for (int i = 0; i < subtasksCount; ++i){
			retVal = retVal && archive.BeginTag(taskTag);

			iinsp::ISupplier* taskPtr = m_subtasksCompPtr[i];
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
		retVal = retVal && archive.BeginTag(generalParamsTag);
		retVal = retVal && m_generalParamsCompPtr->Serialize(archive);
		retVal = retVal && archive.EndTag(generalParamsTag);
	}

	return retVal;
}


// reimplemented (iinsp::ISupplier)

int CInspectionTaskComp::GetWorkStatus() const
{
	return m_workStatus.GetSupplierState();
}


imod::IModel* CInspectionTaskComp::GetWorkStatusModel() const
{
	return &(const_cast<CInspectionTaskComp*>(this))->m_workStatus;
}


void CInspectionTaskComp::InvalidateSupplier()
{
	int inspectionsCount = m_subtasksCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		iinsp::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->InvalidateSupplier();
		}
	}

	// additional suppliers
	int addSuppliersCount = m_additionalSupppliersCompPtr.GetCount();
	for (int i = 0; i < addSuppliersCount; ++i){
		iinsp::ISupplier* supplierPtr = m_additionalSupppliersCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->InvalidateSupplier();
		}
	}
}


void CInspectionTaskComp::EnsureWorkInitialized()
{
	istd::CChangeNotifier changeNotifier(this, &m_supplierResultsChangeSet);

	int inspectionsCount = m_subtasksCompPtr.GetCount();

	// set change notifier for each input supplier
	for (int i = 0; i < inspectionsCount; ++i){
		iinsp::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			NotifierPtr& notifierPtr = m_subtaskNotifiers[supplierPtr];
			if (!notifierPtr.IsValid()){
				notifierPtr.SetPtr(new istd::CChangeNotifier(supplierPtr));
			}
		}
	}

	// set change notifier for additional suppliers
	int addSuppliersCount = m_additionalSupppliersCompPtr.GetCount();
	for (int i = 0; i < addSuppliersCount; ++i){
		iinsp::ISupplier* supplierPtr = m_additionalSupppliersCompPtr[i];
		if (supplierPtr != NULL){
			NotifierPtr& notifierPtr = m_subtaskNotifiers[supplierPtr];
			if (!notifierPtr.IsValid()){
				notifierPtr.SetPtr(new istd::CChangeNotifier(supplierPtr));
			}
		}
	}

	// delegate the work initialization to each supplier
	for (int i = 0; i < inspectionsCount; ++i){
		iinsp::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->EnsureWorkInitialized();
		}
	}

	// additional suppliers
	for (int i = 0; i < addSuppliersCount; ++i){
		iinsp::ISupplier* supplierPtr = m_additionalSupppliersCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->EnsureWorkInitialized();
		}
	}
}


void CInspectionTaskComp::EnsureWorkFinished()
{
	istd::CChangeNotifier changeNotifier(this, &m_supplierResultsChangeSet);

	istd::CGeneralTimeStamp timer;

	int inspectionsCount = m_subtasksCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		iinsp::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->EnsureWorkFinished();

			m_subtaskNotifiers[supplierPtr].Reset();
		}
	}

	// additional suppliers
	int addSuppliersCount = m_additionalSupppliersCompPtr.GetCount();
	for (int i = 0; i < addSuppliersCount; ++i){
		iinsp::ISupplier* supplierPtr = m_additionalSupppliersCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->EnsureWorkFinished();

			m_subtaskNotifiers[supplierPtr].Reset();
		}
	}

	if (m_diagnosticNameAttrPtr.IsValid()){
		ilog::IMessageConsumer::MessagePtr messagePtr(new ilog::CMessage(
				istd::IInformationProvider::IC_NONE,
				MI_LOCAL,
				QObject::tr("Processing took %1 ms").arg(timer.GetElapsed() * 1000),
				*m_diagnosticNameAttrPtr));
		
		m_resultMessages.AddMessage(messagePtr);
	}

	m_subtaskNotifiers.clear();
}


void CInspectionTaskComp::ClearWorkResults()
{
	istd::CChangeNotifier changeNotifier(this, &m_supplierResultsChangeSet);

	int inspectionsCount = m_subtasksCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		iinsp::ISupplier* supplierPtr = m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->ClearWorkResults();
		}
	}

	// additional suppliers
	int addSuppliersCount = m_additionalSupppliersCompPtr.GetCount();
	for (int i = 0; i < addSuppliersCount; ++i){
		iinsp::ISupplier* supplierPtr = m_additionalSupppliersCompPtr[i];
		if (supplierPtr != NULL){
			supplierPtr->ClearWorkResults();
		}
	}
}


const ilog::IMessageContainer* CInspectionTaskComp::GetWorkMessages(int containerType) const
{
	if (containerType == MCT_RESULTS){
		return &m_resultMessages;
	}
	else if (*m_supportTempMessagesAttrPtr && (containerType == MCT_TEMP)){
		return &m_tempContainer;
	}

	return NULL;
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

		int infoSubtasksCount = m_subtaskInfoProviderCompPtr.GetCount();
		int subtasksCount = m_subtasksCompPtr.GetCount();
		for (int i = 0; i < subtasksCount; ++i){
			const iinsp::ISupplier* supplierPtr = m_subtasksCompPtr[i];
			if (supplierPtr != NULL){
				int workStatus = supplierPtr->GetWorkStatus();

				if (workStatus < WS_OK){	// no result was calculated for this subtask
					const iinsp::IEnableableSupplier* enableableSupplierPtr = dynamic_cast<const iinsp::IEnableableSupplier*>(supplierPtr);
					if ((enableableSupplierPtr != NULL) && !enableableSupplierPtr->IsSupplierEnabled()){
						continue;
					}

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

	m_generalParamsCompPtr.EnsureInitialized();
	m_generalParamsModelCompPtr.EnsureInitialized();

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
						iinsp::ISupplier* taskPtr = inspectionTaskPtr->GetSubtask(childIndex);

						m_subtasks.push_back(taskPtr);
					}
					continue;
				}
			}
		}

		iinsp::ISupplier* taskPtr = m_subtasksCompPtr[i];
		if (taskPtr != NULL){
			imod::IModel* taskStatusModelPtr = taskPtr->GetWorkStatusModel();
			if (taskStatusModelPtr != NULL){
				taskStatusModelPtr->AttachObserver(&m_subTaskStatusObserver);
			}

			m_subtasks.push_back(taskPtr);
		}
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

void CInspectionTaskComp::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	m_isStatusKnown = false;

	BaseClass2::AfterUpdate(modelPtr, changeSet);
}


// public methods of embedded class MessageContainer

CInspectionTaskComp::MessageContainer::MessageContainer(
			CInspectionTaskComp* parentPtr,
			iinsp::ISupplier::MessageContainerType containerType)
:	m_parentPtr(parentPtr),
	m_containerType(containerType)
{
}


// reimplemented (ilog::IMessageContainer)

int CInspectionTaskComp::MessageContainer::GetWorstCategory() const
{
	if (m_containerType == MCT_RESULTS){
		m_parentPtr->EnsureStatusKnown();

		return m_parentPtr->m_resultCategory;
	}
	else{
		int retVal = istd::IInformationProvider::IC_NONE;

		int subtasksCount = m_parentPtr->m_subtasksCompPtr.GetCount();
		for (int i = 0; i < subtasksCount; ++i){
			const iinsp::ISupplier* supplierPtr = m_parentPtr->m_subtasksCompPtr[i];
			if (supplierPtr != NULL){
				const ilog::IMessageContainer* containerPtr = supplierPtr->GetWorkMessages(m_containerType);

				if (containerPtr != NULL){
					int category = containerPtr->GetWorstCategory();

					if (category > retVal){
						retVal = category;
					}
				}
			}
		}

		return retVal;
	}
}


ilog::IMessageContainer::Messages CInspectionTaskComp::MessageContainer::GetMessages() const
{
	ilog::IMessageContainer::Messages retVal = BaseClass::GetMessages();

	int subtasksCount = m_parentPtr->m_subtasksCompPtr.GetCount();
	for (int i = 0; i < subtasksCount; ++i){
		const iinsp::ISupplier* supplierPtr = m_parentPtr->m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			const ilog::IMessageContainer* containerPtr = supplierPtr->GetWorkMessages(m_containerType);

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
		iinsp::ISupplier* supplierPtr = m_parentPtr->m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			ilog::IMessageContainer* containerPtr = const_cast<ilog::IMessageContainer*>(supplierPtr->GetWorkMessages(m_containerType));

			if (containerPtr != NULL){
				retVal = containerPtr->Serialize(archive) && retVal;
			}
		}
	}

	return retVal;
}


// public methods of embedded class Parameters

CInspectionTaskComp::Parameters::Parameters()
:	imod::CModelUpdateBridge(this),
	m_parentPtr(NULL)
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
					const iinsp::ISupplier* subtaskPtr = m_parentPtr->m_subtasksCompPtr[i];
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
				const iinsp::ISupplier* subtaskPtr = m_parentPtr->m_subtasksCompPtr[i];
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
				const iinsp::ISupplier* subtaskPtr = m_parentPtr->m_subtasksCompPtr[i];
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
				iinsp::ISupplier* subtaskPtr = m_parentPtr->m_subtasksCompPtr[i];
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


// reimplemented (istd::IChangeable)

bool CInspectionTaskComp::Parameters::ResetData(CompatibilityMode mode)
{
	Ids parameterIds = GetParamIds();

	bool retVal = true;

	for (Ids::Iterator iter = parameterIds.begin(); iter != parameterIds.end(); ++iter){
		iser::ISerializable* paramPtr = GetEditableParameter(*iter);
		Q_ASSERT(paramPtr != NULL);

		retVal = paramPtr->ResetData(mode) && retVal;
	}

	return retVal;
}


// public methods of embedded class TaskStatusObserver

CInspectionTaskComp::TaskStatusObserver::TaskStatusObserver(CInspectionTaskComp* parentPtr)
	:m_parentPtr(parentPtr)
{
}


// reimplemented (imod::IObserver)

void CInspectionTaskComp::TaskStatusObserver::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(m_parentPtr != NULL);

	m_parentPtr->m_isStatusKnown = false;

	int supplierState = WS_INVALID;
	int inspectionsCount = m_parentPtr->m_subtasksCompPtr.GetCount();
	for (int i = 0; i < inspectionsCount; ++i){
		const iinsp::ISupplier* supplierPtr = m_parentPtr->m_subtasksCompPtr[i];
		if (supplierPtr != NULL){
			int workStatus = supplierPtr->GetWorkStatus();
			if (workStatus > supplierState){
				supplierState = workStatus;
			}
		}
	}

	m_parentPtr->m_workStatus.SetSupplierState(supplierState);

	BaseClass::AfterUpdate(modelPtr, changeSet);
}


// public methods of embedded class Status

CInspectionTaskComp::Status::Status()
	:m_state(ISupplier::WS_INVALID)
{
}


int CInspectionTaskComp::Status::GetSupplierState() const
{
	return m_state;
}


void CInspectionTaskComp::Status::SetSupplierState(int state)
{
	if (m_state != state){
		istd::CChangeNotifier changeNotifier(this);

		m_state = state;
	}
}


} // namespace iinsp


