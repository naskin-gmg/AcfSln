#ifndef icam_CMultiCameraBitmapSupplierComp_included
#define icam_CMultiCameraBitmapSupplierComp_included


// Qt includes
#include <QtCore/QMutex>

// ACF includes
#include "istd/TDelPtr.h"
#include "istd/TPointerVector.h"
#include "i2d/ITransformation2d.h"
#include "i2d/ICalibrationProvider.h"
#include "iimg/IBitmap.h"
#include "iprm/IParamsManager.h"
#include "iproc/TSupplierCompWrap.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"
#include "iipr/IMultiBitmapProvider.h"


namespace icam
{


/**
	Implementation of multi-bitmap camera supplier.
	This component works with a set of camera parameters and iterate this set during acquisition of the output bitmaps.
	The number of output bitmaps is equal the number of parameter sets in the parameter manager.
*/
class CMultiCameraBitmapSupplierComp:
			public iproc::TSupplierCompWrap< istd::TPointerVector<const iimg::IBitmap> >,
			virtual public iipr::IMultiBitmapProvider,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iproc::TSupplierCompWrap< istd::TPointerVector<const iimg::IBitmap> > BaseClass;

	I_BEGIN_COMPONENT(CMultiCameraBitmapSupplierComp);
		I_REGISTER_INTERFACE(iipr::IMultiBitmapProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquision object for image snap", true, "BitmapAcquisition");
		I_ASSIGN(m_logTransformCompPtr, "LogTransform", "Optional transformation of pixel to logical units", false, "LogTransform");
		I_ASSIGN(m_cameraParamsManagerCompPtr, "CameraParamsManager", "Manager of camera parameters", true, "CameraParamsManager");
	I_END_COMPONENT;

	// reimplemented (iipr::IMultiBitmapProvider)
	virtual const iprm::IOptionsList* GetBitmapSelectionContraints() const;
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual bool InitializeWork();
	virtual int ProduceObject(ProductType& result) const;

private:
	/**
		Factory for creation of the output bitmap object.
	*/
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	/**
		Image acquisition component, e.g. camera.
	*/
	I_REF(IBitmapAcquisition, m_bitmapAcquisitionCompPtr);

	/**
		Pixel to logical units transformation.
	*/
	I_REF(i2d::ICalibration2d, m_logTransformCompPtr);

	/**
		Camera parameters.
	*/
	I_REF(iprm::IParamsManager, m_cameraParamsManagerCompPtr);

	// security mutex
	mutable QMutex m_lock;
};


} // namespace icam


#endif // !icam_CMultiCameraBitmapSupplierComp_included


