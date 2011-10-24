#include "iipr/CImageProcessorCompBase.h"


// ACF includes
#include "istd/CStaticServicesProvider.h"
#include "iimg/CGeneralBitmap.h"

#include "iipr/CImageProcessorCompBase.h"


namespace iipr
{


// public methods

CImageProcessorCompBase::CImageProcessorCompBase()
{
}


// reimplemented (iproc::IProcessor)

int CImageProcessorCompBase::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);

	if ((inputBitmapPtr == NULL) || (outputBitmapPtr == NULL)){
		return TS_INVALID;
	}

	// do image processing:
	if (!ProcessImage(paramsPtr, *inputBitmapPtr, *outputBitmapPtr)){
		return TS_INVALID;
	}

	return TS_OK;
}


} // namespace iipr


