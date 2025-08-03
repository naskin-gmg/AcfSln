#ifndef iipr_CDifferenceBitmapSupplierComp_included
#define iipr_CDifferenceBitmapSupplierComp_included


// ACF includes
#include <i2d/ICalibrationProvider.h>
#include <iproc/IProcessor.h>
#include <iinsp/TSupplierCompWrap.h>

// ACF-Solutions includes
#include <iimg/IBitmapProvider.h>
#include <imeas/CGeneralNumericConstraints.h>


namespace iipr
{


/**
	Supplier of bitmap built on difference of two images.
*/
class CDifferenceBitmapSupplierComp:
			public iinsp::TSupplierCompWrap< QPair<i2d::ICalibration2d*,  iimg::IBitmapSharedPtr > >,
			virtual public iimg::IBitmapProvider,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iinsp::TSupplierCompWrap< QPair<i2d::ICalibration2d*,  iimg::IBitmapSharedPtr > > BaseClass;

	I_BEGIN_COMPONENT(CDifferenceBitmapSupplierComp);
		I_REGISTER_SUBELEMENT(OffsetContraints);
		I_REGISTER_SUBELEMENT_INTERFACE(OffsetContraints, imeas::INumericConstraints, ExtractOffsetContraints);
		I_REGISTER_SUBELEMENT_INTERFACE(OffsetContraints, istd::IChangeable, ExtractOffsetContraints);
		I_REGISTER_INTERFACE(iimg::IBitmapProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_firstBitmapProviderCompPtr, "FirstBitmapSupplier", "Provide fist input image", true, "FirstBitmapSupplier");
		I_ASSIGN_TO(m_firstBitmapSupplierCompPtr, m_firstBitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_firstBitmapProviderModelCompPtr, m_firstBitmapProviderCompPtr, false);
		I_ASSIGN(m_secondBitmapProviderCompPtr, "SecondBitmapSupplier", "Provide second input image", true, "SecondBitmapSupplier");
		I_ASSIGN_TO(m_secondBitmapSupplierCompPtr, m_secondBitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_secondBitmapProviderModelCompPtr, m_secondBitmapProviderCompPtr, false);
		I_ASSIGN(m_offsetParamIdAttrPtr, "OffsetParamId", "ID of offset value added to each pixel by difference calculation (imeas::INumericValue)", false, "Threshold");
		I_ASSIGN(m_defaultOffsetCompPtr, "DefaultOffset", "Offset value used if not specified in parameters", false, "DefaultThreshold");
	I_END_COMPONENT;

	CDifferenceBitmapSupplierComp();

	// reimplemented (iimg::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	bool EnsureBitmapCreated(ProductType& result) const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual iinsp::ISupplier::WorkStatus ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(iimg::IBitmapProvider, m_firstBitmapProviderCompPtr);
	I_REF(iinsp::ISupplier, m_firstBitmapSupplierCompPtr);
	I_REF(imod::IModel, m_firstBitmapProviderModelCompPtr);

	I_REF(iimg::IBitmapProvider, m_secondBitmapProviderCompPtr);
	I_REF(iinsp::ISupplier, m_secondBitmapSupplierCompPtr);
	I_REF(imod::IModel, m_secondBitmapProviderModelCompPtr);

	I_ATTR(QByteArray, m_offsetParamIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultOffsetCompPtr);

	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractOffsetContraints(CDifferenceBitmapSupplierComp& component)
	{
		return &component.m_offsetContraints;
	}

	imeas::CGeneralNumericConstraints m_offsetContraints;
};


} // namespace iipr


#endif // !iipr_CDifferenceBitmapSupplierComp_included


