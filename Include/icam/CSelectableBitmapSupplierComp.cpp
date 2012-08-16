#include "icam/CSelectableBitmapSupplierComp.h"


// ACF includes
#include "i2d/CAffineTransformation2d.h"


namespace icam
{


// reimplemented (iipr::IBitmapProvider)

const iimg::IBitmap* CSelectableBitmapSupplierComp::GetBitmap() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second.GetPtr();
	}
	else{
		return NULL;
	}
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ITransformation2d* CSelectableBitmapSupplierComp::GetCalibration() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->first.GetPtr();
	}
	else{
		return NULL;
	}
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CSelectableBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_bitmapCompFact.IsValid()){
		return WS_CRITICAL;
	}

	result.first.Reset();
	result.second.Reset();

	if (!result.second.IsValid()){
		result.second.SetPtr(m_bitmapCompFact.CreateInstance());
		if (!result.second.IsValid()){
			return WS_CRITICAL;
		}
	}

	if (result.second.IsValid() && m_multiBitmapProviderCompPtr.IsValid() && m_bitmapSelectionCompPtr.IsValid()){
		int selectedIndex = m_bitmapSelectionCompPtr->GetSelectedOptionIndex();
		if (selectedIndex < 0){
			SendVerboseMessage("Bitmap selection index invalid");

			return WS_ERROR;
		}

		const iimg::IBitmap* outputBitmapPtr = m_multiBitmapProviderCompPtr->GetBitmap(selectedIndex);
		if (outputBitmapPtr != NULL){
			if (result.second->CopyFrom(*outputBitmapPtr)){
				const i2d::ITransformation2d* calbirationPtr = m_multiBitmapProviderCompPtr->GetLogTransform(selectedIndex);
				if (calbirationPtr != NULL){
					i2d::CAffineTransformation2d* defaultTranformPtr = new i2d::CAffineTransformation2d();
					defaultTranformPtr->Reset();

					result.first.SetPtr(defaultTranformPtr);

					result.first->CreateCombinedTransformation(*calbirationPtr);
				}
	
				return WS_OK;
			}
		}
		else{
			return WS_ERROR;
		}
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CSelectableBitmapSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapSelectionModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapSelectionModelCompPtr.GetPtr());
	}

	if (m_multiBitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_multiBitmapProviderModelCompPtr.GetPtr());
	}
}


void CSelectableBitmapSupplierComp::OnComponentDestroyed()
{
	if (m_bitmapSelectionModelCompPtr.IsValid()){
		UnregisterSupplierInput(m_bitmapSelectionModelCompPtr.GetPtr());
	}

	if (m_multiBitmapProviderModelCompPtr.IsValid()){
		UnregisterSupplierInput(m_multiBitmapProviderModelCompPtr.GetPtr());
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace icam

