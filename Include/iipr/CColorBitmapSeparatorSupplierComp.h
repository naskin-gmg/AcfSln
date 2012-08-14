#ifndef iipr_CColorBitmapSeparatorSupplierComp_included
#define iipr_CColorBitmapSeparatorSupplierComp_included


// ACF includes
#include "i2d/ICalibrationProvider.h"
#include "iproc/IProcessor.h"
#include "iproc/TSupplierCompWrap.h"
#include "istd/TPointerVector.h"

// ACF-Solutions includes
#include "iipr/IBitmapProvider.h"
#include "iipr/IMultiBitmapProvider.h"


namespace iipr
{

/**
	Supplier of bitmap's vector built on bitmap 
*/
class CColorBitmapSeparatorSupplierComp:
			public iproc::TSupplierCompWrap< istd::TPointerVector<iimg::IBitmap> >,
			virtual public iipr::IMultiBitmapProvider
{
public:
	typedef iproc::TSupplierCompWrap< istd::TPointerVector<iimg::IBitmap> > BaseClass;

	I_BEGIN_COMPONENT(CColorBitmapSeparatorSupplierComp);
		I_REGISTER_INTERFACE(iipr::IMultiBitmapProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapSupplier", "Provide input image", true, "BitmapSupplier");
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_calibrationCompPtr, "Calibration", "Optional calibration object", false, "Calibration");
		I_ASSIGN_TO(m_calibrationModelCompPtr, m_calibrationCompPtr, false);
		I_ASSIGN(m_useAlphaChannelAttrPtr, "UseAlphaChannel", "If enabled and image format of bitmap is RGBA, create bitmap for alpha chanel", true, false);		
	I_END_COMPONENT;

protected:
	bool EnsureBitmapCreated(ProductType& result) const;

	// reimplemented (iipr::IMultiBitmapProvider)
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;
	virtual const i2d::ITransformation2d* GetLogTransform(int bitmapIndex) const;	

	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(iipr::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);

	I_REF(i2d::ITransformation2d, m_calibrationCompPtr);
	I_REF(imod::IModel, m_calibrationModelCompPtr);

	I_ATTR(bool, m_useAlphaChannelAttrPtr);
};


} // namespace iipr


#endif // !iipr_CColorBitmapSeparatorSupplierComp_included


