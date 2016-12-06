#include <iipr/CRectImageSmoothProcessorComp.h>


// ACF includes
#include <imath/CFixedPointManip.h>
#include <iprm/TParamsPtr.h>
#include <iimg/CGeneralBitmap.h>

// ACF-Solutions includes
#include <iipr/CPixelManip.h>


namespace iipr
{


// template function for several image modes

template <	typename InputPixelType,
			typename OutputPixelType,
			typename WorkingType,
			typename CounterType>
bool DoSimpleFilterHor(
			int kernelMaxWidth,
			iimg::IBitmap::PixelFormat outputPixelFormat,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage,
			CRectImageSmoothProcessorComp::BorderMode borderMode)
{
	Q_ASSERT(kernelMaxWidth >= 1);

	istd::CIndex2d imageSize = inputImage.GetImageSize();

	int imageWidth = imageSize.GetX();
	int imageHeight = imageSize.GetY();
	int outputImageWidth = (borderMode == CRectImageSmoothProcessorComp::BM_REDUCE_OUTPUT)? imageWidth - (kernelMaxWidth - 1): imageWidth;

	if (!outputImage.CreateBitmap(outputPixelFormat, istd::CIndex2d(outputImageWidth, imageHeight))){
		return false;
	}

	for (int y = 0; y < imageHeight; ++y){
		OutputPixelType* outputPtr = (OutputPixelType*)outputImage.GetLinePtr(y);

		const InputPixelType* inputLinePtr = (const InputPixelType*)inputImage.GetLinePtr(y);

		WorkingType meanValue(0);
		CounterType kernelWidth(0);
		while (kernelWidth < CounterType(kernelMaxWidth)){
			meanValue += inputLinePtr[kernelWidth++];

			if (kernelWidth >= CounterType(kernelMaxWidth)){
				break;
			}

			if (borderMode == CRectImageSmoothProcessorComp::BM_STRETCH_KERNEL){
				*outputPtr = OutputPixelType(meanValue / kernelWidth);

				outputPtr++;
			}

			meanValue += inputLinePtr[kernelWidth++];
		}

		int headX = int(kernelWidth);
		int tailX = 0;
		while (headX < imageWidth){
			*(outputPtr++) = OutputPixelType(meanValue / kernelWidth);

			meanValue += inputLinePtr[headX++];

			Q_ASSERT(tailX < imageWidth);
			meanValue -= inputLinePtr[tailX++];
		}

		if (borderMode == CRectImageSmoothProcessorComp::BM_STRETCH_KERNEL){
			while (kernelWidth > 0){
				*(outputPtr++) = OutputPixelType(meanValue / kernelWidth);

				Q_ASSERT(tailX < imageWidth);
				meanValue -= inputLinePtr[tailX++];
				kernelWidth--;

				if  (kernelWidth <= 0){
					break;
				}

				Q_ASSERT(tailX < imageWidth);
				meanValue -= inputLinePtr[tailX++];
				kernelWidth--;
			}
		}

		Q_ASSERT(outputPtr <= (OutputPixelType*)outputImage.GetLinePtr(y) + outputImageWidth);
	}

	return true;
}


template <	typename InputPixelType,
			typename OutputPixelType,
			typename WorkingType,
			typename CounterType>
bool DoSimpleFilterVer(
			int kernelMaxHeight,
			iimg::IBitmap::PixelFormat outputPixelFormat,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage,
			CRectImageSmoothProcessorComp::BorderMode borderMode)
{
	Q_ASSERT(kernelMaxHeight >= 1);

	istd::CIndex2d imageSize = inputImage.GetImageSize();

	int imageWidth = imageSize.GetX();
	int imageHeight = imageSize.GetY();
	int outputImageHeight = (borderMode == CRectImageSmoothProcessorComp::BM_REDUCE_OUTPUT)? imageHeight - (kernelMaxHeight - 1): imageHeight;

	if (!outputImage.CreateBitmap(outputPixelFormat, istd::CIndex2d(imageWidth, outputImageHeight))){
		return false;
	}

	int inputLinesDifference = inputImage.GetLinesDifference();
	int outputLinesDifference = outputImage.GetLinesDifference();

	for (int x = 0; x < imageWidth; ++x){	
		const InputPixelType* inputHeadPixelPtr = ((const InputPixelType*)inputImage.GetLinePtr(0)) + x;
		const InputPixelType* inputTailPixelPtr = inputHeadPixelPtr;
		OutputPixelType* outputPixelPtr = (OutputPixelType*)(outputImage.GetLinePtr(0)) + x;

		WorkingType meanValue(0);
		CounterType kernelHeight(0);
		while (kernelHeight < CounterType(kernelMaxHeight)){
			meanValue += *inputHeadPixelPtr;
			inputHeadPixelPtr = (const InputPixelType*)((const quint8*)inputHeadPixelPtr + inputLinesDifference);
			kernelHeight++;

			if (kernelHeight >= CounterType(kernelMaxHeight)){
				break;
			}

			if (borderMode == CRectImageSmoothProcessorComp::BM_STRETCH_KERNEL){
				*outputPixelPtr = OutputPixelType(meanValue / kernelHeight);
				outputPixelPtr = (OutputPixelType*)((quint8*)outputPixelPtr + outputLinesDifference);
			}

			meanValue += *inputHeadPixelPtr;
			inputHeadPixelPtr = (const InputPixelType*)((const quint8*)inputHeadPixelPtr + inputLinesDifference);
			kernelHeight++;
		}

		int headY = int(kernelHeight);

		for (;headY < imageHeight; ++headY){
			*outputPixelPtr = OutputPixelType(meanValue / kernelHeight);
				outputPixelPtr = (OutputPixelType*)((quint8*)outputPixelPtr + outputLinesDifference);

			meanValue += *inputHeadPixelPtr;
			inputHeadPixelPtr = (const InputPixelType*)((const quint8*)inputHeadPixelPtr + inputLinesDifference);

			meanValue -= *inputTailPixelPtr;
			inputTailPixelPtr = (const InputPixelType*)((const quint8*)inputTailPixelPtr + inputLinesDifference);
		}

		if (borderMode == CRectImageSmoothProcessorComp::BM_STRETCH_KERNEL){
			while (kernelHeight > 0){
				*outputPixelPtr = OutputPixelType(meanValue / kernelHeight);
				outputPixelPtr = (OutputPixelType*)((quint8*)outputPixelPtr + outputLinesDifference);

				meanValue -= *inputTailPixelPtr;
				inputTailPixelPtr = (const InputPixelType*)((const quint8*)inputTailPixelPtr + inputLinesDifference);
				--kernelHeight;

				if  (kernelHeight <= 0){
					break;
				}

				meanValue -= *inputTailPixelPtr;
				inputTailPixelPtr = (const InputPixelType*)((const quint8*)inputTailPixelPtr + inputLinesDifference);
				--kernelHeight;
			}
		}
	}

	return true;
}


// public static methods

bool CRectImageSmoothProcessorComp::DoRectFilter(
			int kernelMaxWidth,
			int kernelMaxHeight,
			iimg::IBitmap::PixelFormat outputPixelFormat,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage,
			BorderMode borderMode)
{
	if (kernelMaxWidth > 1){
		if (kernelMaxHeight > 1){
			iimg::CGeneralBitmap tempBitmap;

			return DoRectFilterHorizontal(kernelMaxWidth, outputPixelFormat, inputImage, tempBitmap, borderMode) && DoRectFilterVertical(kernelMaxHeight, outputPixelFormat, inputImage, outputImage, borderMode);
		}
		else{
			return DoRectFilterHorizontal(kernelMaxWidth, outputPixelFormat, inputImage, outputImage, borderMode);
		}
	}
	else{
		return DoRectFilterVertical(kernelMaxHeight, outputPixelFormat, inputImage, outputImage, borderMode);
	}
}


bool CRectImageSmoothProcessorComp::DoRectFilterHorizontal(
			int filterWidth,
			iimg::IBitmap::PixelFormat outputPixelFormat,
			const iimg::IBitmap& inputBitmap,
			iimg::IBitmap& outputBitmap,
			BorderMode borderMode)
{
	if (outputPixelFormat == iimg::IBitmap::PF_UNKNOWN){
		outputPixelFormat = inputBitmap.GetPixelFormat();
	}

	iimg::IBitmap::PixelFormat inputFormat = inputBitmap.GetPixelFormat();
	switch (inputFormat){
	case iimg::IBitmap::PF_GRAY:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoSimpleFilterHor<quint8, quint8, quint32, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_GRAY16:
			return DoSimpleFilterHor<quint8, quint16, quint32, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_GRAY32:
			return DoSimpleFilterHor<quint8, quint32, quint32, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterHor<quint8, float, float, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterHor<quint8, double, double, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_RGB:
	case iimg::IBitmap::PF_RGBA:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoSimpleFilterHor<CPixelManip::Rgba, quint8, CPixelManip::RgbCropAccum32<quint32, 0, false, true>, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_RGB:
			return DoSimpleFilterHor<CPixelManip::Rgba, quint8, CPixelManip::RgbCropAccum32<quint32, 0, false, true>, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_GRAY16:
			return DoSimpleFilterHor<CPixelManip::Rgba, quint16, CPixelManip::RgbCropAccum32<quint32, 0, false, true>, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_GRAY32:
			return DoSimpleFilterHor<CPixelManip::Rgba, quint32, CPixelManip::RgbCropAccum32<quint32, 0, false, false>, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterHor<CPixelManip::Rgba, float, CPixelManip::RgbCropAccum32<quint32, 0, false, false>, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterHor<CPixelManip::Rgba, double, CPixelManip::RgbCropAccum32<quint32, 0, false, false>, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_GRAY16:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY16:
			return DoSimpleFilterHor<quint16, quint16, quint32, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_GRAY32:
			return DoSimpleFilterHor<quint16, quint32, quint32, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterHor<quint16, float, float, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterHor<quint16, double, double, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_GRAY32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY32:
			return DoSimpleFilterHor<quint32, quint32, quint64, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterHor<quint32, float, double, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterHor<quint32, double, double, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterHor<float, float, double, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterHor<float, double, double, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT64:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterHor<double, float, double, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterHor<double, double, double, qint32>(qMax(filterWidth, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	default:
		break;
	}

	return false;
}


bool CRectImageSmoothProcessorComp::DoRectFilterVertical(
			int filterHeight,
			iimg::IBitmap::PixelFormat outputPixelFormat,
			const iimg::IBitmap& inputBitmap,
			iimg::IBitmap& outputBitmap,
			BorderMode borderMode)
{
	if (outputPixelFormat == iimg::IBitmap::PF_UNKNOWN){
		outputPixelFormat = inputBitmap.GetPixelFormat();
	}

	iimg::IBitmap::PixelFormat inputFormat = inputBitmap.GetPixelFormat();
	switch (inputFormat){
	case iimg::IBitmap::PF_GRAY:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoSimpleFilterVer<quint8, quint8, quint32, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_GRAY16:
			return DoSimpleFilterVer<quint8, quint16, quint32, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_GRAY32:
			return DoSimpleFilterVer<quint8, quint32, quint32, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterVer<quint8, float, float, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterVer<quint8, double, double, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_RGB:
	case iimg::IBitmap::PF_RGBA:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoSimpleFilterVer<CPixelManip::Rgba, quint8, CPixelManip::RgbCropAccum32<quint32, 0, false, true>, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_RGB:
			return DoSimpleFilterVer<CPixelManip::Rgba, quint8, CPixelManip::RgbCropAccum32<quint32, 0, false, true>, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_GRAY16:
			return DoSimpleFilterVer<CPixelManip::Rgba, quint16, CPixelManip::RgbCropAccum32<quint32, 0, false, true>, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_GRAY32:
			return DoSimpleFilterVer<CPixelManip::Rgba, quint32, CPixelManip::RgbCropAccum32<quint32, 0, false, false>, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterVer<CPixelManip::Rgba, float, CPixelManip::RgbCropAccum32<quint32, 0, false, false>, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterVer<CPixelManip::Rgba, double, CPixelManip::RgbCropAccum32<quint32, 0, false, false>, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_GRAY16:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY16:
			return DoSimpleFilterVer<quint16, quint16, quint32, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_GRAY32:
			return DoSimpleFilterVer<quint16, quint32, quint32, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterVer<quint16, float, float, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterVer<quint16, double, double, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_GRAY32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY32:
			return DoSimpleFilterVer<quint32, quint32, quint64, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterVer<quint32, float, double, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterVer<quint32, double, double, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterVer<float, float, double, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterVer<float, double, double, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT64:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_FLOAT32:
			return DoSimpleFilterVer<double, float, double, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		case iimg::IBitmap::PF_FLOAT64:
			return DoSimpleFilterVer<double, double, double, qint32>(qMax(filterHeight, 1), outputPixelFormat, inputBitmap, outputBitmap, borderMode);

		default:
			break;
		}
		break;

	default:
		break;
	}

	return false;
}


// protected methods

// reimplemented (iipr::CImageProcessorCompBase)

bool CRectImageSmoothProcessorComp::ProcessImage(
			const iprm::IParamsSet* paramsPtr, 
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	iprm::TParamsPtr<imeas::INumericValue> filterParamsPtr(paramsPtr, m_filterParamsIdAttrPtr, m_defaultFilterParamsCompPtr);	
	if (!filterParamsPtr.IsValid()){
		SendCriticalMessage(0, QObject::tr("No filter length for smoothing can be determined"));

		return false;
	}

	imath::CVarVector filterLengths = filterParamsPtr->GetValues();
	int filterDimensionsCount = filterLengths.GetElementsCount();
	if (filterDimensionsCount < 1){
		return false;
	}

	istd::CIndex2d imageSize = inputImage.GetImageSize();
	if (imageSize.IsSizeEmpty()){
		return false;
	}

	int imageWidth = imageSize.GetX();
	int imageHeight = imageSize.GetY();

	int kernelMaxWidth = 0;
	int kernelMaxHeight = 0;
	switch (*m_unitModeAttrPtr){
	case UM_PERCENT:
		kernelMaxWidth = qMax(1, qMin(int(filterLengths[0] * imageWidth), imageWidth));
		kernelMaxHeight = qMax(1, qMin(int((filterDimensionsCount < 2)? filterLengths[0]: filterLengths[1] * imageHeight), imageHeight));
		break;

	case UM_PERCENT_DIAG:
		{
			double diag = qSqrt(double(imageWidth * imageWidth + imageHeight * imageHeight));
			kernelMaxWidth = qMax(1, qMin(int(filterLengths[0] * diag), imageWidth));
			kernelMaxHeight = qMax(1, qMin((filterDimensionsCount < 2)? kernelMaxWidth: int(filterLengths[1] * diag), imageHeight));
		}
		break;

	default:
		kernelMaxWidth = qMax(1, qMin(int(filterLengths[0]), imageWidth));
		kernelMaxHeight = qMax(1, qMin((filterDimensionsCount < 2)? kernelMaxWidth: int(filterLengths[1]), imageHeight));
		break;
	}

	Q_ASSERT(kernelMaxWidth >= 1);
	Q_ASSERT(kernelMaxHeight >= 1);

	return DoRectFilter(
				kernelMaxWidth,
				kernelMaxHeight,
				GetOutputPixelFormat(paramsPtr),
				inputImage,
				outputImage,
				BorderMode(*m_borderModeAttrPtr));
}


// reimplemented (icomp::CComponentBase)

void CRectImageSmoothProcessorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_filterConstraints.Reset();

	static imath::CFixedPointManip pixelManip(0);
	static imath::CFixedPointManip percentManip(3);

	switch (*m_unitModeAttrPtr){
	case UM_PERCENT:
		{
			imath::CGeneralUnitInfo unitInfo(imath::IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(1, 1), &percentManip);

			m_filterConstraints.InsertValueInfo(QObject::tr("Filter width"), QObject::tr("Horizontal bluring filter length"), unitInfo);
			m_filterConstraints.InsertValueInfo(QObject::tr("Filter height"), QObject::tr("Vertical bluring filter length"), unitInfo);
		}
		break;

	case UM_PERCENT_DIAG:
		{
			imath::CGeneralUnitInfo unitInfo(imath::IUnitInfo::UT_TECHNICAL, "px", 1, istd::CRange(1, 100), &pixelManip);

			m_filterConstraints.InsertValueInfo(QObject::tr("Filter"), QObject::tr("Bluring filter length"), unitInfo);
		}
		break;

	default:
		{
			imath::CGeneralUnitInfo unitInfo(imath::IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(1, 1), &percentManip);

			m_filterConstraints.InsertValueInfo(QObject::tr("Filter width"), QObject::tr("Horizontal bluring filter length"), unitInfo);
			m_filterConstraints.InsertValueInfo(QObject::tr("Filter height"), QObject::tr("Vertical bluring filter length"), unitInfo);
		}
		break;
	}
}


} // namespace iipr


