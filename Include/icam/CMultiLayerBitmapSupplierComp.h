#ifndef icam_CMultiLayerBitmapSupplierComp_included
#define icam_CMultiLayerBitmapSupplierComp_included


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include "istd/TDelPtr.h"
#include "iimg/CMultiPageBitmapBase.h"

// ACF-Solutions includes
#include "iproc/TSupplierCompWrap.h"
#include "icam/IBitmapAcquisition.h"
#include <imeas/INumericConstraints.h>
#include <imeas/CSimpleNumericValue.h>


namespace icam
{


/**
	Implementation of bitmap supplier based on image acquisition.
*/
class CMultiLayerBitmapSupplierComp:
			public iproc::TSupplierCompWrap< istd::TDelPtr<iimg::IMultiBitmapProvider> >,
			virtual public iimg::IMultiBitmapProvider
{
public:
	typedef iproc::TSupplierCompWrap< istd::TDelPtr<iimg::IMultiBitmapProvider> > BaseClass;

	I_BEGIN_COMPONENT(CMultiLayerBitmapSupplierComp);
		I_REGISTER_INTERFACE(iimg::IMultiBitmapProvider);
		I_REGISTER_SUBELEMENT(ScaleConstraints);		
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquisition object for image snap", true, "BitmapAcquisition");
	I_END_COMPONENT;

	// reimplemented (iimg::IMultiBitmapProvider)
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;
	virtual const iprm::IOptionsList* GetBitmapListInfo() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual bool InitializeWork();
	virtual int ProduceObject(ProductType& result) const;

private:
	I_FACT(iimg::IMultiBitmapProvider, m_bitmapCompFact);
	I_REF(IBitmapAcquisition, m_bitmapAcquisitionCompPtr);
};


} // namespace icam


#endif // !icam_CMultiLayerBitmapSupplierComp_included


