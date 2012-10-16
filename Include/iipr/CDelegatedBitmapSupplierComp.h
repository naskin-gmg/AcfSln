#ifndef iipr_CDelegatedBitmapSupplierComp_included
#define iipr_CDelegatedBitmapSupplierComp_included


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include "imod/IModel.h"
#include "imod/CMultiModelObserverBase.h"
#include "i2d/ICalibrationProvider.h"
#include "iproc/TSupplierCompWrap.h"

// ACF-Solutions includes
#include "iipr/IBitmapProvider.h"


namespace iipr
{


/**
	Image supplier delegating the calls to another one or accessing some bitmap object directly.
*/
class CDelegatedBitmapSupplierComp:
			public iproc::TSupplierCompWrap< QPair<const i2d::ITransformation2d*, const iimg::IBitmap*> >,
			virtual public IBitmapProvider,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iproc::TSupplierCompWrap< QPair<const i2d::ITransformation2d*, const iimg::IBitmap*> > BaseClass;

	I_BEGIN_COMPONENT(CDelegatedBitmapSupplierComp);
		I_REGISTER_INTERFACE(IBitmapProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_inputBitmapProviderCompPtr, "SlaveSupplier", "Slave supplier where all calls will be delegated", false, "SlaveSupplier");
		I_ASSIGN_TO(m_inputBitmapProviderModelCompPtr, m_inputBitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_calibrationProviderCompPtr, m_inputBitmapProviderCompPtr, false);
		I_ASSIGN(m_bitmapCompPtr, "BitmapObject", "Bitmap object used if no slave supplier is provided", false, "BitmapObject");
		I_ASSIGN_TO(m_bitmapModelCompPtr, m_bitmapCompPtr, false);
		I_ASSIGN(m_calibrationCompPtr, "CalibrationObject", "Calibration object used as image to log transformation, if no slave supplier is provided", false, "CalibrationObject");
		I_ASSIGN_TO(m_calibrationModelCompPtr, m_calibrationCompPtr, false);
	I_END_COMPONENT;

	CDelegatedBitmapSupplierComp();

protected:
	// reimplemented (iipr::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ITransformation2d* GetCalibration() const;

	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iipr::IBitmapProvider, m_inputBitmapProviderCompPtr);
	I_REF(i2d::ICalibrationProvider, m_calibrationProviderCompPtr);
	I_REF(imod::IModel, m_inputBitmapProviderModelCompPtr);
	I_REF(iimg::IBitmap, m_bitmapCompPtr);
	I_REF(imod::IModel, m_bitmapModelCompPtr);
	I_REF(i2d::ITransformation2d, m_calibrationCompPtr);
	I_REF(imod::IModel, m_calibrationModelCompPtr);
};


} // namespace iipr


#endif // !iipr_CDelegatedBitmapSupplierComp_included


