#ifndef iipr_CExtremumCaliperSupplierComp_included
#define iipr_CExtremumCaliperSupplierComp_included


// Acf includes
#include "iinsp/TSupplierCompWrap.h"
#include "iipr/CFeaturesContainer.h"
#include "imeas/IDataSequenceProvider.h"
#include "iproc/IProcessor.h"


namespace iipr
{


class CExtremumCaliperSupplierComp: public iinsp::TSupplierCompWrap<iipr::CFeaturesContainer>
{
public:
	typedef public iinsp::TSupplierCompWrap<iipr::CFeaturesContainer> BaseClass;

	I_BEGIN_COMPONENT(CExtremumCaliperSupplierComp);
		I_ASSIGN(m_sequenceProviderCompPtr, "SequenceProvider", "Provides sequence to analyse", true, "SequenceProvider");
		I_ASSIGN_TO(m_sequenceSupplierCompPtr, m_sequenceProviderCompPtr, false);
		I_ASSIGN_TO(m_sequenceProviderModelCompPtr, m_sequenceProviderCompPtr, false);
		I_ASSIGN(m_extremumProcessorCompPtr, "ExtremumProcessor", "Processor for extremum calculation", true, "ExtremumProcessor");
	I_END_COMPONENT;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(imeas::IDataSequenceProvider, m_sequenceProviderCompPtr);
	I_REF(iinsp::ISupplier, m_sequenceSupplierCompPtr);
	I_REF(imod::IModel, m_sequenceProviderModelCompPtr);
	I_REF(iproc::IProcessor, m_extremumProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CExtremumCaliperSupplierComp_included