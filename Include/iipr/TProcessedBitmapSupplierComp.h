// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF-Solutions includes
#include <iipr/CProcessedBitmapSupplierComp.h>


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
	virtual iimg::IBitmapUniquePtr CreateBitmap() const;
};


// reimplemented (CProcessedBitmapSupplierBase)

template<typename Bitmap>
iimg::IBitmapUniquePtr TProcessedBitmapSupplierComp<Bitmap>::CreateBitmap() const
{
	return new BitmapType;
}


} // namespace iipr


