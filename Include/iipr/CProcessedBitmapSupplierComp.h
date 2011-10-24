#ifndef iipr_CProcessedBitmapSupplierComp_included
#define iipr_CProcessedBitmapSupplierComp_included


// STL includes
#include <utility>

// ACF includes
#include "iproc/IProcessor.h"
#include "iproc/TSupplierCompWrap.h"

// IACF includes
#include "iipr/IBitmapProvider.h"


namespace iipr
{


/**
	Image supplier providing processed image from some other input image supplier.
*/
class CProcessedBitmapSupplierComp:
			public iproc::TSupplierCompWrap< std::pair<istd::TDelPtr<i2d::ITransformation2d>,  istd::TDelPtr<iimg::IBitmap> > >,
			virtual public iipr::IBitmapProvider
{
public:
	typedef iproc::TSupplierCompWrap< std::pair<istd::TDelPtr<i2d::ITransformation2d>,  istd::TDelPtr<iimg::IBitmap> > > BaseClass;

	I_BEGIN_COMPONENT(CProcessedBitmapSupplierComp);
		I_REGISTER_INTERFACE(iipr::IBitmapProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapSupplier", "Provide input image", true, "BitmapSupplier");
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_imageProcessorCompPtr, "BitmapProcessor", "Bitmap conversion processor (takes bitmap as input and output)", true, "BitmapProcessor");
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

	I_REF(iipr::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iproc::IProcessor, m_imageProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CProcessedBitmapSupplierComp_included


