#include "iipr/CArcProjectionSupplierComp.h"


namespace iipr
{


// reimplemented (iinsp::TSupplierCompWrap)

int CArcProjectionSupplierComp::ProduceObject(ProductType& result) const
{
	if (		m_bitmapProviderCompPtr.IsValid() &&
				m_arcCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();

		bool isOk = m_projectionProcessorCompPtr->DoProjection(*bitmapPtr, *m_arcCompPtr, NULL, result);
		if (!isOk){
			return WS_ERROR;
		}

		return WS_OK;
	}

	SendCriticalMessage(0, "Bad component archtecture. Bitmap provider or arc were not set");
	
	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CArcProjectionSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapSupplierCompPtr.GetPtr());
	}

	if (m_arcModelCompPtr.IsValid()){
		RegisterSupplierInput(m_arcModelCompPtr.GetPtr());
	}
}


// reimplemented (imeas::IDataSequenceProvider)

const imeas::IDataSequence* CArcProjectionSupplierComp::GetDataSequence() const
{
	return GetWorkProduct();
}


} // namespace iipr


