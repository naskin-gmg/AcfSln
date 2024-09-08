#include <iipr/CProcessedAcquisitionComp.h>
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>
#include <iimg/CGeneralBitmap.h>


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
	if (!m_bitmapFactCompPtr.IsValid()){
		SendErrorMessage(0, "Bitmap factory was not set");

		return TS_INVALID;
	}

	if (outputPtr == NULL){
		return TS_OK;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if ((outputBitmapPtr == NULL) || !m_slaveAcquisitionCompPtr.IsValid()){
		return TS_INVALID;
	}

	istd::CChangeNotifier outputNotifier(outputBitmapPtr);

	int retVal = m_slaveAcquisitionCompPtr->DoProcessing(paramsPtr, inputPtr, outputBitmapPtr);
	if (retVal == TS_OK){
		if (m_processorCompPtr.IsValid()){
			istd::TDelPtr<iimg::IBitmap> tempBitmapPtr(m_bitmapFactCompPtr.CreateInstance());
			if (!tempBitmapPtr.IsValid()){
				SendErrorMessage(0, "Temporary bitmap could not be created");

				return TS_INVALID;
			}

			retVal = m_processorCompPtr->DoProcessing(paramsPtr, outputBitmapPtr, tempBitmapPtr.GetPtr());
			if (retVal == TS_OK){
				if (!outputBitmapPtr->CopyFrom(*tempBitmapPtr)){
					retVal = TS_INVALID;
				}
			}
		}
	}

	return retVal;
}


} // namespace iipr

