// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icam/CSnapBitmapSupplierComp.h>


namespace icam
{


// protected methods

iimg::IBitmapUniquePtr CSnapBitmapSupplierComp::CreateBitmap() const
{
	if (m_bitmapCompFact.IsValid()){
		return m_bitmapCompFact.CreateInstance();
	}

	return NULL;
}


} // namespace icam


