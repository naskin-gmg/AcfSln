#include <iblob/CPreciseBlobProcessorComp.h>


// STL includes
#include <cstring>

// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <istd/TRange.h>
#include <iprm/TParamsPtr.h>
#include <iimg/CPixelFormatList.h>
#include <iimg/CGeneralBitmap.h>

// ACF-Solutions includes
#include <iipr/CPixelManip.h>
#include <iipr/CConvolutionKernel2d.h>
#include <iipr/CImageCopyProcessorComp.h>


namespace iblob
{


/**
	Template filter funtion.
*/
template <	typename InputPixelType,
			typename WorkingType>
bool DoCalculateBlobsTemplate(
			const iblob::IBlobFilterParams* filterParamsPtr,
			const iimg::CScanlineMask& imageMask,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& result)
{
	istd::CIndex2d imageSize = image.GetImageSize();

	istd::CIntRange lineRange(0, imageSize.GetX());

	for (int y = 0; y < imageSize.GetY(); ++y){
		const istd::CIntRanges* outputRangesPtr = imageMask.GetPixelRanges(y);
		if (outputRangesPtr != NULL){
			const InputPixelType* inputLinePtr = static_cast<const InputPixelType*>(image.GetLinePtr(y));

			istd::CIntRanges::RangeList rangeList;
			outputRangesPtr->GetAsList(lineRange, rangeList);
			for (		istd::CIntRanges::RangeList::ConstIterator iter = rangeList.constBegin();
						iter != rangeList.constEnd();
						++iter){
				const istd::CIntRange& rangeH = *iter;
				Q_ASSERT(rangeH.GetMinValue() >= 0);
				Q_ASSERT(rangeH.GetMaxValue() <= imageSize.GetX());

				for (int x = rangeH.GetMinValue(); x < rangeH.GetMaxValue(); ++x){
					const InputPixelType pixel = inputLinePtr[x];

					// TODO: implement it!
					Q_UNUSED(filterParamsPtr);
					Q_UNUSED(result);
					Q_UNUSED(pixel);
				}
			}
		}
	}

	return true;
}


// static public methods

bool CPreciseBlobProcessorComp::DoCalculateBlobs(
			const iblob::IBlobFilterParams* filterParamsPtr,
			const iimg::CScanlineMask& imageMask,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& result,
			istd::ILogger* loggerPtr)
{
	iimg::IBitmap::PixelFormat pixelFormat = image.GetPixelFormat();

	switch (pixelFormat){
	case iimg::IBitmap::PF_GRAY:
		return DoCalculateBlobsTemplate<quint8, float>(filterParamsPtr, imageMask, image, result);

	case iimg::IBitmap::PF_RGB:
		return DoCalculateBlobsTemplate<iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbCropAccum32<qint32, 22, true, true> >(filterParamsPtr, imageMask, image, result);

	case iimg::IBitmap::PF_RGBA:
		return DoCalculateBlobsTemplate<iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbaCropAccum32<qint32, 22, true, true> >(filterParamsPtr, imageMask, image, result);

	case iimg::IBitmap::PF_GRAY16:
		return DoCalculateBlobsTemplate<quint16, float >(filterParamsPtr, imageMask, image, result);

	case iimg::IBitmap::PF_GRAY32:
		return DoCalculateBlobsTemplate<quint32, double>(filterParamsPtr, imageMask, image, result);

	case iimg::IBitmap::PF_FLOAT32:
		return DoCalculateBlobsTemplate<float, float>(filterParamsPtr, imageMask, image, result);

	case iimg::IBitmap::PF_FLOAT64:
		return DoCalculateBlobsTemplate<double, double>(filterParamsPtr, imageMask, image, result);

	default:
		if (loggerPtr != NULL){
			loggerPtr->SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						0,
						QObject::tr("Input image format '%1' not supported").arg(iimg::CPixelFormatList::GetInstance().GetOptionName(pixelFormat)),
						QObject::tr("PreciseBlobProcessor"));
		}
		return false;
	}
}


// protected methods

// reimplemented (iblob::CBlobProcessorCompBase)

bool CPreciseBlobProcessorComp::CalculateBlobs(
			const iprm::IParamsSet* paramsPtr,
			const iblob::IBlobFilterParams* filterParamsPtr,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& result)
{
	// create image mask
	istd::CIndex2d imageSize = image.GetImageSize();
	i2d::CRect clipArea(imageSize);

	iimg::CScanlineMask imageMask;
	iprm::TParamsPtr<i2d::IObject2d> aoiObjectPtr(paramsPtr, m_aoiParamIdAttrPtr, m_defaultAoiCompPtr, false);
	if (aoiObjectPtr.IsValid()){
		imageMask.SetCalibration(image.GetCalibration());

		if (!imageMask.CreateFromGeometry(*aoiObjectPtr.GetPtr(), &clipArea)){
			SendErrorMessage(0, QObject::tr("AOI type is not supported"));

			return false;
		}
	}
	else{
		imageMask.CreateFilled(clipArea);
	}

	return DoCalculateBlobs(
				filterParamsPtr,
				imageMask,
				image,
				result,
				this);
}


} // namespace iblob


