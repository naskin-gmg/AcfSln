#include <icam/CSnapMultiPageBitmapSupplierComp.h>


namespace icam
{


// reimplemented (iimg::IMultiBitmapProvider)

int CSnapMultiPageBitmapSupplierComp::GetBitmapsCount() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetPtr()->GetBitmapsCount();
	}

	return 0;
}


const iimg::IBitmap* CSnapMultiPageBitmapSupplierComp::GetBitmap(int bitmapIndex) const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetPtr()->GetBitmap(bitmapIndex);
	}

	return NULL;
}


const iprm::IOptionsList* CSnapMultiPageBitmapSupplierComp::GetBitmapListInfo() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetPtr()->GetBitmapListInfo();
	}

	return NULL;
}


// reimplemented (idoc::IMultiPageDocumentProvider)

const idoc::IMultiPageDocument* CSnapMultiPageBitmapSupplierComp::GetDocument() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return dynamic_cast<const idoc::IMultiPageDocument*>(productPtr->GetPtr());
	}

	return NULL;
}


// protected methods

// reimplemented (iinsp::TSupplierCompWrap)

bool CSnapMultiPageBitmapSupplierComp::InitializeWork()
{
	if (m_bitmapAcquisitionCompPtr.IsValid()){
		m_bitmapAcquisitionCompPtr->InitProcessor(GetModelParametersSet());

		return true;
	}

	return false;
}


int CSnapMultiPageBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	result.Reset();

	if (!m_bitmapCompFact.IsValid()){
		SendCriticalMessage(0, "Bad component architecture, 'BitmapFactory' component reference is not set");

		return WS_CRITICAL;
	}

	if (!m_bitmapAcquisitionCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component architecture, 'BitmapAcquisition' component reference is not set");

		return WS_CRITICAL;
	}

	if (!result.IsValid()){
		result.SetPtr(m_bitmapCompFact.CreateInstance());
		if (!result.IsValid()){
			SendErrorMessage(0, "Bitmap instance could not be created");

			return WS_ERROR;
		}
	}

	if (result.IsValid()){
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

