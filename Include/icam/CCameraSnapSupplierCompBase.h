#pragma once


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include <istd/TDelPtr.h>
#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>
#include <iimg/IBitmap.h>
#include <iinsp/TSupplierCompWrap.h>

// ACF-Solutions includes
#include <icam/IBitmapAcquisition.h>
#include <icam/CSnapBitmapSupplierCompBase.h>


namespace icam
{


/**
	Basic implementation of bitmap supplier based on the camera snap.
*/
class CCameraSnapSupplierCompBase: public CSnapBitmapSupplierCompBase
{
public:
	typedef CSnapBitmapSupplierCompBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CCameraSnapSupplierCompBase);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquisition object for image snap", true, "BitmapAcquisition");
	I_END_COMPONENT;

protected:
	// reimplemented (CSnapBitmapSupplierCompBase)
	virtual int DoSnap(const iprm::IParamsSet* snapParamsPtr, iimg::IBitmap& snapBitmap) const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual bool InitializeWork();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(IBitmapAcquisition, m_bitmapAcquisitionCompPtr);
};


} // namespace icam


