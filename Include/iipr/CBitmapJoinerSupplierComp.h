#ifndef iipr_CBitmapJoinerSupplierComp_included
#define iipr_CBitmapJoinerSupplierComp_included


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
class CBitmapJoinerSupplierComp:
			public iproc::TSupplierCompWrap< QPair<i2d::ITransformation2d*,  istd::TDelPtr<iimg::IBitmap> > >,
			virtual public iipr::IBitmapProvider
{
public:
	typedef iproc::TSupplierCompWrap< QPair<i2d::ITransformation2d*,  istd::TDelPtr<iimg::IBitmap> > > BaseClass;

	I_BEGIN_COMPONENT(CBitmapJoinerSupplierComp);
		I_REGISTER_INTERFACE(iipr::IBitmapProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_inputBitmapsProviderCompPtr, "BitmapListSupplier", "Provide list of the input images", true, "BitmapListSupplier");
		I_ASSIGN_TO(m_inputBitmapsProviderModelCompPtr, m_inputBitmapsProviderCompPtr, false);
		I_ASSIGN(m_calibrationCompPtr, "Calibration", "Optional calibration object", false, "Calibration");
		I_ASSIGN_TO(m_calibrationModelCompPtr, m_calibrationCompPtr, false);
		I_ASSIGN(m_useHorizontalJoinAttrPtr, "JoinHorizontal", "Join the image in horizontal direction", true, false);
	I_END_COMPONENT;

protected:
	bool EnsureBitmapCreated(ProductType& result) const;

	// reimplemented (iipr::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;
	virtual const i2d::ITransformation2d* GetLogTransform() const;	

	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(iipr::IMultiBitmapProvider, m_inputBitmapsProviderCompPtr);
	I_REF(imod::IModel, m_inputBitmapsProviderModelCompPtr);

	I_REF(i2d::ITransformation2d, m_calibrationCompPtr);
	I_REF(imod::IModel, m_calibrationModelCompPtr);

	I_ATTR(bool, m_useHorizontalJoinAttrPtr);
};


} // namespace iipr


#endif // !iipr_CBitmapJoinerSupplierComp_included


