#ifndef iipr_CSearchBasedFeaturesSupplierComp_included
#define iipr_CSearchBasedFeaturesSupplierComp_included


// ACF includes
#include "i2d/CVector2d.h"
#include "iprm/IParamsSet.h"
#include "iproc/IProcessor.h"
#include "iproc/TSupplierCompWrap.h"

#include "iipr/IBitmapProvider.h"
#include "iipr/IFeaturesProvider.h"
#include "iipr/CFeaturesContainer.h"


namespace iipr
{


class CSearchBasedFeaturesSupplierComp:
			public iproc::TSupplierCompWrap<CFeaturesContainer>,
			virtual public iipr::IFeaturesProvider
{
public:
	typedef iproc::TSupplierCompWrap<CFeaturesContainer> BaseClass;

	I_BEGIN_COMPONENT(CSearchBasedFeaturesSupplierComp);
		I_REGISTER_INTERFACE(iipr::IFeaturesProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapSupplier", "Provide image to analyse", true, "BitmapSupplier");
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_searchProcessorCompPtr, "Processor", "Calculate model positions in the image", true, "Processor");
	I_END_COMPONENT;

	// reimplemented (iipr::IFeaturesProvider)
	virtual Features GetFeatures() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(CFeaturesContainer& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iipr::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iproc::IProcessor, m_searchProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CSearchBasedFeaturesSupplierComp_included


