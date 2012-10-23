#include "iipr/CSelectableCalibrationSupplierComp.h"


// ACF includes
#include "i2d/ITransformation2d.h"


namespace iipr
{


// public methods

// reimplemented (i2d::ICalibrationProvider)

const i2d::ITransformation2d* CSelectableCalibrationSupplierComp::GetCalibration() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetPtr();
	}

	return NULL;
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CSelectableCalibrationSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_calibrationCompFact.IsValid()){
		return WS_CRITICAL;
	}


	if (!result.IsValid()){
		result.SetPtr(m_calibrationCompFact.CreateInstance());
		if (!result.IsValid()){
			return WS_CRITICAL;
		}
	}

	if (m_multiCalibrationProviderCompPtr.IsValid() && m_calibrationSelectionCompPtr.IsValid()){
		int selectedIndex = m_calibrationSelectionCompPtr->GetSelectedOptionIndex();
		if (selectedIndex < 0){
			SendVerboseMessage("Calibration selection index invalid");

			return WS_ERROR;
		}

		int calibrationsCount = m_multiCalibrationProviderCompPtr->GetCalibrationsCount();
		if (selectedIndex >= calibrationsCount){
			return WS_ERROR;		
		}

		const i2d::ITransformation2d* outputCalibrationPtr = m_multiCalibrationProviderCompPtr->GetCalibration(selectedIndex);
		if (outputCalibrationPtr != NULL){
			if (result->CopyFrom(*outputCalibrationPtr)){
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

void CSelectableCalibrationSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_calibrationSelectionModelCompPtr.IsValid()){
		RegisterSupplierInput(m_calibrationSelectionModelCompPtr.GetPtr());
	}

	if (m_multiCalibrationProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_multiCalibrationProviderModelCompPtr.GetPtr(), m_multiCalibrationSupplierCompPtr.GetPtr());
	}
}


void CSelectableCalibrationSupplierComp::OnComponentDestroyed()
{
	if (m_calibrationSelectionModelCompPtr.IsValid()){
		UnregisterSupplierInput(m_calibrationSelectionModelCompPtr.GetPtr());
	}

	if (m_multiCalibrationProviderModelCompPtr.IsValid()){
		UnregisterSupplierInput(m_multiCalibrationProviderModelCompPtr.GetPtr());
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iipr

