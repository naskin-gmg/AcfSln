#ifndef imeas_CDataSequenceJoinerSupplierComp_included
#define imeas_CDataSequenceJoinerSupplierComp_included


// ACF-Solutions includes
#include "iproc/IProcessor.h"
#include "iinsp/ISupplier.h"
#include "iinsp/TSupplierCompWrap.h"
#include "imeas/CGeneralDataSequence.h"
#include "imeas/IDataSequenceProvider.h"
#include "imeas/IMultiDataSequenceProvider.h"


namespace imeas
{


class CDataSequenceJoinerSupplierComp:
			public iinsp::TSupplierCompWrap<
						imod::TModelWrap< imeas::CGeneralDataSequence> >,
			virtual public imeas::IDataSequenceProvider
{
public:
	typedef iinsp::TSupplierCompWrap<
				imod::TModelWrap< imeas::CGeneralDataSequence> > BaseClass;

	I_BEGIN_COMPONENT(CDataSequenceJoinerSupplierComp);
		I_REGISTER_INTERFACE(imeas::IDataSequenceProvider);
		I_ASSIGN_MULTI_0(m_dataSequenceProviderCompPtr, "DataSequenceProvider", "Provide list of the data sequence provider", false);
		I_ASSIGN_TO(m_dataSequenceSupplierCompPtr, m_dataSequenceProviderCompPtr, false);
		I_ASSIGN_TO(m_dataSequenceProviderModelCompPtr, m_dataSequenceProviderCompPtr, false);
		I_ASSIGN_MULTI_0(m_multiDataSequenceProviderCompPtr, "MultiDataSequenceProvider", "Provide list of the multi data sequence provider", false);
		I_ASSIGN_TO(m_multiDataSequenceSupplierCompPtr, m_multiDataSequenceProviderCompPtr, false);
		I_ASSIGN_TO(m_multiDataSequenceProviderModelCompPtr, m_multiDataSequenceProviderCompPtr, false);
	I_END_COMPONENT;

protected:
	// reimplemented (imeas::IDataSequenceProvider)
	virtual const imeas::IDataSequence* GetDataSequence() const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_MULTIREF(imeas::IDataSequenceProvider, m_dataSequenceProviderCompPtr);
	I_MULTIREF(iinsp::ISupplier, m_dataSequenceSupplierCompPtr);
	I_MULTIREF(imod::IModel, m_dataSequenceProviderModelCompPtr);
	I_MULTIREF(imeas::IMultiDataSequenceProvider, m_multiDataSequenceProviderCompPtr);
	I_MULTIREF(iinsp::ISupplier, m_multiDataSequenceSupplierCompPtr);
	I_MULTIREF(imod::IModel, m_multiDataSequenceProviderModelCompPtr);
};


} // namespace imeas


#endif // !imeas_CDataSequenceJoinerSupplierComp_included


