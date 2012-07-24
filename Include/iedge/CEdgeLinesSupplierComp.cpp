#include "iedge/CEdgeLinesSupplierComp.h"


namespace iedge
{


// reimplemented (iedge::IEdgeLinesProvider)

const CEdgeLine::Container* CEdgeLinesSupplierComp::GetEdgesContainer() const
{
	return GetWorkProduct();
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CEdgeLinesSupplierComp::ProduceObject(CEdgeLine::Container& result) const
{
	if (m_bitmapProviderCompPtr.IsValid() && edgesExtractorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (		(bitmapPtr != NULL) &&
					edgesExtractorCompPtr->DoContourExtraction(GetModelParametersSet(), *bitmapPtr, result)){
			return WS_OK;
		}
		else{
			return WS_ERROR;
		}
	}
	else{
		return WS_CRITICAL;
	}
}


// reimplemented (icomp::CComponentBase)

void CEdgeLinesSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr());
	}

	if (m_calibrationProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_calibrationProviderModelCompPtr.GetPtr());
	}
}


} // namespace iedge


