#include "icam/CSelectableBitmapSupplierComp.h"


// ACF includes
#include "i2d/CAffineTransformation2d.h"


namespace icam
{


// reimplemented (iimg::IBitmapProvider)

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

const i2d::ICalibration2d* CSelectableBitmapSupplierComp::GetCalibration() const
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

		int bitmapsCount = m_multiBitmapProviderCompPtr->GetBitmapsCount();
		if (selectedIndex >= bitmapsCount){
			return WS_ERROR;		
		}

		const iimg::IBitmap* outputBitmapPtr = m_multiBitmapProviderCompPtr->GetBitmap(selectedIndex);
		if (outputBitmapPtr != NULL){
			if (result.second->CopyFrom(*outputBitmapPtr)){
				const i2d::ICalibration2d* calibrationPtr = NULL;

				if (m_multiBitmapMultiCalibCompPtr.IsValid() && (selectedIndex < m_multiBitmapMultiCalibCompPtr->GetCalibrationsCount())){
					calibrationPtr = m_multiBitmapMultiCalibCompPtr->GetCalibration(selectedIndex);
				}

				if ((calibrationPtr == NULL) && m_multiBitmapCalibCompPtr.IsValid()){
					calibrationPtr = m_multiBitmapCalibCompPtr->GetCalibration();
				}

				if (calibrationPtr != NULL){
					result.first.SetCastedOrRemove(calibrationPtr->CloneMe());
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
		RegisterSupplierInput(m_multiBitmapProviderModelCompPtr.GetPtr(), m_multiBitmapSupplierCompPtr.GetPtr());
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

