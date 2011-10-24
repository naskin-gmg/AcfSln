#ifndef iipr_IBitmapProvider_included
#define iipr_IBitmapProvider_included


// ACF includes
#include "istd/IChangeable.h"
#include "i2d/ITransformation2d.h"
#include "iimg/IBitmap.h"


namespace iipr
{


/**
	Bitmap supplier allowing to access of produced bitmap.
*/
class IBitmapProvider: virtual public istd::IChangeable
{
public:
	/**
		Get access to produced bitmap object.
		\return	pointer to bitmap instance if this bitmap is accessible, or NULL.
	*/
	virtual const iimg::IBitmap* GetBitmap() const = 0;

	/**
		Get access to transformation from bitmap to logical positions.
		Please note, not all implementations must support this information.
		\return	pointer to transformation if it is accessible, or NULL.
	*/
	virtual const i2d::ITransformation2d* GetLogTransform() const = 0;
};


} // namespace iipr


#endif // !iipr_IBitmapProvider_included


