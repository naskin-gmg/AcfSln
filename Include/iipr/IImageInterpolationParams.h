#ifndef iipr_IImageInterpolationParams_included
#define iipr_IImageInterpolationParams_included


// ACF includes
#include "iser/ISerializable.h"


namespace iipr
{


/**
	Common interface for interpolation parameters
*/
class IImageInterpolationParams: virtual public iser::ISerializable
{
public:

	/**
		Interpolation mode
	*/
	enum InterpolationMode
	{
		IM_NO_INTERPOLATION,
		IM_BILINEAR,
		IM_BICUBIC
	};

	/**
		Get used interpolation mode.
		\sa InterpolationMode
	*/
	virtual int GetInterpolationMode() const = 0;

	/**
		Set interpolation mode.
		\sa InterpolationMode
	*/
	virtual void SetInterpolationMode(int interpolationMode) = 0;
};


} // namespace iipr


#endif // !iipr_IImageInterpolationParams_included


