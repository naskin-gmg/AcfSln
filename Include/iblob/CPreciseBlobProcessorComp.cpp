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
#include <iblob/CBlobFeature.h>


namespace iblob
{


struct ClassDescriptor
{
	ClassDescriptor()
	:	m_isActive(true),
		m_finalClassIndex(-1),
		m_prevDescriptorIndex(-1),
		m_maxArea(0),
		m_cummulatedX(0),
		m_cummulatedY(0)
	{
	}

	bool m_isActive;
	int m_finalClassIndex;
	int m_prevDescriptorIndex;
	double m_maxArea;
	double m_cummulatedX;
	double m_cummulatedY;
};


/**
	Template filter funtion.
*/
template <	typename InputPixelType,
			typename WorkingType>
bool DoCalculateBlobsTemplate(
			WorkingType threshold,
			const iimg::CScanlineMask& imageMask,
			const iimg::IBitmap& image,
			QVector<ClassDescriptor>& classDescriptors)
{
	istd::CIndex2d imageSize = image.GetImageSize();

	QVector<int> classIndexLine(imageSize.GetX(), -1);

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

				int currentClassIndex = -1;

				for (int x = rangeH.GetMinValue(); x < rangeH.GetMaxValue(); ++x){
					const InputPixelType pixelValue = inputLinePtr[x];

					int& aboveClassIndex = classIndexLine[x];

					bool isIn = (pixelValue > threshold);

					if (isIn){
						if (currentClassIndex >= 0){
							// there is class conflict, merge it!
							int index = aboveClassIndex;
							while (index >= 0){
								ClassDescriptor& reducedDescriptor = classDescriptors[index];

								if (reducedDescriptor.m_finalClassIndex < 0){
									ClassDescriptor& finalDescriptor = classDescriptors[reducedDescriptor.m_finalClassIndex];
									finalDescriptor.m_maxArea += reducedDescriptor.m_maxArea;
									finalDescriptor.m_cummulatedX += reducedDescriptor.m_cummulatedX;
									finalDescriptor.m_cummulatedY += reducedDescriptor.m_cummulatedY;
								}
								reducedDescriptor.m_maxArea = 0;
								reducedDescriptor.m_cummulatedX = 0;
								reducedDescriptor.m_cummulatedY = 0;

								index = reducedDescriptor.m_prevDescriptorIndex;

								reducedDescriptor.m_finalClassIndex = currentClassIndex;
								reducedDescriptor.m_prevDescriptorIndex = currentClassIndex;
							}
						}
						else{
							if (aboveClassIndex >= 0){
								currentClassIndex = aboveClassIndex;
							}
							else{
								currentClassIndex = classDescriptors.size();
								classDescriptors.push_back(ClassDescriptor());
							}
						}

						ClassDescriptor& currentDescriptor = classDescriptors[currentClassIndex];
						currentDescriptor.m_maxArea += 1;
						currentDescriptor.m_cummulatedX += x;
						currentDescriptor.m_cummulatedY += y;
					}
					else{
						currentClassIndex = -1;
					}

					aboveClassIndex = currentClassIndex;
				}
			}
		}
	}

	return true;
}


// static public methods

bool CPreciseBlobProcessorComp::DoCalculateBlobs(
			double threshold,
			const iblob::IBlobFilterParams* filterParamsPtr,
			const iimg::CScanlineMask& imageMask,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& result,
			istd::ILogger* loggerPtr)
{
	iimg::IBitmap::PixelFormat pixelFormat = image.GetPixelFormat();

	QVector<ClassDescriptor> classDescriptors;

	bool retVal = false;

	switch (pixelFormat){
	case iimg::IBitmap::PF_GRAY:
		retVal = DoCalculateBlobsTemplate<quint8, float>(threshold, imageMask, image, classDescriptors);

	case iimg::IBitmap::PF_GRAY16:
		retVal = DoCalculateBlobsTemplate<quint16, float >(threshold, imageMask, image, classDescriptors);

	case iimg::IBitmap::PF_GRAY32:
		retVal = DoCalculateBlobsTemplate<quint32, double>(threshold, imageMask, image, classDescriptors);

	case iimg::IBitmap::PF_FLOAT32:
		retVal = DoCalculateBlobsTemplate<float, float>(threshold, imageMask, image, classDescriptors);

	case iimg::IBitmap::PF_FLOAT64:
		retVal = DoCalculateBlobsTemplate<double, double>(threshold, imageMask, image, classDescriptors);

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

	if (retVal){
		for (QVector<ClassDescriptor>::ConstIterator iter = classDescriptors.constBegin(); iter != classDescriptors.constEnd(); ++iter){
			const ClassDescriptor& descriptor = *iter;
			if (descriptor.m_finalClassIndex < 0){
				double posX = descriptor.m_cummulatedX / descriptor.m_maxArea;
				double posY = descriptor.m_cummulatedY / descriptor.m_maxArea;
				double area = descriptor.m_maxArea;
				double perimeter = descriptor.m_maxArea / I_PI;
				double circularity = 1;

				bool passedByFilter = true;

				if ((filterParamsPtr != NULL) && filterParamsPtr->IsFiltersEnabled()){
					int filtersCount = filterParamsPtr->GetFiltersCount();

					for (int filterIndex = 0; filterIndex < filtersCount; ++filterIndex){
						const iblob::IBlobFilterParams::Filter& filter = filterParamsPtr->GetFilterAt(filterIndex);

						if (filter.blobDescriptorType == iblob::CBlobDescriptorInfo::BDT_CIRCULARITY){
							passedByFilter = passedByFilter && IsValueAcceptedByFilter(filter, circularity);
						}

						if (filter.blobDescriptorType == iblob::CBlobDescriptorInfo::BDT_AREA){
							passedByFilter = passedByFilter && IsValueAcceptedByFilter(filter, area);
						}

						if (filter.blobDescriptorType == iblob::CBlobDescriptorInfo::BDT_PERIMETER){
							passedByFilter = passedByFilter && IsValueAcceptedByFilter(filter, perimeter);
						}
					}
				}

				if (passedByFilter){
					iblob::CBlobFeature* blobFeaturePtr = new iblob::CBlobFeature(area, perimeter, i2d::CVector2d(posX, posY));

					result.AddFeature(blobFeaturePtr);
				}
			}
		}
	}

	return retVal;
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

	double threshold = 128;
	iprm::TParamsPtr<imeas::INumericValue> thresholdValuePtr(paramsPtr, m_thresholdParamIdAttrPtr, m_defaultThresholdCompPtr, false);
	if (thresholdValuePtr.IsValid()){
		imath::CVarVector values = thresholdValuePtr->GetValues();
		if (values.GetElementsCount() >= 1){
			threshold = values[0];
		}
	}

	return DoCalculateBlobs(
				threshold,
				filterParamsPtr,
				imageMask,
				image,
				result,
				this);
}


} // namespace iblob


