#include <iipr/CDifferenceBitmapSupplierComp.h>


// ACF includes
#include <imath/CFixedPointManip.h>
#include <iprm/TParamsPtr.h>
#include <iimg/CPixelFormatList.h>


namespace iipr
{


// template functions

template <	typename FirstPixelType,
			typename SecondPixelType,
			typename OutputPixelType,
			typename WorkingType>
void CalculateDifferenceBitmap(
			WorkingType offset,
			const iimg::IBitmap& firstInputBitmap,
			const iimg::IBitmap& secondInputBitmap,
			iimg::IBitmap& result)
{
	istd::CIndex2d outputImageSize = result.GetImageSize();

	int componentsCount = firstInputBitmap.GetComponentsCount();

	for (int componentIndex = 0; componentIndex < componentsCount; componentIndex++){
		for (int y = 0; y < outputImageSize.GetY(); ++y){
			const FirstPixelType* firstLinePtr = (const FirstPixelType*)firstInputBitmap.GetLinePtr(y);
			const SecondPixelType* secondLinePtr = (const SecondPixelType*)secondInputBitmap.GetLinePtr(y);
			OutputPixelType* outputLinePtr = (OutputPixelType*)result.GetLinePtr(y);

			for (int x = 0; x < outputImageSize.GetX(); ++x){
				int pixelComponentIndex = x * componentsCount + componentIndex;

				WorkingType firstValue = firstLinePtr[pixelComponentIndex];
				WorkingType secondValue = secondLinePtr[pixelComponentIndex];

				outputLinePtr[pixelComponentIndex] = qAbs(offset + firstValue - secondValue);
			}
		}
	}
}


CDifferenceBitmapSupplierComp::CDifferenceBitmapSupplierComp()
{
	static imath::CFixedPointManip percentManip(3);

	imath::CGeneralUnitInfo unitInfo(imath::IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(0, 1), &percentManip);

	m_OffsetContraints.InsertValueInfo(QObject::tr("Offset"), QObject::tr("Offset added to each pixel as percentage"), unitInfo);
}


// reimplemented (iimg::IBitmapProvider)

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


// reimplemented (iinsp::TSupplierCompWrap)

int CDifferenceBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_firstBitmapProviderCompPtr.IsValid() || !m_secondBitmapProviderCompPtr.IsValid()){
		return WS_CRITICAL;
	}

	if (!EnsureBitmapCreated(result)){
		return WS_CRITICAL;
	}
	Q_ASSERT(result.second.IsValid());

	result.first = NULL;

	const iimg::IBitmap* firstBitmapPtr = m_firstBitmapProviderCompPtr->GetBitmap();
	if (firstBitmapPtr == NULL){
		AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QObject::tr("First input image could not be provided"), "DifferenceBitmapSupplier"));

		return WS_ERROR;
	}

	const iimg::IBitmap* secondBitmapPtr = m_secondBitmapProviderCompPtr->GetBitmap();
	if (secondBitmapPtr == NULL){
		AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QObject::tr("Second input image could not be provided"), "DifferenceBitmapSupplier"));

		return WS_ERROR;
	}

	iimg::IBitmap::PixelFormat pixelFormat = firstBitmapPtr->GetPixelFormat();

	if (pixelFormat != secondBitmapPtr->GetPixelFormat()){
		AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QObject::tr("Format of input images differs"), "DifferenceBitmapSupplier"));

		return WS_ERROR;
	}

	Timer performanceTimer(this, "Image difference");

	istd::CIndex2d firstImageSize = firstBitmapPtr->GetImageSize();
	istd::CIndex2d secondImageSize = secondBitmapPtr->GetImageSize();

	istd::CIndex2d outputImageSize = istd::CIndex2d(qMin(firstImageSize.GetX(), secondImageSize.GetX()), qMin(firstImageSize.GetY(), secondImageSize.GetY()));
	if (result.second->CreateBitmap(pixelFormat, outputImageSize)){
	}

	double offset = 0;
	iprm::TParamsPtr<imeas::INumericValue> thresholdValuePtr(GetModelParametersSet(), m_offsetParamIdAttrPtr, m_defaultOffsetCompPtr, false);
	if (thresholdValuePtr.IsValid()){
		imath::CVarVector values = thresholdValuePtr->GetValues();
		if (values.GetElementsCount() >= 1){
			offset = values[0];
		}
	}

	switch (pixelFormat){
	case iimg::IBitmap::PF_GRAY:
		CalculateDifferenceBitmap<quint8, quint8, quint8, int>(quint8(offset * 255), *firstBitmapPtr, *secondBitmapPtr, *result.second);
		return WS_OK;

	case iimg::IBitmap::PF_GRAY16:
		CalculateDifferenceBitmap<quint16, quint16, quint16, int>(quint16(offset * 255), *firstBitmapPtr, *secondBitmapPtr, *result.second);
		return WS_OK;

	case iimg::IBitmap::PF_GRAY32:
		CalculateDifferenceBitmap<quint32, quint32, quint32, int>(quint16(offset * 255), *firstBitmapPtr, *secondBitmapPtr, *result.second);
		return WS_OK;

	case iimg::IBitmap::PF_FLOAT32:
		CalculateDifferenceBitmap<float, float, float, float>(float(offset), *firstBitmapPtr, *secondBitmapPtr, *result.second);
		return WS_OK;

	case iimg::IBitmap::PF_FLOAT64:
		CalculateDifferenceBitmap<double, double, double, double>(offset, *firstBitmapPtr, *secondBitmapPtr, *result.second);
		return WS_OK;

	default:
		SendErrorMessage(
					iproc::IProcessor::MI_BAD_PARAMS,
					QObject::tr("Input image format '%1' not supported").arg(iimg::CPixelFormatList::GetInstance().GetOptionName(pixelFormat)),
					QObject::tr("BitmapDifference"));
		return WS_CRITICAL;
	}
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


} // namespace iipr


