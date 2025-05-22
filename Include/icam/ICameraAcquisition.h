/********************************************************************************
**
**	Copyright (C) 2007-2021 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF-Solutions Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org or write info@imagingtools.de for further
** 	information about the ACF.
**
********************************************************************************/


#ifndef icam_ICameraAcquisition_included
#define icam_ICameraAcquisition_included


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


#endif // !icam_ICameraAcquisition_included


