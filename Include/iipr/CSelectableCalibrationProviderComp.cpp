#include "iipr/CSelectableCalibrationProviderComp.h"


namespace iipr
{


// public methods

// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CSelectableCalibrationProviderComp::GetCalibration() const
{
	if (m_multiCalibrationProviderCompPtr.IsValid() && m_calibrationSelectionCompPtr.IsValid()){
		int calibrationsCount = m_multiCalibrationProviderCompPtr->GetCalibrationsCount();

		int selectedIndex = m_calibrationSelectionCompPtr->GetSelectedOptionIndex();
		if ((selectedIndex >= 0) && (selectedIndex < calibrationsCount)){
			return m_multiCalibrationProviderCompPtr->GetCalibration(selectedIndex);
		}
	}

	return NULL;
}


} // namespace iipr


