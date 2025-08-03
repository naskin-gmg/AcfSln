#pragma once


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include <istd/TDelPtr.h>
#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>
#include <iimg/IBitmap.h>
#include <iimg/IBitmapProvider.h>
#include <iinsp/TSupplierCompWrap.h>

// ACF-Solutions includes
#include <imeas/INumericValue.h>


namespace icam
{


/**
	Basic implementation of snap bitmap supplier.
	Creation of the resulting bitmap object and implementation of the low-level snap functionality should be done in the derrived class.
*/
class CSnapBitmapSupplierCompBase:
			public iinsp::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>, iimg::IBitmapSharedPtr > >,
			virtual public iimg::IBitmapProvider,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iinsp::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>,  iimg::IBitmapSharedPtr > > BaseClass;

	I_BEGIN_BASE_COMPONENT(CSnapBitmapSupplierCompBase);
		I_REGISTER_INTERFACE(iimg::IBitmapProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_calibrationIdAttrPtr, "CalibrationParamId", "ID of calibration in parameter set", false, "Calibration");
		I_ASSIGN(m_defaultCalibrationCompPtr, "Calibration", "Optional calibration object", false, "Calibration");
		I_ASSIGN(m_scaleParamIdAttrPtr, "ScaleParamId", "ID of resolution parameter (pixel/mm) in parameter set", false, "Scale");
		I_ASSIGN(m_defaultScaleValueCompPtr, "DefaultScaleParam", "Default resolution in pixel/mm", false, "DefaultScaleParam");
		I_ASSIGN(m_moveCalibrationToCenterAttrPtr, "MoveCalibrationToCenter", "if true calibration zero is center of the image", true, true);
		I_ASSIGN(m_allowEmptyImageResultAttrPtr, "AllowEmptyImageResult", "if true is empty image result is not an error", false, false);
	I_END_COMPONENT;

	// reimplemented (iimg::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	// abstract methods
	virtual iimg::IBitmapUniquePtr CreateBitmap() const = 0;

	/**
		Execute snap on the camera according to the given parameters.
		\return Snap status. \sa IProcessor::TaskState
	*/
	virtual int DoSnap(const iprm::IParamsSet* snapParamsPtr, iimg::IBitmap& snapBitmap) const = 0;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual iinsp::ISupplier::WorkStatus ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_ATTR(QByteArray, m_calibrationIdAttrPtr);
	I_REF(i2d::ICalibration2d, m_defaultCalibrationCompPtr);
	I_ATTR(QByteArray, m_scaleParamIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultScaleValueCompPtr);
	I_ATTR(bool, m_moveCalibrationToCenterAttrPtr);
	I_ATTR(bool, m_allowEmptyImageResultAttrPtr);
};


} // namespace icam


