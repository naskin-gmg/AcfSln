#ifndef icalib_CStaticCalibrationProviderComp_included
#define icalib_CStaticCalibrationProviderComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <imod/CModelUpdateBridge.h>
#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>


namespace icalib
{


/**
	Implementation of a static calibration provider.
	This component uses a staticaly defined calibration object (set via component reference) to provide the calibration instance.
	If the undelaying calibration object will be changed, this object will also fire a change notification event.
*/
class CStaticCalibrationProviderComp:
			public icomp::CComponentBase,
			public i2d::ICalibrationProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CStaticCalibrationProviderComp);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_calibrationCompPtr, "Calibration", "Static calibration object", true, "Calibration");
		I_ASSIGN_TO(m_calibrationModelCompPtr, m_calibrationCompPtr, false);
	I_END_COMPONENT;

	CStaticCalibrationProviderComp();

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(i2d::ICalibration2d, m_calibrationCompPtr);
	I_REF(imod::IModel, m_calibrationModelCompPtr);

	imod::CModelUpdateBridge m_updateBridge;
};


} // namespace icalib


#endif // !icalib_CStaticCalibrationProviderComp_included


