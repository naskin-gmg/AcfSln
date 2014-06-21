#ifndef iipr_CArcProjectionSupplierComp_included
#define iipr_CArcProjectionSupplierComp_included


// ACF-Solutions includes
#include "iinsp/ISupplier.h"
#include "iinsp/TSupplierCompWrap.h"
#include "iimg/IBitmapProvider.h"
#include "iipr/IArcProjectionProcessor.h"
#include "imeas/IMultiDataSequenceProvider.h"
#include "imeas/INumericValueProvider.h"
#include "imeas/CGeneralDataSequence.h"


namespace iipr
{


/**
	Provider of bitmap projection series, which are calculated based on single arc
*/
class CArcProjectionSupplierComp:
			public iinsp::TSupplierCompWrap<imeas::CGeneralDataSequence>,
			virtual public imeas::IDataSequenceProvider
{
public:
	typedef iinsp::TSupplierCompWrap<imeas::CGeneralDataSequence> BaseClass;

	I_BEGIN_COMPONENT(CArcProjectionSupplierComp);
		I_REGISTER_INTERFACE(imeas::IDataSequenceProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provides image to analyse", true, "BitmapProvider");
		I_ASSIGN_TO(m_bitmapSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_arcCompPtr, "ArcProvider", "Provides an arc object", true, "ArcProvider");
		I_ASSIGN_TO(m_arcModelCompPtr, m_arcCompPtr, false);
		I_ASSIGN(m_projectionProcessorCompPtr, "ProjectionProcessor", "Processor for projection data generation", true, "ProjectionProcessor");
	I_END_COMPONENT;

	// reimplemented (imeas::IDataSequenceProvider)
	virtual const imeas::IDataSequence* GetDataSequence() const;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapSupplierCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(i2d::CArc, m_arcCompPtr);
	I_REF(imod::IModel, m_arcModelCompPtr);
	I_REF(iipr::IArcProjectionProcessor, m_projectionProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CArcProjectionSupplierComp_included
