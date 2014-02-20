#include "imeas/CProcessedDataSequenceSupplierComp.h"


namespace imeas
{


// reimplemented (imeas::IDataSequenceProvider)

const imeas::IDataSequence* CProcessedDataSequenceSupplierComp::GetDataSequence() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr;
	}

	return NULL;
}


// reimplemented (iproc::TSupplierCompWrap)

int CProcessedDataSequenceSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_dataSequenceProviderCompPtr.IsValid() || !m_dataSequenceProcessorCompPtr.IsValid() || !m_dataSequenceSupplierCompPtr.IsValid()){
		return WS_CRITICAL;
	}

	const imeas::IDataSequence* dataSequencePtr = m_dataSequenceProviderCompPtr->GetDataSequence();
	if (dataSequencePtr == NULL){
		return WS_ERROR;
	}

	Timer performanceTimer(this, "Data sequence processing");

	istd::CChangeNotifier updater(&result);

	int status = m_dataSequenceProcessorCompPtr->DoProcessing(GetModelParametersSet(), dataSequencePtr, &result);
	switch (status){
	case iproc::IProcessor::TS_OK:
		return WS_OK;

	case iproc::IProcessor::TS_CANCELED:
		return WS_CANCELED;

	default:
		return WS_ERROR;
	}
}


// reimplemented (icomp::CComponentBase)

void CProcessedDataSequenceSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_dataSequenceProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_dataSequenceProviderModelCompPtr.GetPtr(), m_dataSequenceSupplierCompPtr.GetPtr());
	}
}


} // namespace iipr


