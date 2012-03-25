#include "iipr/CProcessedBitmapSupplierComp.h"


namespace iipr
{


// reimplemented (iipr::IBitmapProvider)

const iimg::IBitmap* CProcessedBitmapSupplierComp::GetBitmap() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second.GetPtr();
	}

	return NULL;
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ITransformation2d* CProcessedBitmapSupplierComp::GetCalibration() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->first;
	}
	else{
		return NULL;
	}
}


//protected methods

bool CProcessedBitmapSupplierComp::EnsureBitmapCreated(ProductType& result) const
{
	if (!m_bitmapCompFact.IsValid()){
		return false;
	}

	if (!result.second.IsValid()){
		result.second.SetPtr(m_bitmapCompFact.CreateInstance());
	}

	return result.second.IsValid();
}


// reimplemented (iproc::TSupplierCompWrap)

int CProcessedBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_bitmapProviderCompPtr.IsValid() || !m_imageProcessorCompPtr.IsValid()){
		return WS_CRITICAL;
	}

	if (!EnsureBitmapCreated(result)){
		return WS_CRITICAL;
	}
	I_ASSERT(result.second.IsValid());

	result.first = NULL;

	const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
	if (bitmapPtr == NULL){
		return WS_ERROR;
	}

	if (m_workingLogTransformCompPtr.IsValid() && m_calibrationProviderCompPtr.IsValid()){
		const i2d::ITransformation2d* transformationPtr = m_calibrationProviderCompPtr->GetCalibration();
		if (transformationPtr != NULL){
			if (m_imageProcessorCompPtr->DoProcessing(GetModelParametersSet(), transformationPtr, m_workingLogTransformCompPtr.GetPtr()) == iproc::IProcessor::TS_OK){
				result.first = m_workingLogTransformCompPtr.GetPtr();
			}
			else{
				return WS_ERROR;
			}
		}
	}
	else{
		result.first = m_defaultLogTransformCompPtr.GetPtr();
	}

	int status = m_imageProcessorCompPtr->DoProcessing(GetModelParametersSet(), bitmapPtr, result.second.GetPtr());
	switch (status){
	case iproc::IProcessor::TS_OK:
		return WS_OK;

	case iproc::IProcessor::TS_CANCELED:
		return WS_CANCELED;

	default:
		return WS_ERROR;
	}
}


// reimplemented (icomp::CComponentBase)

void CProcessedBitmapSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr());
	}
}


} // namespace iipr


