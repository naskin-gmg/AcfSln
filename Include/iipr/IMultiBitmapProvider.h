#ifndef iipr_IMultiBitmapProvider_included
#define iipr_IMultiBitmapProvider_included


// ACF includes
#include "istd/IChangeable.h"
#include "i2d/ITransformation2d.h"
#include "iprm/ISelectionConstraints.h"
#include "iimg/IBitmap.h"


namespace iipr
{


/**
	Supplier allowing to access of multiple produced bitmaps.
*/
class IMultiBitmapProvider: virtual public istd::IChangeable
{
public:
	/**
		Get optional information about bitmaps in form of section contraints.
		It allows to get name and description of each bitmap channel.
	*/
	virtual const iprm::ISelectionConstraints* GetBitmapSelectionContraints() const = 0;

	/**
		Get number of produced bitmaps.
	*/
	virtual int GetBitmapsCount() const = 0;
	/**
		Get access to produced bitmap object.
		\return	pointer to bitmap instance if this bitmap is accessible, or NULL.
	*/
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const = 0;
};


} // namespace iipr


#endif // !iipr_IMultiBitmapProvider_included


