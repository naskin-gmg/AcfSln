#ifndef iipr_CDifferenceBitmapSupplierComp_included
#define iipr_CDifferenceBitmapSupplierComp_included


// ACF includes
#include "i2d/ICalibrationProvider.h"
#include "iproc/IProcessor.h"
#include "iproc/TSupplierCompWrap.h"

// ACF-Solutions includes
#include "iimg/IBitmapProvider.h"


namespace iipr
{


/**
	Supplier of bitmap built on difference of two images.
*/
class CDifferenceBitmapSupplierComp:
			public iproc::TSupplierCompWrap< QPair<i2d::ICalibration2d*,  istd::TDelPtr<iimg::IBitmap> > >,
			virtual public iimg::IBitmapProvider,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iproc::TSupplierCompWrap< QPair<i2d::ICalibration2d*,  istd::TDelPtr<iimg::IBitmap> > > BaseClass;

	I_BEGIN_COMPONENT(CDifferenceBitmapSupplierComp);
		I_REGISTER_INTERFACE(iimg::IBitmapProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_firstBitmapProviderCompPtr, "FirstBitmapSupplier", "Provide fist input image", true, "FirstBitmapSupplier");
		I_ASSIGN_TO(m_firstBitmapSupplierCompPtr, m_firstBitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_firstBitmapProviderModelCompPtr, m_firstBitmapProviderCompPtr, false);
		I_ASSIGN(m_secondBitmapProviderCompPtr, "SecondBitmapSupplier", "Provide second input image", true, "SecondBitmapSupplier");
		I_ASSIGN_TO(m_secondBitmapSupplierCompPtr, m_secondBitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_secondBitmapProviderModelCompPtr, m_secondBitmapProviderCompPtr, false);
	I_END_COMPONENT;

protected:
	bool EnsureBitmapCreated(ProductType& result) const;

	// reimplemented (iimg::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	int CalculateDifferenceBitmap(
				const iimg::IBitmap& firstInputBitmap,
				const iimg::IBitmap& secondInputBitmap,
				ProductType& result) const;
private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(iimg::IBitmapProvider, m_firstBitmapProviderCompPtr);
	I_REF(iproc::ISupplier, m_firstBitmapSupplierCompPtr);
	I_REF(imod::IModel, m_firstBitmapProviderModelCompPtr);

	I_REF(iimg::IBitmapProvider, m_secondBitmapProviderCompPtr);
	I_REF(iproc::ISupplier, m_secondBitmapSupplierCompPtr);
	I_REF(imod::IModel, m_secondBitmapProviderModelCompPtr);
};


} // namespace iipr


#endif // !iipr_CDifferenceBitmapSupplierComp_included


