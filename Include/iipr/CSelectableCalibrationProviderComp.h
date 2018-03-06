#ifndef iipr_CSelectableCalibrationProviderComp_included
#define iipr_CSelectableCalibrationProviderComp_included


// ACF includes
#include <ilog/TLoggerCompWrap.h>
#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>
#include <i2d/IMultiCalibrationProvider.h>
#include <iprm/ISelectionParam.h>


namespace iipr
{


/**
	Single 2D-calibration supplier implemented as a selector from a multi calibration provider.
*/
class CSelectableCalibrationProviderComp:
			public ilog::CLoggerComponentBase,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSelectableCalibrationProviderComp);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_multiCalibrationProviderCompPtr, "MultiCalibrationProvider", "Provider of the calibration list", true, "MultiCalibrationProvider");
		I_ASSIGN(m_calibrationSelectionCompPtr, "CalibrationSelection", "Index of the calibration in the list", true, "CalibrationSelection");
		I_ASSIGN(m_defaultCalibrationProviderCompPtr, "DefaultCalibrationProvider", "Provider of default calibration. E.g. in case of non-selected or non-available calibration", false, "DefaultCalibrationProvider");
	I_END_COMPONENT;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

private:
	I_REF(i2d::IMultiCalibrationProvider, m_multiCalibrationProviderCompPtr);
	I_REF(iprm::ISelectionParam, m_calibrationSelectionCompPtr);
	I_REF(i2d::ICalibrationProvider, m_defaultCalibrationProviderCompPtr);
};


} // namespace iipr


#endif // !iipr_CSelectableCalibrationProviderComp_included


