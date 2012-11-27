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
#include "iipr/IBitmapProvider.h"


namespace icam
{


/**
	Implementation of bitmap supplier based on image acquisition.
*/
class CSnapBitmapSupplierComp:
			public iproc::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>, istd::TDelPtr<iimg::IBitmap> > >,
			virtual public iipr::IBitmapProvider,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iproc::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>,  istd::TDelPtr<iimg::IBitmap> > > BaseClass;

	I_BEGIN_COMPONENT(CSnapBitmapSupplierComp);
		I_REGISTER_INTERFACE(iipr::IBitmapProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquision object for image snap", true, "BitmapAcquisition");
		I_ASSIGN(m_calibrationCompPtr, "Calibration", "Optional calibration object", false, "Calibration");
	I_END_COMPONENT;

	// reimplemented (iipr::IBitmapProvider)
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
};


} // namespace icam


#endif // !icam_CSnapBitmapSupplierComp_included


