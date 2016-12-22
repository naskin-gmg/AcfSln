#include <icalib/CStaticCalibrationProviderComp.h>


namespace icalib
{


CStaticCalibrationProviderComp::CStaticCalibrationProviderComp()
:	m_updateBridge(this)
{
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CStaticCalibrationProviderComp::GetCalibration() const
{
	if (m_calibrationCompPtr.IsValid()){
		return m_calibrationCompPtr.GetPtr();
	}

	return NULL;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CStaticCalibrationProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_calibrationModelCompPtr.IsValid()){
		m_calibrationModelCompPtr->AttachObserver(&m_updateBridge);
	}
}


void CStaticCalibrationProviderComp::OnComponentDestroyed()
{
	m_updateBridge.EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace icalib


