#include "icam/CSelectionParamCameraComp.h"


// ACF includes
#include "iprm/TParamsPtr.h"


namespace icam
{


// reimplemented (iipr::IBitmapAcquisition)

istd::CIndex2d CSelectionParamCameraComp::GetBitmapSize(const iprm::IParamsSet* paramsPtr) const
{
	ParamSetPtr extParamsPtr(CreateParamsSet(paramsPtr));

	return BaseClass::GetBitmapSize(extParamsPtr.GetPtr());
}


// reimplemented (iproc::IProcessor)

int CSelectionParamCameraComp::GetProcessorState(const iprm::IParamsSet* paramsPtr) const
{
	ParamSetPtr extParamsPtr(CreateParamsSet(paramsPtr));

	return BaseClass::GetProcessorState(extParamsPtr.GetPtr());
}


bool CSelectionParamCameraComp::AreParamsAccepted(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			const istd::IChangeable* outputPtr) const
{
	ParamSetPtr extParamsPtr(CreateParamsSet(paramsPtr));

	return BaseClass::AreParamsAccepted(
				extParamsPtr.GetPtr(),
				inputPtr,
				outputPtr);
}


int CSelectionParamCameraComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* progressManagerPtr)
{
	ParamSetPtr extParamsPtr(CreateParamsSet(paramsPtr));

	return BaseClass::DoProcessing(
				extParamsPtr.GetPtr(),
				inputPtr,
				outputPtr,
				progressManagerPtr);
}


int CSelectionParamCameraComp::BeginTask(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* progressManagerPtr)
{
	ParamSetPtr extParamsPtr(CreateParamsSet(paramsPtr));

	int retVal = BaseClass::BeginTask(
				extParamsPtr.GetPtr(),
				inputPtr,
				outputPtr,
				progressManagerPtr);

	if (retVal >= 0){
		m_paramsMap[retVal].TakeOver(extParamsPtr);
	}

	return retVal;
}


int CSelectionParamCameraComp::WaitTaskFinished(
				int taskId,
				double timeoutTime,
				bool killOnTimeout)
{
	int retVal = BaseClass::WaitTaskFinished(
				taskId,
				timeoutTime,
				killOnTimeout);

	if (retVal != TS_WAIT){
		if (taskId >= 0){
			m_paramsMap.remove(retVal);
		}
		else{
			m_paramsMap.clear();
		}
	}

	return retVal;
}


void CSelectionParamCameraComp::CancelTask(int taskId)
{
	BaseClass::WaitTaskFinished(taskId);

	if (taskId >= 0){
		m_paramsMap.remove(taskId);
	}
	else{
		m_paramsMap.clear();
	}
}


void CSelectionParamCameraComp::InitProcessor(const iprm::IParamsSet* paramsPtr)
{
	ParamSetPtr extParamsPtr(CreateParamsSet(paramsPtr));

	return BaseClass::InitProcessor(extParamsPtr.GetPtr());
}


// protected methods

const iprm::IParamsSet* CSelectionParamCameraComp::CreateParamsSet(const iprm::IParamsSet* paramsPtr) const
{
	const iprm::IParamsSet* selectedParamsPtr = NULL;

	if (m_paramsManagerCompPtr.IsValid()){
		int selectedIndex = -1;

		iprm::TParamsPtr<iprm::ISelectionParam> selectionPtr(paramsPtr, *m_selectionIdAttrPtr);
		if (selectionPtr.IsValid()){
			selectedIndex = selectionPtr->GetSelectedOptionIndex();
		}
		else{
			selectedIndex = *m_defaultIndexAttrPtr;
		}

		if ((selectedIndex >= 0) && selectedIndex < m_paramsManagerCompPtr->GetParamsSetsCount()){
			selectedParamsPtr = m_paramsManagerCompPtr->GetParamsSet(selectedIndex);
		}
	}

	return new JoinParamsSet(paramsPtr, selectedParamsPtr);
}


// public methods of embedded class JoinParamsSet

CSelectionParamCameraComp::JoinParamsSet::JoinParamsSet(const iprm::IParamsSet* origParamsPtr, const iprm::IParamsSet* selectedParamsPtr)
:	m_origParamsPtr(origParamsPtr),
	m_selectedParamsPtr(selectedParamsPtr)
{
}


// reimplemented (iprm::IParamsSet)

iprm::IParamsSet::Ids CSelectionParamCameraComp::JoinParamsSet::GetParamIds(bool editableOnly) const
{
	iprm::IParamsSet::Ids retVal;

	if (!editableOnly){
		if (m_origParamsPtr != NULL){
			retVal += m_origParamsPtr->GetParamIds(false);
		}

		if (m_selectedParamsPtr != NULL){
			retVal += m_selectedParamsPtr->GetParamIds(false);
		}
	}

	return retVal;
}


const iser::ISerializable* CSelectionParamCameraComp::JoinParamsSet::GetParameter(const QByteArray& id) const
{
	const iser::ISerializable* retVal = NULL;

	if (m_origParamsPtr != NULL){
		retVal = m_origParamsPtr->GetParameter(id);
	}

	if ((retVal == NULL) && (m_selectedParamsPtr != NULL)){
		retVal = m_selectedParamsPtr->GetParameter(id);
	}

	return retVal;
}


iser::ISerializable* CSelectionParamCameraComp::JoinParamsSet::GetEditableParameter(const QByteArray& /*id*/)
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CSelectionParamCameraComp::JoinParamsSet::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


} // namespace icam


