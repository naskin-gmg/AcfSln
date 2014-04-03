#ifndef imeas_CProcessedDataSequenceSupplierComp_included
#define imeas_CProcessedDataSequenceSupplierComp_included


// ACF-Solutions includes
#include "iproc/IProcessor.h"
#include "iinsp/ISupplier.h"
#include "iinsp/TSupplierCompWrap.h"
#include "imeas/CGeneralDataSequence.h"
#include "imeas/IDataSequenceProvider.h"


namespace imeas
{


/**
	Data sequence supplier providing processed data sequence from some other input data sequence supplier.
*/
class CProcessedDataSequenceSupplierComp:
			public iinsp::TSupplierCompWrap<
						imod::TModelWrap< imeas::CGeneralDataSequence> >,
			virtual public imeas::IDataSequenceProvider
{
public:
	typedef iinsp::TSupplierCompWrap<
				imod::TModelWrap< imeas::CGeneralDataSequence> > BaseClass;

	I_BEGIN_COMPONENT(CProcessedDataSequenceSupplierComp);
		I_REGISTER_INTERFACE(imeas::IDataSequenceProvider);
		I_ASSIGN(m_dataSequenceProviderCompPtr, "DataSequenceProvider", "Provide input data sequence", true, "DataSequenceProvider");
		I_ASSIGN_TO(m_dataSequenceSupplierCompPtr, m_dataSequenceProviderCompPtr, false);
		I_ASSIGN_TO(m_dataSequenceProviderModelCompPtr, m_dataSequenceProviderCompPtr, false);
		I_ASSIGN(m_dataSequenceProcessorCompPtr, "DataSequenceProcessor", "Data sequence conversion processor (takes data sequence as input and output)", true, "DataSequenceProcessor");
	I_END_COMPONENT;

protected:
	// reimplemented (imeas::IDataSequenceProvider)
	virtual const imeas::IDataSequence* GetDataSequence() const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(imeas::IDataSequenceProvider, m_dataSequenceProviderCompPtr);
	I_REF(iinsp::ISupplier, m_dataSequenceSupplierCompPtr);
	I_REF(imod::IModel, m_dataSequenceProviderModelCompPtr);
	I_REF(iproc::IProcessor, m_dataSequenceProcessorCompPtr);
};


} // namespace imeas


#endif // !imeas_CProcessedDataSequenceSupplierComp_included


