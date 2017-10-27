#include <iblob/CBlobFilterParamsComp.h>


namespace iblob
{


// public methods

// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CBlobFilterParamsComp::GetCalibration() const
{
	return m_calibrationCompPtr.GetPtr();
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CBlobFilterParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetSupportedProperties(m_supportedPropertiesCompPtr.GetPtr());
}


} // namespace iblob


