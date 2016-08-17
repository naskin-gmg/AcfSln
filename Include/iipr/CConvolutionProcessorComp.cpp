#include "iipr/CConvolutionProcessorComp.h"


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "iprm/TParamsPtr.h"
#include "iimg/CPixelFormatList.h"

// ACF-Solutions includes
#include "iipr/CPixelManip.h"
#include "iipr/CConvolutionKernel2d.h"


namespace iipr
{


/**
	Template filter funtion.
*/
template <	typename InputPixelType,
			typename OutputPixelType,
			typename WorkingType>
bool DoConvolutionTemplate(
			const IConvolutionKernel2d& kernel,
			int backgroundMode,
			const iimg::IBitmap& inputImage,
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

	i2d::CRect outputMaxRect(halfKernelWidth, halfKernelHeight, outputImageSize.GetX() - halfKernelWidth, outputImageSize.GetY() - halfKernelHeight);

	#pragma omp parallel for

	for (int y = 0; y < outputImageSize.GetY(); ++y){
		OutputPixelType* outputLinePtr = static_cast<OutputPixelType*>(outputImage.GetLinePtr(y));

		const istd::CIntRanges* outputRangesPtr = outputMaxRect.GetVerticalRange().Contains(y)? resultMask.GetPixelRanges(y): NULL;
		if (outputRangesPtr != NULL){
			const InputPixelType* inputLinePtr = static_cast<const InputPixelType*>(inputImage.GetLinePtr(y));

			istd::CIntRanges::RangeList rangeList;
			outputRangesPtr->GetAsList(outputMaxRect.GetHorizontalRange(), rangeList);

			int x = 0;
			for (		istd::CIntRanges::RangeList::ConstIterator iter = rangeList.constBegin();
						iter != rangeList.constEnd();
						++iter){
				const istd::CIntRange& rangeH = *iter;
				Q_ASSERT(rangeH.GetMinValue() >= halfKernelWidth);
				Q_ASSERT(rangeH.GetMaxValue() <= outputImageSize.GetX() - halfKernelWidth);

				if (rangeH.GetMinValue() > x){
					if (backgroundMode == CImageProcessorCompBase::BFM_RESET){
						std::memset(outputLinePtr + x, 0, (rangeH.GetMinValue() - x) * sizeof(OutputPixelType));
					}
					else if (backgroundMode == CImageProcessorCompBase::BFM_INPUT){
						for (; x < rangeH.GetMinValue(); ++x){
							WorkingType inputValue = inputLinePtr[x];
							outputLinePtr[x] = inputValue;
						}
					}

					x = rangeH.GetMinValue();
				}

				for (; x < rangeH.GetMaxValue(); ++x){
					WorkingType sum = initialSum;

					for (int i = 0; i < kernelElementsCount; ++i){
						const QPair<int, WorkingType>& kernelElement = fastAccessElements[i];

						WorkingType inputValue = *(const InputPixelType*)((quint8*)&inputLinePtr[x] + kernelElement.first);
						inputValue *= kernelElement.second;

						sum += inputValue;
					}

					outputLinePtr[x] = sum;
				}
			}

			if (outputImageSize.GetX() > x){
				if (backgroundMode == CImageProcessorCompBase::BFM_RESET){
					std::memset(outputLinePtr + x, 0, (outputImageSize.GetX() - x) * sizeof(OutputPixelType));
				}
				else if (backgroundMode == CImageProcessorCompBase::BFM_INPUT){
					for (; x < outputImageSize.GetX(); ++x){
						WorkingType inputValue = inputLinePtr[x];
						outputLinePtr[x] = inputValue;
					}
				}
			}
		}
		else{
			if (backgroundMode == CImageProcessorCompBase::BFM_RESET){
				std::memset(outputLinePtr, 0, outputImageSize.GetX() * sizeof(OutputPixelType));
			}
			else if (backgroundMode == CImageProcessorCompBase::BFM_INPUT){
				const InputPixelType* inputLinePtr = static_cast<const InputPixelType*>(inputImage.GetLinePtr(y));

				for (int x = 0; x < outputImageSize.GetX(); ++x){
					WorkingType inputValue = inputLinePtr[x];
					outputLinePtr[x] = inputValue;
				}
			}
		}
	}

	return true;
}


// static public methods

bool CConvolutionProcessorComp::DoConvolutionFilter(
			const IConvolutionKernel2d& kernel,
			int backgroundMode,
			iimg::IBitmap::PixelFormat outputPixelFormat,
			const iimg::IBitmap& inputImage,
			const iimg::CScanlineMask& resultMask,
			iimg::IBitmap& outputImage,
			istd::ILogger* loggerPtr)
{
	iimg::IBitmap::PixelFormat inputPixelFormat = inputImage.GetPixelFormat();
	if (outputPixelFormat <= iimg::IBitmap::PF_UNKNOWN){
		outputPixelFormat = inputPixelFormat;
	}

	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	if (inputImageSize != outputImage.GetImageSize() || outputPixelFormat != outputImage.GetPixelFormat()){
		if (!outputImage.CreateBitmap(iimg::IBitmap::PixelFormat(outputPixelFormat), inputImageSize)){
			if (loggerPtr != NULL){
				loggerPtr->SendLogMessage(
							istd::IInformationProvider::IC_ERROR,
							0,
							QObject::tr("Output image could not be created"),
							QObject::tr("ConvolutionProcessor"));
			}

			return false;
		}
	}

	istd::CIndex2d kernelSize = kernel.GetKernelSize();
	if (kernelSize.IsSizeEmpty()){
		if (loggerPtr != NULL){
			loggerPtr->SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						0,
						QObject::tr("Kernel size to small"),
						QObject::tr("ConvolutionProcessor"));
		}

		return false;
	}

