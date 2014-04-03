#include "icam/CMultiLayerBitmapSupplierComp.h"


// ACF includes
#include "imod/TModelWrap.h"
#include "iprm/TParamsPtr.h"


namespace icam
{


// reimplemented (iimg::IMultiBitmapProvider)

int CMultiLayerBitmapSupplierComp::GetBitmapsCount() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetPtr()->GetBitmapsCount();
	}

	return 0;
}


const iimg::IBitmap* CMultiLayerBitmapSupplierComp::GetBitmap(int bitmapIndex) const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetPtr()->GetBitmap(bitmapIndex);
	}

	return NULL;
}


const iprm::IOptionsList* CMultiLayerBitmapSupplierComp::GetBitmapListInfo() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetPtr()->GetBitmapListInfo();
	}

	return NULL;
}


// reimplemented (idoc::IMultiPageDocumentProvider)

const idoc::IMultiPageDocument* CMultiLayerBitmapSupplierComp::GetDocument() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return dynamic_cast<const idoc::IMultiPageDocument*>(productPtr->GetPtr());
	}

	return NULL;
}


// protected methods

// reimplemented (iinsp::TSupplierCompWrap)

bool CMultiLayerBitmapSupplierComp::InitializeWork()
{
	if (m_bitmapAcquisitionCompPtr.IsValid()){
		m_bitmapAcquisitionCompPtr->InitProcessor(GetModelParametersSet());

		return true;
	}

	return false;
}


int CMultiLayerBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_bitmapCompFact.IsValid()){
		return WS_CRITICAL;
	}

	result.Reset();

	if (!result.IsValid()){
		result.SetPtr(m_bitmapCompFact.CreateInstance());
		if (!result.IsValid()){
			return WS_CRITICAL;
		}
	}

	if (result.IsValid() && m_bitmapAcquisitionCompPtr.IsValid()){
		Timer performanceTimer(this, "Image acquisition");

		int status = m_bitmapAcquisitionCompPtr->DoProcessing(GetModelParametersSet(), NULL, result.GetPtr());
		switch (status){
			case iproc::IProcessor::TS_OK:
				return WS_OK;

			case iproc::IProcessor::TS_CANCELED:
				return WS_CANCELED;

			default:
				return WS_ERROR;
		}
	}

	return WS_CRITICAL;
}


} // namespace icam

