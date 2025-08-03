#ifndef iipr_CBitmapJoinerSupplierComp_included
#define iipr_CBitmapJoinerSupplierComp_included


// ACF includes
#include <i2d/ICalibrationProvider.h>
#include <istd/TPointerVector.h>

// ACF-Solutions includes
#include <iproc/IProcessor.h>
#include <iinsp/ISupplier.h>
#include <iinsp/TSupplierCompWrap.h>
#include <iimg/IBitmapProvider.h>
#include <iimg/IMultiBitmapProvider.h>


namespace iipr
{

/**
	Supplier of bitmap's vector built on bitmap 
*/
class CBitmapJoinerSupplierComp:
			public iinsp::TSupplierCompWrap< QPair<i2d::ITransformation2d*,  iimg::IBitmapSharedPtr> >,
			virtual public iimg::IBitmapProvider
{
public:
	typedef iinsp::TSupplierCompWrap< QPair<i2d::ITransformation2d*, iimg::IBitmapSharedPtr> > BaseClass;

	I_BEGIN_COMPONENT(CBitmapJoinerSupplierComp);
		I_REGISTER_INTERFACE(iimg::IBitmapProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapsProviderCompPtr, "BitmapListSupplier", "Provide list of the input images", true, "BitmapListSupplier");
		I_ASSIGN_TO(m_bitmapsSupplierCompPtr, m_bitmapsProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapsProviderModelCompPtr, m_bitmapsProviderCompPtr, false);
		I_ASSIGN(m_useHorizontalJoinAttrPtr, "JoinHorizontal", "Join the image in horizontal direction", true, false);
	I_END_COMPONENT;

protected:
	bool EnsureBitmapCreated(ProductType& result) const;

	// reimplemented (iimg::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;
	virtual const i2d::ITransformation2d* GetLogTransform() const;	

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual iinsp::ISupplier::WorkStatus ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(iimg::IMultiBitmapProvider, m_bitmapsProviderCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapsSupplierCompPtr);
	I_REF(imod::IModel, m_bitmapsProviderModelCompPtr);

	I_ATTR(bool, m_useHorizontalJoinAttrPtr);
};


} // namespace iipr


#endif // !iipr_CBitmapJoinerSupplierComp_included


