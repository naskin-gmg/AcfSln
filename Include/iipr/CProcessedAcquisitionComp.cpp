#include "iipr/CProcessedAcquisitionComp.h"

#include "istd/CChangeNotifier.h"

#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d CProcessedAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* paramsPtr) const
{
	if (m_slaveAcquisitionCompPtr.IsValid()){
		return m_slaveAcquisitionCompPtr->GetBitmapSize(paramsPtr);
	}

	return istd::CIndex2d();
}


// reimplemented iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>

int CProcessedAcquisitionComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if ((outputBitmapPtr == NULL) || !m_slaveAcquisitionCompPtr.IsValid()){
		return TS_INVALID;
	}

	istd::CChangeNotifier outputNotifier(outputBitmapPtr);

	int retVal = TS_INVALID;

	retVal = m_slaveAcquisitionCompPtr->DoProcessing(paramsPtr, inputPtr, outputBitmapPtr);
	if (retVal == TS_OK){
		if (m_processorCompPtr.IsValid()){
			iimg::CGeneralBitmap bufferBitmap;
			retVal = m_processorCompPtr->DoProcessing(paramsPtr, outputBitmapPtr, &bufferBitmap);
			if (retVal == TS_OK){
				if (!outputBitmapPtr->CopyFrom(bufferBitmap)){
					retVal = TS_INVALID;
				}
			}
		}
	}

	return retVal;
}


} // namespace iipr

