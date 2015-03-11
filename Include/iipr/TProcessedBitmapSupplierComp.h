#pragma once


// ACF-Solutions includes
#include "iipr/CProcessedBitmapSupplierComp.h"


namespace iipr
{


/**
	Generic implementation of the processed bitmap supplier.
	Bitmap instance will be factorized using template argument
*/
template<typename Bitmap>
class TProcessedBitmapSupplierComp: public CProcessedBitmapSupplierBase
{
public:
	typedef Bitmap BitmapType;

	typedef CProcessedBitmapSupplierBase BaseClass;

	I_BEGIN_COMPONENT(TProcessedBitmapSupplierComp);
	I_END_COMPONENT;

protected:
	// reimplemented (CProcessedBitmapSupplierBase)
	virtual iimg::IBitmap* CreateBitmap() const;
};


// reimplemented (CProcessedBitmapSupplierBase)

template<typename Bitmap>
iimg::IBitmap* TProcessedBitmapSupplierComp<Bitmap>::CreateBitmap() const
{
	return new BitmapType;
}


} // namespace iipr


