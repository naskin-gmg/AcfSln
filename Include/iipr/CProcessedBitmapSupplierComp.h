#ifndef iipr_CProcessedBitmapSupplierComp_included
#define iipr_CProcessedBitmapSupplierComp_included


// ACF includes
#include "i2d/ICalibrationProvider.h"

// ACF-Solutions includes
#include "iproc/IProcessor.h"
#include "iinsp/ISupplier.h"
#include "iinsp/TSupplierCompWrap.h"
#include "iimg/IBitmapProvider.h"


namespace iipr
{


/**
	Image supplier providing processed image from some other input image supplier.
*/
class CProcessedBitmapSupplierBase:
			public iinsp::TSupplierCompWrap<istd::TDelPtr<iimg::IBitmap> >,
			virtual public iimg::IBitmapProvider
{
public:
	typedef iinsp::TSupplierCompWrap<istd::TDelPtr<iimg::IBitmap> > BaseClass;

	I_BEGIN_BASE_COMPONENT(CProcessedBitmapSupplierBase);
		I_REGISTER_INTERFACE(iimg::IBitmapProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provide input image", true, "BitmapProvider");
		I_ASSIGN_TO(m_bitmapSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_imageProcessorCompPtr, "BitmapProcessor", "Bitmap conversion processor (takes bitmap as input and output)", true, "BitmapProcessor");
	I_END_COMPONENT;

protected:
	bool EnsureBitmapCreated(ProductType& result) const;

	// abstract methods
	virtual iimg::IBitmap* CreateBitmap() const = 0;

	// reimplemented (iimg::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapSupplierCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iproc::IProcessor, m_imageProcessorCompPtr);
};


class CProcessedBitmapSupplierComp: public CProcessedBitmapSupplierBase
{
public:
	typedef CProcessedBitmapSupplierBase BaseClass;

	I_BEGIN_COMPONENT(CProcessedBitmapSupplierComp);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
	I_END_COMPONENT;

protected:
	// reimplemented (CProcessedBitmapSupplierBase)
	virtual iimg::IBitmap* CreateBitmap() const
	{
		if (!m_bitmapCompFact.IsValid()){
			return false;
		}

		return m_bitmapCompFact.CreateInstance();
	}

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);
};


} // namespace iipr


#endif // !iipr_CProcessedBitmapSupplierComp_included


