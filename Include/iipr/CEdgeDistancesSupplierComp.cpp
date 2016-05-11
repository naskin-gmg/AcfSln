#include "iipr/CEdgeDistancesSupplierComp.h"



namespace iipr
{


// reimplemented (imeas::INumericValueProvider)

int CEdgeDistancesSupplierComp::GetValuesCount() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetValuesCount();
	}

	return 0;
}


const imeas::INumericValue& CEdgeDistancesSupplierComp::GetNumericValue(int index) const
{
	Q_ASSERT(index == 0);

	const ProductType* productPtr = GetWorkProduct();
	Q_ASSERT(productPtr != NULL);

	return productPtr->GetNumericValue(index);
}


// protected methods

// reimplemented (iinsp::TSupplierCompWrap)

int CEdgeDistancesSupplierComp::ProduceObject(ProductType& result) const
{
	result.ResetFeatures();

	if (		m_bitmapProviderCompPtr.IsValid() &&
				m_processorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (bitmapPtr == NULL){
			AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QObject::tr("No input image"), "EdgeDistances"));

			return WS_ERROR;
		}

		iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

		Timer performanceTimer(this, "Distance calculation");

		int processorState = m_processorCompPtr->DoProcessing(
						paramsSetPtr,
						bitmapPtr,
						&result);

		if (processorState != iproc::IProcessor::TS_OK){
			return WS_ERROR;
		}

		if (result.GetValuesCount() < 1){
			return WS_ERROR;
		}

		return WS_OK;
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CEdgeDistancesSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapSupplierCompPtr.GetPtr());
	}
}


} // namespace iipr


