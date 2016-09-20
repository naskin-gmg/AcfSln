#ifndef iipr_CPerspCalibFinder_included
#define iipr_CPerspCalibFinder_included


// ACF includes
#include <istd/IPolymorphic.h>
#include <i2d/CPerspectiveCalibration2d.h>
#include <i2d/CVector2d.h>


namespace iipr
{


/**
	Helper class allowing to find perspective calibration using list of nominal and found positions.
*/
class CPerspCalibFinder: public istd::IPolymorphic
{
public:
	virtual bool FindPerspCalib(
				const i2d::CVector2d* nominalPositionsPtr,
				const i2d::CVector2d* foundPositionsPtr,
				int positionsCount,
				i2d::CPerspectiveCalibration2d& results,
				bool allowPerspective = true,
				bool allowRotation = true,
				bool allowScale = true,
				bool allowAnisotropic = true,
				bool allowTranslation = true) const;
};


} // namespace iipr


#endif // !iipr_CPerspCalibFinder_included


