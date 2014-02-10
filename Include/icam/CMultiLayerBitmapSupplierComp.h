#ifndef icam_CMultiLayerBitmapSupplierComp_included
#define icam_CMultiLayerBitmapSupplierComp_included


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include "istd/TDelPtr.h"
#include "i2d/ICalibration2d.h"
#include "i2d/ICalibrationProvider.h"
#include "iimg/IMultiLayerBitmap.h"
#include "iproc/TSupplierCompWrap.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"
#include "iimg/IMultiBitmapProvider.h"
#include <imeas/INumericConstraints.h>
#include <imeas/CSimpleNumericValue.h>


namespace icam
{


/**
	Implementation of bitmap supplier based on image acquisition.
*/
class CMultiLayerBitmapSupplierComp:
	public iproc::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>, istd::TDelPtr<iimg::IMultiLayerBitmap> > >,
	virtual public iimg::IMultiBitmapProvider,
	virtual public i2d::ICalibrationProvider
{
public:
	typedef iproc::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>, istd::TDelPtr<iimg::IMultiLayerBitmap> > > BaseClass;

	I_BEGIN_COMPONENT(CMultiLayerBitmapSupplierComp);
		I_REGISTER_INTERFACE(iimg::IMultiBitmapProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_REGISTER_SUBELEMENT(ScaleConstraints);		
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquisition object for image snap", true, "BitmapAcquisition");
		I_ASSIGN(m_calibrationCompPtr, "Calibration", "Optional calibration object", false, "Calibration");
		I_ASSIGN(m_scaleParamIdAttrPtr, "ScaleParamId", "ID of resolution parameter in parameter set (type imeas::INumericValue, unit: pixel/mm)", false, "ScaleParamId");
		I_ASSIGN(m_defaultScaleValueCompPtr, "DefaultScaleParam", "Default resolution in pixel/mm", false, "DefaultScaleParam");
		I_ASSIGN(m_calibratedUnitInfoCompPtr, "CalibratedUnit", "Describe unit of result calibration", false, "CalibratedUnit");
	I_END_COMPONENT;

	// reimplemented (iimg::IMultiBitmapProvider)
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;
	virtual const iprm::IOptionsList* GetBitmapListInfo() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual bool InitializeWork();
	virtual int ProduceObject(ProductType& result) const;

private:
	I_FACT(iimg::IMultiLayerBitmap, m_bitmapCompFact);

	I_REF(IBitmapAcquisition, m_bitmapAcquisitionCompPtr);
	I_REF(i2d::ICalibration2d, m_calibrationCompPtr);
	I_ATTR(QByteArray, m_scaleParamIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultScaleValueCompPtr);
	I_REF(imath::IUnitInfo, m_calibratedUnitInfoCompPtr);
};


} // namespace icam


#endif // !icam_CMultiLayerBitmapSupplierComp_included


