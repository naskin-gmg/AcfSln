#ifndef iipr_CEdgeDistancesSupplierComp_included
#define iipr_CEdgeDistancesSupplierComp_included


// ACF includes
#include <istd/TPointerVector.h>
#include <i2d/CVector2d.h>

// ACF-Solutions includes
#include <iproc/IProcessor.h>
#include <iinsp/ISupplier.h>
#include <iinsp/TSupplierCompWrap.h>
#include <imeas/INumericValueProvider.h>
#include <imeas/CSimpleNumericValue.h>
#include <iimg/IBitmapProvider.h>
#include <iipr/IImageToFeatureProcessor.h>
#include <iipr/CCaliperDistanceFeature.h>
#include <iipr/CFeaturesContainer.h>


namespace iipr
{


class CEdgeDistancesSupplierComp:
			public iinsp::TSupplierCompWrap<CFeaturesContainer>,
			virtual public imeas::INumericValueProvider
{
public:
	typedef iinsp::TSupplierCompWrap<CFeaturesContainer> BaseClass;

	I_BEGIN_COMPONENT(CEdgeDistancesSupplierComp);
		I_REGISTER_INTERFACE(imeas::INumericValueProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provide image to analyse", true, "BitmapProvider");
		I_ASSIGN_TO(m_bitmapSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_processorCompPtr, "Processor", "Processor calculating set of positions from image", true, "Processor");
	I_END_COMPONENT;

	// reimplemented (imeas::INumericValueProvider)
	virtual int GetValuesCount() const;
	virtual const imeas::INumericValue& GetNumericValue(int index) const;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapSupplierCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iproc::IProcessor, m_processorCompPtr);
};


} // namespace iipr


#endif // !iipr_CEdgeDistancesSupplierComp_included


