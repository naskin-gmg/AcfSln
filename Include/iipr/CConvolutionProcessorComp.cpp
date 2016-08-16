#include "iipr/CConvolutionProcessorComp.h"


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "iprm/TParamsPtr.h"
#include "iimg/CScanlineMask.h"

// ACF-Solutions includes
#include "iipr/CPixelManip.h"
#include "iipr/CConvolutionKernel2d.h"


namespace iipr
{


/**
	Template convolution funtion.
*/
template <	typename InputPixelType,
			typename OutputPixelType,
			typename WorkingType>
bool DoFastConvolution(
			const iimg::IBitmap& inputImage,
			const IConvolutionKernel2d& kernel,
			const iimg::CScanlineMask& resultMask,
			iimg::IBitmap& outputImage)
{
	istd::CIndex2d kernelSize = kernel.GetKernelSize();
	Q_ASSERT(!kernelSize.IsSizeEmpty());

	int linesDifference = inputImage.GetLinesDifference();
	int pixelsDifference = inputImage.GetPixelsDifference();

	QVector< QPair<int, WorkingType> > fastAccessElements;

	int halfKernelWidth = kernelSize.GetX() / 2;
	int halfKernelHeight = kernelSize.GetY() / 2;

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			double value = kernel.GetKernelElement(index);

			QPair<int, WorkingType> element;
			element.first = (index[0] - halfKernelWidth) * pixelsDifference + (index[1] - halfKernelHeight) * linesDifference;
			element.second = value;

			if (element.second != 0){
				fastAccessElements.push_back(element);
			}
		}
	}

	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	istd::CIndex2d outputImageSize = inputImageSize;

	if (outputImageSize.IsSizeEmpty()){
		return false;
	}

	double offsetValue = kernel.GetOffsetValue();

	WorkingType initialSum = WorkingType(offsetValue);

	int kernelElementsCount = int(fastAccessElements.size());

	istd::CIntRange outputRangeH(halfKernelWidth, outputImageSize.GetX() - halfKernelWidth);

	#pragma omp parallel for

	for (int y = 0; y < outputImageSize.GetY(); ++y){
		OutputPixelType* outputPtr = static_cast<OutputPixelType*>(outputImage.GetLinePtr(y));

		const istd::CIntRanges* outputRangesPtr = ((y >= halfKernelHeight) && (y < outputImageSize.GetY()))? resultMask.GetPixelRanges(y): NULL;
		if (outputRangesPtr != NULL){
			const InputPixelType* inputPtr = static_cast<const InputPixelType*>(inputImage.GetLinePtr(y));

			istd::CIntRanges::RangeList rangeList;
			outputRangesPtr->GetAsList(outputRangeH, rangeList);

			int x = 0;
			for (		istd::CIntRanges::RangeList::ConstIterator iter = rangeList.constBegin();
						iter != rangeList.constEnd();
						++iter){
				const istd::CIntRange& rangeH = *iter;

				if (rangeH.GetMinValue() > x){
					std::memset(outputPtr + x, 0, (rangeH.GetMinValue() - x) * sizeof(OutputPixelType));

					x = rangeH.GetMinValue();
				}

				for (; x < rangeH.GetMaxValue(); ++x){
					WorkingType sum = initialSum;

					for (int i = 0; i < kernelElementsCount; ++i){
						const QPair<int, WorkingType>& kernelElement = fastAccessElements[i];

						WorkingType inputValue = *(const InputPixelType*)((quint8*)&inputPtr[x] + kernelElement.first);
						inputValue *= kernelElement.second;

						sum += inputValue;
					}

					outputPtr[x] = sum;
				}
			}

			if (outputImageSize.GetX() > x){
				std::memset(outputPtr + x, 0, (outputImageSize.GetX() - x) * sizeof(OutputPixelType));
			}
		}
		else{
			std::memset(outputPtr, 0, outputImageSize.GetX() * sizeof(OutputPixelType));
		}
	}

	return true;
}


// reimplemented (iipr::TImageParamProcessorCompBase<ParameterType>)

