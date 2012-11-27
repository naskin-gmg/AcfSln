#include "iipr/CDifferenceBitmapSupplierComp.h"


namespace iipr
{


// reimplemented (iipr::IBitmapProvider)

const iimg::IBitmap* CDifferenceBitmapSupplierComp::GetBitmap() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second.GetPtr();
	}

	return NULL;
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CDifferenceBitmapSupplierComp::GetCalibration() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->first;
	}
	else{
		return NULL;
	}
}


//protected methods

bool CDifferenceBitmapSupplierComp::EnsureBitmapCreated(ProductType& result) const
{
	if (!m_bitmapCompFact.IsValid()){
		return false;
	}

	if (!result.second.IsValid()){
		result.second.SetPtr(m_bitmapCompFact.CreateInstance());
	}

	return result.second.IsValid();
}


// reimplemented (iproc::TSupplierCompWrap)

int CDifferenceBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_firstBitmapProviderCompPtr.IsValid() || !m_secondBitmapProviderCompPtr.IsValid()){
		return WS_CRITICAL;
	}

	if (!EnsureBitmapCreated(result)){
		return WS_CRITICAL;
	}
	I_ASSERT(result.second.IsValid());

	result.first = NULL;

	const iimg::IBitmap* firstBitmapPtr = m_firstBitmapProviderCompPtr->GetBitmap();
	if (firstBitmapPtr == NULL){
		SendErrorMessage(0, "First input image could not be provided", "DifferenceBitmapSupplier");

		return WS_ERROR;
	}

	const iimg::IBitmap* secondBitmapPtr = m_secondBitmapProviderCompPtr->GetBitmap();
	if (secondBitmapPtr == NULL){
		SendErrorMessage(0, "Second input image could not be provided", "DifferenceBitmapSupplier");

		return WS_ERROR;
	}

	if (firstBitmapPtr->GetPixelFormat() != secondBitmapPtr->GetPixelFormat()){
		SendErrorMessage(0, "Input bitmaps have different formats. Difference bitmap calculation canceled", "DifferenceBitmapSupplier");

		return WS_ERROR;
	}

	Timer performanceTimer(this, "Image difference");

	return CalculateDifferenceBitmap(*firstBitmapPtr, *secondBitmapPtr, result);
}


// reimplemented (icomp::CComponentBase)

void CDifferenceBitmapSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_firstBitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_firstBitmapProviderModelCompPtr.GetPtr(), m_firstBitmapSupplierCompPtr.GetPtr());
	}

	if (m_secondBitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_secondBitmapProviderModelCompPtr.GetPtr(), m_secondBitmapSupplierCompPtr.GetPtr());
	}
}


// private methods

int CDifferenceBitmapSupplierComp::CalculateDifferenceBitmap(
			const iimg::IBitmap& firstInputBitmap,
			const iimg::IBitmap& secondInputBitmap,
			ProductType& result) const
{
	istd::CIndex2d firstImageSize = firstInputBitmap.GetImageSize();
	istd::CIndex2d secondImageSize = secondInputBitmap.GetImageSize();

	istd::CIndex2d outputImageSize = istd::CIndex2d(qMax(firstImageSize.GetX(), secondImageSize.GetX()), qMax(firstImageSize.GetY(), secondImageSize.GetY()));
	result.second->CreateBitmap(firstInputBitmap.GetPixelFormat(), outputImageSize);

	int componentsCount = firstInputBitmap.GetComponentsCount();

	for (int componentIndex = 0; componentIndex < componentsCount; componentIndex++){
		for (int y = 0; y < outputImageSize.GetY(); ++y){
			const quint8* firstLinePtr = (const quint8*)firstInputBitmap.GetLinePtr(y);
			const quint8* secondLinePtr = (const quint8*)secondInputBitmap.GetLinePtr(y);
			quint8* outputLinePtr = (quint8*)result.second->GetLinePtr(y);

			for (int x = 0; x < outputImageSize.GetX(); ++x){
				int pixelComponentIndex = x * componentsCount + componentIndex;

				quint8 firstValue = firstLinePtr[pixelComponentIndex];
				quint8 secondValue = secondLinePtr[pixelComponentIndex];

				outputLinePtr[pixelComponentIndex] = qAbs(firstValue - secondValue);
			}
		}
	}

	return WS_OK;
}


} // namespace iipr


