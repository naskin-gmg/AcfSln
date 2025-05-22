#include <icam/CMultiSourceSnapBitmapSupplierComp.h>


// ACF includes
#include <iprm/ISelectionParam.h>
#include <iprm/IParamsManager.h>
#include <iprm/TParamsPtr.h>


namespace icam
{


// public methods

CMultiSourceSnapBitmapSupplierComp::CMultiSourceSnapBitmapSupplierComp()
{
	m_cameraList.SetParent(this);
}


// protected methods

// reimplemented (CSnapBitmapSupplierCompBase)

iimg::IBitmapUniquePtr CMultiSourceSnapBitmapSupplierComp::CreateBitmap() const
{
	return m_bitmapCompFact.CreateInstance();
}


int CMultiSourceSnapBitmapSupplierComp::DoSnap(const iprm::IParamsSet* snapParamsPtr, iimg::IBitmap& snapBitmap) const
{
	int selectedCameraIndex = -1;
	IBitmapAcquisition* cameraPtr = GetSelectedCamera(&selectedCameraIndex);
	if (cameraPtr == NULL){
		SendErrorMessage(0, "No camera was selected");

		return iproc::IProcessor::TS_INVALID;
	}

	iprm::TParamsPtr<iprm::IParamsManager> cameraParamsManagerPtr(snapParamsPtr, *m_cameraManagerParamIdAttrPtr);
	if (cameraParamsManagerPtr.IsValid()){
		const iprm::IParamsSet* cameraParamsPtr = NULL;
		if ((selectedCameraIndex >= 0) && (selectedCameraIndex < cameraParamsManagerPtr->GetParamsSetsCount())){
			cameraParamsPtr = cameraParamsManagerPtr->GetParamsSet(selectedCameraIndex);
		}

		return cameraPtr->DoProcessing(cameraParamsPtr, NULL, &snapBitmap);
	}

	return iproc::IProcessor::TS_INVALID;
}


// reimplemented (iinsp::TSupplierCompWrap)

bool CMultiSourceSnapBitmapSupplierComp::InitializeWork()
{
	IBitmapAcquisition* cameraPtr = GetSelectedCamera();
	if (cameraPtr != NULL){
		cameraPtr->InitProcessor(GetModelParametersSet());

		return true;
	}

	return false;
}


// protected methods

bool CMultiSourceSnapBitmapSupplierComp::SetSelectedCamera(int cameraIndex) const
{
	if (cameraIndex < 0 || cameraIndex >= m_cameraListCompPtr.GetCount()) {
		return false;
	}

	iprm::TParamsPtr<iprm::ISelectionParam> sourceSelectionParamPtr(GetModelParametersSet(), *m_sourceSelectionParamIdAttrPtr);
	if (sourceSelectionParamPtr.IsValid()){
		int sourceIndex = sourceSelectionParamPtr->GetSelectedOptionIndex();
		if (sourceIndex != cameraIndex) {
			iprm::IParamsSet* paramsPtr = GetModelParametersSet();
			if (paramsPtr != nullptr){
				iprm::TEditableParamsPtr<iprm::ISelectionParam> sourceCameraSelectorPtr(paramsPtr, *m_sourceSelectionParamIdAttrPtr);
				if (sourceCameraSelectorPtr.IsValid()){
					return sourceCameraSelectorPtr->SetSelectedOptionIndex(cameraIndex);
				}
			}
		}
		return true;
	}

	return false;
}


IBitmapAcquisition* CMultiSourceSnapBitmapSupplierComp::GetSelectedCamera(int* selectedIndexPtr) const
{
	if (selectedIndexPtr != NULL){
		*selectedIndexPtr = -1;
	}

	iprm::TParamsPtr<iprm::ISelectionParam> sourceSelectionParamPtr(GetModelParametersSet(), *m_sourceSelectionParamIdAttrPtr);
	if (sourceSelectionParamPtr.IsValid()){
		int sourceIndex = sourceSelectionParamPtr->GetSelectedOptionIndex();
		if ((sourceIndex >= 0) && (sourceIndex < m_cameraListCompPtr.GetCount())){
			if (selectedIndexPtr != NULL){
				*selectedIndexPtr = sourceIndex;
			}
			return m_cameraListCompPtr[sourceIndex];
		}
	}

	return NULL;
}


// public methods of the embedded class CameraList

CMultiSourceSnapBitmapSupplierComp::CameraList::CameraList()
	:m_parentPtr(NULL)
{
}


void CMultiSourceSnapBitmapSupplierComp::CameraList::SetParent(CMultiSourceSnapBitmapSupplierComp* parentPtr)
{
	m_parentPtr = parentPtr;
}


// reimplemented (iprm::IOptionsList)

int CMultiSourceSnapBitmapSupplierComp::CameraList::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CMultiSourceSnapBitmapSupplierComp::CameraList::GetOptionsCount() const
{
	Q_ASSERT(m_parentPtr != NULL);

	return m_parentPtr->m_cameraListCompPtr.GetCount();
}


QString CMultiSourceSnapBitmapSupplierComp::CameraList::GetOptionName(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);

	if (index < m_parentPtr->m_cameraNamesAttrPtr.GetCount()){
		return m_parentPtr->m_cameraNamesAttrPtr[index];
	}
	else{
		return "Unnamed";
	}
}


QString CMultiSourceSnapBitmapSupplierComp::CameraList::GetOptionDescription(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);
	Q_UNUSED(index);

	return QString();
}


QByteArray CMultiSourceSnapBitmapSupplierComp::CameraList::GetOptionId(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);

	if (index < m_parentPtr->m_cameraIdsAttrPtr.GetCount()){
		return m_parentPtr->m_cameraIdsAttrPtr[index];
	}
	else{
		return QString("Camera-%1").arg(index + 1).toUtf8();
	}
}


bool CMultiSourceSnapBitmapSupplierComp::CameraList::IsOptionEnabled(int index) const
{
	Q_UNUSED(index);

	return true;
}


} // namespace icam


