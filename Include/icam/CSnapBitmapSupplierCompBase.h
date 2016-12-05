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
#include <iimg/IBitmapProvider.h>
#include <imeas/INumericConstraints.h>
#include <imeas/CSimpleNumericValue.h>


namespace icam
{


/**
	Basic implementation of bitmap supplier based on image acquisition.
 */
class CSnapBitmapSupplierCompBase:
			public iinsp::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>, istd::TDelPtr<iimg::IBitmap> > >,
			virtual public iimg::IBitmapProvider,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iinsp::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>,  istd::TDelPtr<iimg::IBitmap> > > BaseClass;

	I_BEGIN_BASE_COMPONENT(CSnapBitmapSupplierCompBase);
		I_REGISTER_INTERFACE(iimg::IBitmapProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_REGISTER_SUBELEMENT(ScaleConstraints);		
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquisition object for image snap", true, "BitmapAcquisition");
		I_ASSIGN(m_calibrationIdAttrPtr, "CalibrationParamId", "ID of calibration in parameter set", false, "Calibration");
		I_ASSIGN(m_defaultCalibrationCompPtr, "Calibration", "Optional calibration object", false, "Calibration");
	I_END_COMPONENT;

	// reimplemented (iimg::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	// abstract methods
	virtual iimg::IBitmap* CreateBitmap() const = 0;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual bool InitializeWork();
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(IBitmapAcquisition, m_bitmapAcquisitionCompPtr);
	I_ATTR(QByteArray, m_calibrationIdAttrPtr);
	I_REF(i2d::ICalibration2d, m_defaultCalibrationCompPtr);
};


} // namespace icam


