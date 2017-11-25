#ifndef icam_CBitmapSupplierMultiplexerComp_included
#define icam_CBitmapSupplierMultiplexerComp_included


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include <istd/TDelPtr.h>
#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>
#include <iimg/IBitmap.h>
#include <iprm/ISelectionParam.h>

// ACF-Solutions includes
#include <iinsp/ISupplier.h>
#include <iinsp/TSupplierCompWrap.h>
#include <iimg/IBitmapProvider.h>


namespace icam
{


/**
	Implementation of bitmap supplier based on selection of the underlayed bitmap provider.
*/
class CBitmapSupplierMultiplexerComp:
			public iinsp::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>, istd::TDelPtr<iimg::IBitmap> > >,
			virtual public iimg::IBitmapProvider,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iinsp::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>,  istd::TDelPtr<iimg::IBitmap> > > BaseClass;

	I_BEGIN_COMPONENT(CBitmapSupplierMultiplexerComp);
		I_REGISTER_INTERFACE(iimg::IBitmapProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN_MULTI_0(m_bitmapProvidersCompPtr, "BitmapProviders", "List of bitmap providers", true);
		I_ASSIGN_TO(m_calibProvidersCompPtr, m_bitmapProvidersCompPtr, false);
		I_ASSIGN_TO(m_bitmapSuppliersCompPtr, m_bitmapProvidersCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderModelsCompPtr, m_bitmapProvidersCompPtr, false);
		I_ASSIGN(m_bitmapSelectionCompPtr, "BitmapSelection", "Index of the bitmap in the list", true, "BitmapSelection");
		I_ASSIGN_TO(m_bitmapSelectionModelCompPtr, m_bitmapSelectionCompPtr, true);
	I_END_COMPONENT;

	// reimplemented (iimg::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_MULTIREF(iimg::IBitmapProvider, m_bitmapProvidersCompPtr);
	I_MULTIREF(i2d::ICalibrationProvider, m_calibProvidersCompPtr);
	I_MULTIREF(iinsp::ISupplier, m_bitmapSuppliersCompPtr);
	I_MULTIREF(imod::IModel, m_bitmapProviderModelsCompPtr);

	I_REF(iprm::ISelectionParam, m_bitmapSelectionCompPtr);
	I_REF(imod::IModel, m_bitmapSelectionModelCompPtr);
};


} // namespace icam


#endif // !icam_CBitmapSupplierMultiplexerComp_included


