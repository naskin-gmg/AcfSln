#ifndef iipr_CColorBitmapSeparatorSupplierComp_included
#define iipr_CColorBitmapSeparatorSupplierComp_included


// ACF includes
#include "i2d/ICalibrationProvider.h"
#include "istd/TPointerVector.h"

// ACF-Solutions includes
#include "iproc/IProcessor.h"
#include "iinsp/ISupplier.h"
#include "iinsp/TSupplierCompWrap.h"
#include "iimg/IBitmapProvider.h"
#include "iimg/IMultiBitmapProvider.h"


namespace iipr
{

/**
	Supplier of bitmap's vector built on bitmap 
*/
class CColorBitmapSeparatorSupplierComp:
			public iinsp::TSupplierCompWrap< istd::TPointerVector<iimg::IBitmap> >,
			virtual public iimg::IMultiBitmapProvider,
			virtual protected iprm::IOptionsList
{
public:
	typedef iinsp::TSupplierCompWrap< istd::TPointerVector<iimg::IBitmap> > BaseClass;

	I_BEGIN_COMPONENT(CColorBitmapSeparatorSupplierComp);
		I_REGISTER_INTERFACE(iimg::IMultiBitmapProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapSupplier", "Provide input image", true, "BitmapSupplier");
		I_ASSIGN_TO(m_bitmapSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_useAlphaChannelAttrPtr, "UseAlphaChannel", "If enabled and image format of bitmap is RGBA, create bitmap for alpha chanel", true, false);		
	I_END_COMPONENT;

	// reimplemented (iimg::IMultiBitmapProvider)
	virtual const iprm::IOptionsList* GetBitmapListInfo() const;
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;

protected:
	bool EnsureBitmapCreated(ProductType& result) const;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapSupplierCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);

	I_ATTR(bool, m_useAlphaChannelAttrPtr);
};


} // namespace iipr


#endif // !iipr_CColorBitmapSeparatorSupplierComp_included


