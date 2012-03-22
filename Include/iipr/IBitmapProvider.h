#ifndef iipr_IBitmapProvider_included
#define iipr_IBitmapProvider_included


// ACF includes
#include "istd/IChangeable.h"
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
};


} // namespace iipr


#endif // !iipr_IBitmapProvider_included


