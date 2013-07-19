#ifndef icam_CSnapBitmapSupplierComp_included
#define icam_CSnapBitmapSupplierComp_included


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include "istd/TDelPtr.h"
#include "i2d/ICalibration2d.h"
#include "i2d/ICalibrationProvider.h"
#include "iimg/IBitmap.h"
#include "iproc/TSupplierCompWrap.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"
#include "iimg/IBitmapProvider.h"
#include "icam/ISnapControl.h"
#include <imeas/INumericConstraints.h>
#include <imeas/CSimpleNumericValue.h>


namespace icam
{


/**
	Implementation of bitmap supplier based on image acquisition.
 */
class CSnapBitmapSupplierComp:
			public iproc::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>, istd::TDelPtr<iimg::IBitmap> > >,
			virtual public iimg::IBitmapProvider,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iproc::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>,  istd::TDelPtr<iimg::IBitmap> > > BaseClass;

	I_BEGIN_COMPONENT(CSnapBitmapSupplierComp);
		I_REGISTER_INTERFACE(iimg::IBitmapProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquisition object for image snap", true, "BitmapAcquisition");
		I_ASSIGN(m_calibrationCompPtr, "Calibration", "Optional calibration object", false, "Calibration");
		I_ASSIGN(m_scaleParamIdAttrPtr, "ScaleParamId", "ID ´of resolution parameter in parameter set (type imeas::INumericValue, unit: pixel/mm)", false, "ScaleParamId");
		I_ASSIGN(m_defaultScaleValueCompPtr, "DefaultScaleParam", "Default resolution in pixel/mm", false, "DefaultScaleParam");
		I_ASSIGN(m_calibratedUnitInfoCompPtr, "CalibratedUnit", "Describe unit of result calibration", false, "CalibratedUnit");
		I_ASSIGN(m_snapControlCompPtr, "SnapController", "Provides snap control options (i.e. snapping in another direction (backwards) etc)", false, "SnapController");
		I_REGISTER_SUBELEMENT(ScaleConstraints);		
	I_END_COMPONENT;

	// reimplemented (iimg::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual bool InitializeWork();
	virtual int ProduceObject(ProductType& result) const;

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(IBitmapAcquisition, m_bitmapAcquisitionCompPtr);
	I_REF(i2d::ICalibration2d, m_calibrationCompPtr);
	I_ATTR(QByteArray, m_scaleParamIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultScaleValueCompPtr);
	I_REF(imath::IUnitInfo, m_calibratedUnitInfoCompPtr);

	I_REF(icam::ISnapControl, m_snapControlCompPtr);
};


} // namespace icam


#endif // !icam_CSnapBitmapSupplierComp_included


