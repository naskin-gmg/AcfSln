#include "iipr/CDelegatedBitmapSupplierComp.h"


namespace iipr
{


// reimplemented (iipr::IBitmapProvider)

const iimg::IBitmap* CDelegatedBitmapSupplierComp::GetBitmap() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second;
	}

	return NULL;
}

	
// reimplemented (i2d::ICalibrationProvider)

const i2d::ITransformation2d* CDelegatedBitmapSupplierComp::GetCalibration() const
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

// reimplemented (iproc::TSupplierCompWrap)

int CDelegatedBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	if (m_inputBitmapProviderCompPtr.IsValid()){
		result.first = NULL;
		result.second = m_inputBitmapProviderCompPtr->GetBitmap();

		if (m_calibrationProviderCompPtr.IsValid()){
			result.first = m_calibrationProviderCompPtr->GetCalibration();
		}

		if (result.second != NULL){
			return WS_OK;
		}
		else{
			return WS_ERROR;
		}
	}

	if (m_bitmapCompPtr.IsValid()){
		result.first = m_calibrationCompPtr.GetPtr();
		result.second = m_bitmapCompPtr.GetPtr();

		return WS_OK;
	}

	SendVerboseMessage("Either input bitmap supplier or bitmap object must be set!");

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CDelegatedBitmapSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_inputBitmapProviderCompPtr.IsValid()){
		if (m_inputBitmapProviderModelCompPtr.IsValid()){
			RegisterSupplierInput(m_inputBitmapProviderModelCompPtr.GetPtr());
		}
	}
	else{
		if (m_bitmapModelCompPtr.IsValid()){
			RegisterSupplierInput(m_bitmapModelCompPtr.GetPtr());
		}
		if (m_calibrationModelCompPtr.IsValid()){
			RegisterSupplierInput(m_calibrationModelCompPtr.GetPtr());
		}
	}
}


} // namespace iipr


