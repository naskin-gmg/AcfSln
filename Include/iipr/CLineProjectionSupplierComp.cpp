#include "iipr/CLineProjectionSupplierComp.h"


#include "iser/CMemoryReadArchive.h"

#include "iipr/CRectDerivativeProcessor.h"


namespace iipr
{


// reimplemented (iproc::IValueProvider)

const imeas::IDataSequence* CLineProjectionSupplierComp::GetDataSequence() const
{
	return GetWorkProduct();
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CLineProjectionSupplierComp::ProduceObject(imeas::CGeneralDataSequence& result) const
{
	if (		m_bitmapProviderCompPtr.IsValid() &&
				m_projectionProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			int projectionState = m_projectionProcessorCompPtr->DoProcessing(
						paramsSetPtr,
						bitmapPtr,
						&result);
			if (projectionState == iproc::IProcessor::TS_OK){
				return WS_OK;
			}
		}

		return WS_ERROR;
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CLineProjectionSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr());
	}
}


} // namespace iipr


