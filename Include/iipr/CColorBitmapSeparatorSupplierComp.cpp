#include "iipr/CColorBitmapSeparatorSupplierComp.h"


namespace iipr
{


// reimplemented (iipr::IMultiBitmapProvider)

int CColorBitmapSeparatorSupplierComp::GetBitmapsCount() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		if (productPtr->IsEmpty()){
			return 1;
		}
		else{
			return productPtr->GetCount();
		}
	}

	return 0;
}


const iimg::IBitmap* CColorBitmapSeparatorSupplierComp::GetBitmap(int bitmapIndex) const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		if (productPtr->IsEmpty()){
			Q_ASSERT(bitmapIndex == 0);

			if (m_bitmapProviderCompPtr.IsValid()){
				return m_bitmapProviderCompPtr->GetBitmap();
			}
		}
		else{
			Q_ASSERT(bitmapIndex < productPtr->GetCount());

			return productPtr->GetElementAt(bitmapIndex);
		}
	}

	return NULL;
}


const i2d::ITransformation2d* CColorBitmapSeparatorSupplierComp::GetLogTransform(int /*bitmapIndex*/) const
{
	return m_calibrationCompPtr.GetPtr();
}


// reimplemented (iproc::TSupplierCompWrap)

int CColorBitmapSeparatorSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_bitmapProviderCompPtr.IsValid()){
		return WS_CRITICAL;
	}

	const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
	if (bitmapPtr == NULL){
		SendErrorMessage(0, "Input image could not be provided");

		return WS_ERROR;
	}

	result.Reset();	
	
	if (bitmapPtr->GetPixelFormat() != iimg::IBitmap::PF_RGB &&
		bitmapPtr->GetPixelFormat() != iimg::IBitmap::PF_RGBA)	{

			//There is only one chanel
			return WS_OK;
	}

	//Create chanels for RGB(A)
	istd::CIndex2d imageSize = bitmapPtr->GetImageSize();	

	//create red bitmap
	istd::TDelPtr<iimg::IBitmap> rOutputBitmapPtr(m_bitmapCompFact.CreateInstance());
	if (!rOutputBitmapPtr.IsValid()){
		return WS_ERROR;
	}

	if (!rOutputBitmapPtr->CreateBitmap(iimg::IBitmap::PF_GRAY, imageSize)){
		return WS_ERROR;
	}

	//create green bitmap
	istd::TDelPtr<iimg::IBitmap> gOutputBitmapPtr(m_bitmapCompFact.CreateInstance());
	if (!gOutputBitmapPtr.IsValid()){
		return WS_ERROR;
	}

	if (!gOutputBitmapPtr->CreateBitmap(iimg::IBitmap::PF_GRAY, imageSize)){
		return WS_ERROR;
	}

	//create blue bitmap
	istd::TDelPtr<iimg::IBitmap> bOutputBitmapPtr(m_bitmapCompFact.CreateInstance());
	if (!bOutputBitmapPtr.IsValid()){
		return WS_ERROR;
	}

	if (!bOutputBitmapPtr->CreateBitmap(iimg::IBitmap::PF_GRAY, imageSize)){
		return WS_ERROR;
	}

	//create alpha bitmap
	istd::TDelPtr<iimg::IBitmap> aOutputBitmapPtr;
	if (bitmapPtr->GetPixelFormat() == iimg::IBitmap::PF_RGBA){
		if (*m_useAlphaChannelAttrPtr){
			aOutputBitmapPtr.SetPtr(m_bitmapCompFact.CreateInstance());
			if (!aOutputBitmapPtr.IsValid()){
				return WS_ERROR;
			}

			if (!aOutputBitmapPtr->CreateBitmap(iimg::IBitmap::PF_GRAY, imageSize)){
				return WS_ERROR;
			}
		}
	}	

	int inputPixelComponentCount = bitmapPtr->GetComponentsCount();

	for (int y = 0; y < imageSize.GetY(); ++y){
		quint8* inputLinePtr = (quint8*)bitmapPtr->GetLinePtr(y);
		
		quint8* rOutputLinePtr = (quint8*)rOutputBitmapPtr->GetLinePtr(y);
		quint8* gOutputLinePtr = (quint8*)gOutputBitmapPtr->GetLinePtr(y);
		quint8* bOutputLinePtr = (quint8*)bOutputBitmapPtr->GetLinePtr(y);

		quint8* aOutputLinePtr = NULL;
		if (aOutputBitmapPtr.IsValid()){
			aOutputLinePtr = (quint8*)aOutputBitmapPtr->GetLinePtr(y);
		}

		for (int x = 0; x < imageSize.GetX(); ++x){
			quint8* pixelPtr = inputLinePtr + x * inputPixelComponentCount;

			rOutputLinePtr[x] = pixelPtr[0];
			gOutputLinePtr[x] = pixelPtr[1];
			bOutputLinePtr[x] = pixelPtr[2];

			if (aOutputLinePtr != NULL){
				aOutputLinePtr[x] = pixelPtr[3];
			}
		}
	}

	result.PushBack(rOutputBitmapPtr.PopPtr());
	result.PushBack(gOutputBitmapPtr.PopPtr());
	result.PushBack(bOutputBitmapPtr.PopPtr());

	if (aOutputBitmapPtr != NULL){
		result.PushBack(aOutputBitmapPtr.PopPtr());
	}

	return WS_OK;
}


// reimplemented (icomp::CComponentBase)

void CColorBitmapSeparatorSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr());
	}

	if (m_calibrationModelCompPtr.IsValid()){
		RegisterSupplierInput(m_calibrationModelCompPtr.GetPtr());
	}
}


} // namespace iipr


