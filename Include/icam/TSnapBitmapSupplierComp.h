// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF-Solutions includes
#include <icam/CCameraSnapSupplierCompBase.h>


namespace icam
{


/**
	Generic implementation of the snap bitmap supplier.
	Bitmap instance will be factorized using template argument
*/
template<typename Bitmap>
class TSnapBitmapSupplierComp: public CCameraSnapSupplierCompBase
{
public:
	typedef Bitmap BitmapType;
	typedef CCameraSnapSupplierCompBase BaseClass;

	I_BEGIN_COMPONENT(TSnapBitmapSupplierComp);
	I_END_COMPONENT;

protected:
	// reimplemented (CSnapBitmapSupplierCompBase)
	virtual iimg::IBitmapUniquePtr CreateBitmap() const;
};


// reimplemented (CSnapBitmapSupplierCompBase)

template<typename Bitmap>
iimg::IBitmapUniquePtr TSnapBitmapSupplierComp<Bitmap>::CreateBitmap() const
{
	return new BitmapType;
}


} // namespace icam


