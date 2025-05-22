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


