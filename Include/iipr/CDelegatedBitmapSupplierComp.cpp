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

const i2d::ITransformation2d* CDelegatedBitmapSupplierComp::GetLogicalTransform() const
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
			result.first = m_calibrationProviderCompPtr->GetLogicalTransform();
		}

		if (result.second != NULL){
			return ISupplier::WS_OK;
		}
		else{
			return ISupplier::WS_ERROR;
		}
	}

	if (m_bitmapCompPtr.IsValid()){
		result.first = m_calibrationCompPtr.GetPtr();
		result.second = m_bitmapCompPtr.GetPtr();

		return ISupplier::WS_OK;
	}

	return ISupplier::WS_CRITICAL;
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


