// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_CExtremumCaliperSupplierComp_included
#define iipr_CExtremumCaliperSupplierComp_included


// Acf includes
#include <imeas/IDataSequenceProvider.h>
#include <iinsp/TSupplierCompWrap.h>
#include <iipr/CFeaturesContainer.h>
#include <iipr/IFeaturesProvider.h>
#include <iproc/IProcessor.h>


namespace iipr
{


class CExtremumCaliperSupplierComp: 
			public iinsp::TSupplierCompWrap<iipr::CFeaturesContainer>,
			virtual public IFeaturesProvider
{
public:
	typedef iinsp::TSupplierCompWrap<iipr::CFeaturesContainer> BaseClass;

	I_BEGIN_COMPONENT(CExtremumCaliperSupplierComp);
		I_REGISTER_INTERFACE(IFeaturesProvider);
		I_ASSIGN(m_sequenceProviderCompPtr, "SequenceProvider", "Provides sequence to analyse", true, "SequenceProvider");
		I_ASSIGN_TO(m_sequenceSupplierCompPtr, m_sequenceProviderCompPtr, false);
		I_ASSIGN_TO(m_sequenceProviderModelCompPtr, m_sequenceProviderCompPtr, false);
		I_ASSIGN(m_extremumProcessorCompPtr, "ExtremumProcessor", "Processor for extremum calculation", true, "ExtremumProcessor");
	I_END_COMPONENT;

	// reimplemented (iipr::IFeaturesProvider)
	virtual int GetFeaturesCount() const;
	virtual const imeas::INumericValue& GetFeature(int index) const;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual iinsp::ISupplier::WorkStatus ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_REF(imeas::IDataSequenceProvider, m_sequenceProviderCompPtr);
	I_REF(iinsp::ISupplier, m_sequenceSupplierCompPtr);
	I_REF(imod::IModel, m_sequenceProviderModelCompPtr);
	I_REF(iproc::IProcessor, m_extremumProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CExtremumCaliperSupplierComp_included