bool CConvolutionProcessorComp::ParamProcessImage(
			const iprm::IParamsSet* paramsPtr,
			const IConvolutionKernel2d* procParamPtr,
			iimg::IBitmap::PixelFormat outputPixelFormat,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (procParamPtr == NULL){
		SendErrorMessage(0, "Processing parameters are not set");		
		
		return false;
	}

	iimg::IBitmap::PixelFormat inputPixelFormat = inputImage.GetPixelFormat();
	if (outputPixelFormat <= iimg::IBitmap::PF_UNKNOWN){
		outputPixelFormat = inputPixelFormat;
	}

	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	if (inputImageSize != outputImage.GetImageSize() || outputPixelFormat != outputImage.GetPixelFormat()){
		if (!outputImage.CreateBitmap(iimg::IBitmap::PixelFormat(outputPixelFormat), inputImageSize)){
			SendErrorMessage(0, "Output image could not be created");

			return false;
		}
	}

	istd::CIndex2d kernelSize = procParamPtr->GetKernelSize();
	if (kernelSize.IsSizeEmpty()){
		SendErrorMessage(0, "Kernel size to small");

		return false;
	}

	if (inputImageSize.IsInside(kernelSize)){
		SendErrorMessage(0, "Kernel size to big");

		return false;
	}

	// create output image mask
	i2d::CRect inputClipArea(inputImageSize);
	i2d::CRect resultClipArea(kernelSize.GetX() / 2, kernelSize.GetY() / 2, inputImageSize.GetX() - kernelSize.GetX() / 2, inputImageSize.GetY() - kernelSize.GetY() / 2);

	iimg::CScanlineMask resultMask;
	iprm::TParamsPtr<i2d::IObject2d> aoiObjectPtr(paramsPtr, m_aoiParamIdAttrPtr, m_defaultAoiCompPtr, false);
	if (aoiObjectPtr.IsValid()){
		resultMask.SetCalibration(inputImage.GetCalibration());

		if (!resultMask.CreateFromGeometry(*aoiObjectPtr.GetPtr(), (*m_aoiModeAttrPtr == AM_INPUT_PIXELS)? &inputClipArea: &resultClipArea)){
			SendErrorMessage(0, QObject::tr("AOI type is not supported"));
		}

		if (*m_aoiModeAttrPtr == AM_INPUT_PIXELS){
			resultMask.Erode(kernelSize.GetX() / 2, kernelSize.GetX() / 2, kernelSize.GetY() / 2, kernelSize.GetY() / 2);
		}
	}
	else{
		resultMask.CreateFilled(resultClipArea);
	}

	double kernelPosSum = 0.0;
	double kernelSum = 0.0;

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			double kernelElement = procParamPtr->GetKernelElement(index);

			kernelSum += kernelElement;
			kernelPosSum += qFabs(kernelElement);
		}
	}

	iipr::CConvolutionKernel2d normalizedKernel;
	if (*m_normalizeKernelAttrPtr && (kernelPosSum > I_EPSILON)){
		normalizedKernel.SetKernelSize(kernelSize);

		for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
			for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
				double kernelElement = procParamPtr->GetKernelElement(index);

				normalizedKernel.SetKernelElement(index, kernelElement / kernelPosSum);
			}
		}

		procParamPtr = &normalizedKernel;
	}

	bool hasNegativeKernel = kernelSum != kernelPosSum;

	switch (inputPixelFormat){
	case iimg::IBitmap::PF_GRAY:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			if (hasNegativeKernel){
				if (kernelPosSum > 1){
					return DoFastConvolution<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 16, true, false> >(inputImage, *procParamPtr, resultMask, outputImage);
				}
				else{
					return DoFastConvolution<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 23, true, false> >(inputImage, *procParamPtr, resultMask, outputImage);
				}
			}
			else{
				if (kernelPosSum > 1){
					return DoFastConvolution<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 16, false, true> >(inputImage, *procParamPtr, resultMask, outputImage);
				}
				else{
					return DoFastConvolution<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<quint32, 24, false, false> >(inputImage, *procParamPtr, resultMask, outputImage);
				}
			}

		case iimg::IBitmap::PF_RGB:
		case iimg::IBitmap::PF_RGBA:
			return DoFastConvolution<quint8, iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbCropAccum32<qint32, 16, true, true> >(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoFastConvolution<quint8, quint16, iipr::CPixelManip::GrayCropAccum32<qint32, 8, true, false> >(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_GRAY32:
			return DoFastConvolution<quint8, quint32, iipr::CPixelManip::GrayCropAccum32<qint64, 16, true, false> >(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<quint8, float, double>(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<quint8, double, double>(inputImage, *procParamPtr, resultMask, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_RGB:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoFastConvolution<iipr::CPixelManip::Rgba, quint8, iipr::CPixelManip::RgbCropAccum32<qint32, 22, true, true> >(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_RGB:
		case iimg::IBitmap::PF_RGBA:
			return DoFastConvolution<iipr::CPixelManip::Rgba, iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbCropAccum32<qint32, 22, true, true> >(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoFastConvolution<iipr::CPixelManip::Rgba, quint16, iipr::CPixelManip::RgbCropAccum32<qint32, 14, true, false> >(inputImage, *procParamPtr, resultMask, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_RGBA:
		if (outputPixelFormat == iimg::IBitmap::PF_RGBA){
			return DoFastConvolution<iipr::CPixelManip::Rgba, iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbaCropAccum32<qint32, 22, true, true> >(inputImage, *procParamPtr, resultMask, outputImage);
		}
		break;

	case iimg::IBitmap::PF_GRAY16:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoFastConvolution<quint16, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 8, true, true> >(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoFastConvolution<quint16, quint16, iipr::CPixelManip::GrayCropAccum32<qint32, 8, true, true> >(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_GRAY32:
			return DoFastConvolution<quint16, quint32, iipr::CPixelManip::GrayCropAccum32<qint64, 16, true, false> >(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<quint16, float, double>(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<quint16, double, double>(inputImage, *procParamPtr, resultMask, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_GRAY32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY32:
			return DoFastConvolution<quint32, quint32, iipr::CPixelManip::GrayCropAccum32<qint64, 16, true, true>>(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<quint32, float, double>(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<quint32, double, double>(inputImage, *procParamPtr, resultMask, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<float, float, double>(inputImage, *procParamPtr, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<float, double, double>(inputImage, *procParamPtr, resultMask, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT64:
		return DoFastConvolution<double, double, double>(inputImage, *procParamPtr, resultMask, outputImage);

	default:
		SendErrorMessage(0, QObject::tr("Input image format '%1' not supported").arg(m_formatList.GetOptionName(inputPixelFormat)));

		return false;
	}

	SendErrorMessage(0, QObject::tr("Cannot produce output image in format '%1' from input format '%2'").arg(m_formatList.GetOptionName(outputPixelFormat)).arg(m_formatList.GetOptionName(inputPixelFormat)));

	return false;
}


} // namespace iipr


