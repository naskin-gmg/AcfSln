#pragma once


// ACF-Solutions includes
#include "icam/CSnapBitmapSupplierCompBase.h"


namespace icam
{


/**
	Generic implementation of the snap bitmap supplier.
	Bitmap instance will be factorized using template argument
*/
template<typename Bitmap>
class TSnapBitmapSupplierComp: public CSnapBitmapSupplierCompBase
{
public:
	typedef Bitmap BitmapType;

	typedef CSnapBitmapSupplierCompBase BaseClass;

	I_BEGIN_COMPONENT(TSnapBitmapSupplierComp);
	I_END_COMPONENT;

protected:
	// reimplemented (CSnapBitmapSupplierCompBase)
	virtual iimg::IBitmap* CreateBitmap() const;
};


// reimplemented (CSnapBitmapSupplierCompBase)

template<typename Bitmap>
iimg::IBitmap* TSnapBitmapSupplierComp<Bitmap>::CreateBitmap() const
{
	return new BitmapType;
}


} // namespace icam


