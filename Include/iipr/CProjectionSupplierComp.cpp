// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iipr/CProjectionSupplierComp.h>


namespace iipr
{


// public methods
	
// reimplemented (imeas::IDataSequenceProvider)

const imeas::IDataSequence* CProjectionSupplierComp::GetDataSequence() const
{
	return GetWorkProduct();
}


// protected methods

// reimplemented (iinsp::TSupplierCompWrap)

iinsp::ISupplier::WorkStatus CProjectionSupplierComp::ProduceObject(ProductType& result) const
{
	if (		m_bitmapProviderCompPtr.IsValid() &&
				m_projectionProcessorCompPtr.IsValid()){
		Timer performanceTimer(this, "Projection calculation");
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		
		int processingState = m_projectionProcessorCompPtr->DoProcessing(GetModelParametersSet(), bitmapPtr, &result);
		switch (processingState){
			case iproc::IProcessor::TS_OK:
				return WS_OK;
			case iproc::IProcessor::TS_CANCELED:
				return WS_CANCELED;
			default:
				return WS_FAILED;
		}
	}

	SendCriticalMessage(0, "Bad component archtecture. Bitmap provider or projection processor were not set");
	
	return WS_FAILED;
}


// reimplemented (icomp::CComponentBase)

void CProjectionSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapSupplierCompPtr.GetPtr());
	}
}


} // namespace iipr


