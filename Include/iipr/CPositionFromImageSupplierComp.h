#ifndef iipr_CPositionFromImageSupplierComp_included
#define iipr_CPositionFromImageSupplierComp_included


// ACF includes
#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>
#include <i2d/CVector2d.h>

// ACF-Solutions includes
#include <iproc/IProcessor.h>
#include <iinsp/ISupplier.h>
#include <iinsp/TSupplierCompWrap.h>
#include <imeas/INumericValueProvider.h>
#include <imeas/CSimpleNumericValue.h>
#include <iimg/IBitmapProvider.h>


namespace iipr
{


/**
	Implementation of value supplier returning position on the image.
	To calculate this position is some processor used.
	This processor must accept image object as input and produce some set of features containing position into feature consumer.
	This supplier takes the feature with the higher weight value and output it as found position value.
*/
class CPositionFromImageSupplierComp:
			public iinsp::TSupplierCompWrap< istd::TDelPtr<imeas::INumericValue> >,
			virtual public imeas::INumericValueProvider,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iinsp::TSupplierCompWrap< istd::TDelPtr<imeas::INumericValue> > BaseClass;

	I_BEGIN_COMPONENT(CPositionFromImageSupplierComp);
		I_REGISTER_INTERFACE(imeas::INumericValueProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provide image to analyse", true, "BitmapProvider");
		I_ASSIGN_TO(m_bitmapSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_calibrationProviderCompPtr, "CalibrationProvider", "Provide 2D-calibration object", false, "CalibrationProvider");
		I_ASSIGN(m_processorCompPtr, "Processor", "Processor calculating set of positions from image", true, "Processor");
	I_END_COMPONENT;

	// reimplemented (imeas::INumericValueProvider)
	virtual int GetValuesCount() const;
	virtual const imeas::INumericValue& GetNumericValue(int index) const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected:
	mutable istd::TDelPtr<const i2d::ICalibration2d> m_outputCalibrationPtr;

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapSupplierCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(i2d::ICalibrationProvider, m_calibrationProviderCompPtr);
	I_REF(iproc::IProcessor, m_processorCompPtr);

	class Position: public imeas::CSimpleNumericValue
	{
	public:
		Position(){}
		Position(const imath::CVarVector& positionVector);

		// reimplemented (imeas::INumericValue)
		virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const;
		virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const;
	};
};


} // namespace iipr


#endif // !iipr_CPositionFromImageSupplierComp_included


