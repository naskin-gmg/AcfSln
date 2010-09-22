#include "iqtfpf/CFilePreviewGeneratorComp.h"


// ACF includes
#include "iprm/IFileNameParam.h"


namespace iqtfpf
{


// public methods

// reimplemented (iproc::IBitmapAcquisition)

istd::CIndex2d CFilePreviewGeneratorComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return istd::CIndex2d(*m_widthAttrPtr, *m_heightAttrPtr);
}


// reimplemented (iproc::IProcessor)

int CFilePreviewGeneratorComp::DoProcessing(
				const iprm::IParamsSet* /*paramsPtr*/,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* /*progressManagerPtr*/)
{
	if (!m_fileLoaderCompPtr.IsValid()){
		return TS_INVALID;
	}

	if (!m_fileDataCompPtr.IsValid()){
		return TS_INVALID;
	}

	if (!m_objectSnapCompPtr.IsValid()){
		return TS_INVALID;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	I_ASSERT(outputBitmapPtr != NULL);
	if (outputBitmapPtr == NULL){
		return TS_INVALID;
	}

	const iprm::IFileNameParam* fileNamePtr = dynamic_cast<const iprm::IFileNameParam*>(inputPtr);
	if (fileNamePtr == NULL){
		return TS_INVALID;
	}

	if (m_fileLoaderCompPtr->LoadFromFile(*m_fileDataCompPtr.GetPtr(), fileNamePtr->GetPath()) != iser::IFileLoader::StateOk){
		return TS_INVALID;
	}


	if (m_objectSnapCompPtr->GetSnap(*m_fileDataCompPtr.GetPtr(), *outputBitmapPtr, GetBitmapSize(NULL))){
		return TS_OK;
	}

	return TS_INVALID;
}


} // namespace iqtfpf