	if (inputImageSize.IsInside(kernelSize)){
		if (loggerPtr != NULL){
			loggerPtr->SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						0,
						QObject::tr("Kernel size to big"),
						QObject::tr("ConvolutionProcessor"));
		}

		return false;
	}

	double kernelPosSum = 0.0;
	double kernelSum = 0.0;

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			double kernelElement = kernel.GetKernelElement(index);

			kernelSum += kernelElement;
			kernelPosSum += qFabs(kernelElement);
		}
	}

	bool hasNegativeKernel = kernelSum != kernelPosSum;

	switch (inputPixelFormat){
	case iimg::IBitmap::PF_GRAY:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			if (hasNegativeKernel){
				if (kernelPosSum > 1){
					return DoConvolutionTemplate<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 16, true, false> >(kernel, backgroundMode, inputImage, resultMask, outputImage);
				}
				else{
					return DoConvolutionTemplate<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 23, true, false> >(kernel, backgroundMode, inputImage, resultMask, outputImage);
				}
			}
			else{
				if (kernelPosSum > 1){
					return DoConvolutionTemplate<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 16, false, true> >(kernel, backgroundMode, inputImage, resultMask, outputImage);
				}
				else{
					return DoConvolutionTemplate<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<quint32, 24, false, false> >(kernel, backgroundMode, inputImage, resultMask, outputImage);
				}
			}

		case iimg::IBitmap::PF_RGB:
		case iimg::IBitmap::PF_RGBA:
			return DoConvolutionTemplate<quint8, iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbCropAccum32<qint32, 16, true, true> >(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoConvolutionTemplate<quint8, quint16, iipr::CPixelManip::GrayCropAccum32<qint32, 8, true, false> >(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_GRAY32:
			return DoConvolutionTemplate<quint8, quint32, iipr::CPixelManip::GrayCropAccum32<qint64, 16, true, false> >(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoConvolutionTemplate<quint8, float, double>(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoConvolutionTemplate<quint8, double, double>(kernel, backgroundMode, inputImage, resultMask, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_RGB:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoConvolutionTemplate<iipr::CPixelManip::Rgba, quint8, iipr::CPixelManip::RgbCropAccum32<qint32, 22, true, true> >(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_RGB:
		case iimg::IBitmap::PF_RGBA:
			return DoConvolutionTemplate<iipr::CPixelManip::Rgba, iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbCropAccum32<qint32, 22, true, true> >(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoConvolutionTemplate<iipr::CPixelManip::Rgba, quint16, iipr::CPixelManip::RgbCropAccum32<qint32, 14, true, false> >(kernel, backgroundMode, inputImage, resultMask, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_RGBA:
		if (outputPixelFormat == iimg::IBitmap::PF_RGBA){
			return DoConvolutionTemplate<iipr::CPixelManip::Rgba, iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbaCropAccum32<qint32, 22, true, true> >(kernel, backgroundMode, inputImage, resultMask, outputImage);
		}
		break;

	case iimg::IBitmap::PF_GRAY16:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoConvolutionTemplate<quint16, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 8, true, true> >(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoConvolutionTemplate<quint16, quint16, iipr::CPixelManip::GrayCropAccum32<qint32, 8, true, true> >(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_GRAY32:
			return DoConvolutionTemplate<quint16, quint32, iipr::CPixelManip::GrayCropAccum32<qint64, 16, true, false> >(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoConvolutionTemplate<quint16, float, double>(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoConvolutionTemplate<quint16, double, double>(kernel, backgroundMode, inputImage, resultMask, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_GRAY32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY32:
			return DoConvolutionTemplate<quint32, quint32, iipr::CPixelManip::GrayCropAccum32<qint64, 16, true, true>>(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoConvolutionTemplate<quint32, float, double>(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoConvolutionTemplate<quint32, double, double>(kernel, backgroundMode, inputImage, resultMask, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_FLOAT32:
			return DoConvolutionTemplate<float, float, double>(kernel, backgroundMode, inputImage, resultMask, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoConvolutionTemplate<float, double, double>(kernel, backgroundMode, inputImage, resultMask, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT64:
		return DoConvolutionTemplate<double, double, double>(kernel, backgroundMode, inputImage, resultMask, outputImage);

	default:
		if (loggerPtr != NULL){
			loggerPtr->SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						0,
						QObject::tr("Input image format '%1' not supported").arg(iimg::CPixelFormatList::GetInstance().GetOptionName(inputPixelFormat)),
						QObject::tr("ConvolutionProcessor"));
		}
		return false;
	}

	if (loggerPtr != NULL){
		const iimg::CPixelFormatList& formatList = iimg::CPixelFormatList::GetInstance();
		loggerPtr->SendLogMessage(
					istd::IInformationProvider::IC_ERROR,
					0,
					QObject::tr("Cannot produce output image in format '%1' from input format '%2'").arg(formatList.GetOptionName(outputPixelFormat)).arg(formatList.GetOptionName(inputPixelFormat)),
					QObject::tr("ConvolutionProcessor"));
	}

	return false;
}


// protected methods

// reimplemented (iipr::TImageParamProcessorCompBase<ParameterType>)

bool CConvolutionProcessorComp::ParamProcessImage(
			const iprm::IParamsSet* paramsPtr,
			const IConvolutionKernel2d* procParamPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (procParamPtr == NULL){
		SendErrorMessage(0, "Processing parameters are not set");		
		
		return false;
	}

	// create output image mask
	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	i2d::CRect inputClipArea(inputImageSize);

	iimg::CScanlineMask resultMask;
	iprm::TParamsPtr<i2d::IObject2d> aoiObjectPtr(paramsPtr, m_aoiParamIdAttrPtr, m_defaultAoiCompPtr, false);
	if (aoiObjectPtr.IsValid()){
		resultMask.SetCalibration(inputImage.GetCalibration());

		if (!resultMask.CreateFromGeometry(*aoiObjectPtr.GetPtr(), &inputClipArea)){
			SendErrorMessage(0, QObject::tr("AOI type is not supported"));

			return false;
		}

		AoiMode aoiMode = GetAoiMode(paramsPtr);
		if (aoiMode == AM_INPUT_PIXELS){
			istd::CIndex2d kernelSize = procParamPtr->GetKernelSize();
			resultMask.Erode(kernelSize.GetX() / 2, kernelSize.GetX() / 2, kernelSize.GetY() / 2, kernelSize.GetY() / 2);
		}
	}
	else{
		resultMask.CreateFilled(inputClipArea);
	}

	return DoConvolutionFilter(
				*procParamPtr,
				GetBackgroundMode(paramsPtr),
				GetOutputPixelFormat(paramsPtr),
				inputImage,
				resultMask,
				outputImage,
				this);
}


} // namespace iipr


 