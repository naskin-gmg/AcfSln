// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iprm/IParamsSet.h>
#include <iimg/IBitmap.h>


namespace icam
{


class ICameraAcquisition
{
public:
	virtual bool StartCamera() const = 0;
	virtual bool StopCamera() const = 0;
	virtual bool GetLastImage(iimg::IBitmap& snapBitmap) const = 0;
};


} // namespace icam


