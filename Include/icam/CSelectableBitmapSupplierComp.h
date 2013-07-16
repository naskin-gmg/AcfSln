#ifndef icam_CSelectableBitmapSupplierComp_included
#define icam_CSelectableBitmapSupplierComp_included


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include "istd/TDelPtr.h"
#include "i2d/ICalibration2d.h"
#include "i2d/ICalibrationProvider.h"
#include "i2d/IMultiCalibrationProvider.h"
#include "iimg/IBitmap.h"
#include "iprm/ISelectionParam.h"

// ACF-Solutions includes
#include "iproc/ISupplier.h"
#include "iproc/TSupplierCompWrap.h"
#include "iimg/IBitmapProvider.h"
#include "iimg/IMultiBitmapProvider.h"


namespace icam
{


/**
	Implementation of bitmap supplier based on image acquisition.
*/
class CSelectableBitmapSupplierComp:
			public iproc::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>, istd::TDelPtr<iimg::IBitmap> > >,
			virtual public iimg::IBitmapProvider,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iproc::TSupplierCompWrap< QPair<istd::TDelPtr<const i2d::ICalibration2d>,  istd::TDelPtr<iimg::IBitmap> > > BaseClass;

	I_BEGIN_COMPONENT(CSelectableBitmapSupplierComp);
		I_REGISTER_INTERFACE(iimg::IBitmapProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_multiBitmapProviderCompPtr, "MultiBitmapProvider", "Provider of the bitmap list", true, "MultiBitmapProvider");
		I_ASSIGN_TO(m_multiBitmapCalibCompPtr, m_multiBitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_multiBitmapMultiCalibCompPtr, m_multiBitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_multiBitmapSupplierCompPtr, m_multiBitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_multiBitmapProviderModelCompPtr, m_multiBitmapProviderCompPtr, false);
		I_ASSIGN(m_bitmapSelectionCompPtr, "BitmapSelection", "Index of the bitmap in the list", true, "BitmapSelection");
		I_ASSIGN_TO(m_bitmapSelectionModelCompPtr, m_bitmapSelectionCompPtr, true);
	I_END_COMPONENT;

	// reimplemented (iimg::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(iimg::IMultiBitmapProvider, m_multiBitmapProviderCompPtr);
	I_REF(i2d::ICalibrationProvider, m_multiBitmapCalibCompPtr);
	I_REF(i2d::IMultiCalibrationProvider, m_multiBitmapMultiCalibCompPtr);
	I_REF(iproc::ISupplier, m_multiBitmapSupplierCompPtr);
	I_REF(imod::IModel, m_multiBitmapProviderModelCompPtr);

	I_REF(iprm::ISelectionParam, m_bitmapSelectionCompPtr);
	I_REF(imod::IModel, m_bitmapSelectionModelCompPtr);
};


} // namespace icam


#endif // !icam_CSelectableBitmapSupplierComp_included


