#include "iipr/CImageHistogramSupplierComp.h"


namespace iipr
{


// public methods

// reimplemented (imeas::IDataSequenceProvider)

const imeas::IDataSequence* CImageHistogramSupplierComp::GetDataSequence() const
{
	(const_cast<CImageHistogramSupplierComp*>(this))->EnsureWorkInitialized();

	return GetWorkProduct();
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CImageHistogramSupplierComp::ProduceObject(imod::TModelWrap<imeas::CSimpleSamplesSequence32>& result) const
{
	if (		m_bitmapProviderCompPtr.IsValid() &&
				m_histogramProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			Timer performanceTimer(this, "Histogram calculation");

			int processingState = m_histogramProcessorCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							&result);

			if (processingState != iproc::IProcessor::TS_OK){
				return WS_ERROR;
			}
			return WS_OK;
		}
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CImageHistogramSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapSupplierCompPtr.GetPtr());
	}
}


void CImageHistogramSupplierComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		UnregisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr());
	}
}


} // namespace iipr


