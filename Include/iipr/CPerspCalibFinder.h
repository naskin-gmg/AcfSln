#ifndef iipr_CPerspCalibFinder_included
#define iipr_CPerspCalibFinder_included


// ACF includes
#include <istd/IPolymorphic.h>
#include <i2d/CVector2d.h>

// ACF-Solutions includes
#include <icalib/CPerspectiveCalibration2d.h>


namespace iipr
{


/**
	Helper class allowing to find perspective calibration using list of nominal and found positions.
*/
class CPerspCalibFinder: public istd::IPolymorphic
{
public:
	/**
		It allows to find perspective calibration factors.
	*/
	virtual bool FindPerspCalib(
				const i2d::CVector2d* nominalPositionsPtr,
				const i2d::CVector2d* foundPositionsPtr,
				int positionsCount,
				icalib::CPerspectiveCalibration2d& result,
				bool allowPerspective = true,
				bool allowRotation = true,
				bool allowScale = true,
				bool allowAnisotropic = true,
				bool allowTranslation = true) const;
	/**
		It allows to find perspective calibration factors with lens correction factor.
	*/
	virtual bool FindPerspCalibWithCorrection(
				const i2d::CVector2d* nominalPositionsPtr,
				const i2d::CVector2d* foundPositionsPtr,
				int positionsCount,
				const i2d::CVector2d& opticalCenter,
				icalib::CPerspectiveCalibration2d& result,
				double& lensCorrFactor,
				bool allowPerspective = true,
				bool allowRotation = true,
				bool allowScale = true,
				bool allowAnisotropic = true,
				bool allowTranslation = true) const;
};


} // namespace iipr


#endif // !iipr_CPerspCalibFinder_included


