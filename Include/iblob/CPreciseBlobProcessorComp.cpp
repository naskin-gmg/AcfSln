// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iblob/CPreciseBlobProcessorComp.h>


// STL includes
#include <cstring>

// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <istd/TRange.h>
#include <imath/CFixedPointManip.h>
#include <imod/TModelWrap.h>
#include <ilog/CExtMessage.h>
#include <iprm/TParamsPtr.h>
#include <i2d/CCircle.h>
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
	explicit ClassDescriptor(int classIndex = -1)
	:	m_finalClassIndex(classIndex),
		m_area(0),
		m_cummulatedX(0),
		m_cummulatedY(0),
		m_perimeter(0)
	{
	}

	int m_finalClassIndex;
	double m_area;
	double m_cummulatedX;
	double m_cummulatedY;
	double m_perimeter;
};


template <typename InputPixelType>
void DoCalculateRange(
			const istd::CRange& relValueRange,
			const iimg::CScanlineMask& imageMask,
			const iimg::IBitmap& image,
			istd::TRange<InputPixelType>& result)
{
	istd::CIndex2d imageSize = image.GetImageSize();
	istd::CIntRange lineRange(0, imageSize.GetX());

	InputPixelType minValue = std::numeric_limits<InputPixelType>::max();	// initiate min to max and max to min - invalid range
	InputPixelType maxValue = std::numeric_limits<InputPixelType>::lowest();
	Q_ASSERT(minValue > maxValue);

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
					const InputPixelType pixelValue = inputLinePtr[x];

					if (pixelValue < minValue){
						minValue = pixelValue;
					}

					if (pixelValue > maxValue){
						maxValue = pixelValue;
					}
				}
			}
		}
	}

	result.SetMinValue(InputPixelType(minValue + relValueRange.GetMinValue() * (maxValue - minValue)));
	result.SetMaxValue(InputPixelType(minValue + relValueRange.GetMaxValue() * (maxValue - minValue)));
}


/**
	Template filter funtion.
*/
template <typename InputPixelType>
bool DoCalculateBlobsTemplate(
			istd::TRange<InputPixelType>& objectRange,
			const iimg::CScanlineMask& imageMask,
			const iimg::IBitmap& image,
			QVector<ClassDescriptor>& classDescriptors)
{
	if (objectRange.IsEmpty()){
		return true;
	}

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

					int aboveClassIndex = classIndexLine[x];

					if (objectRange.Contains(pixelValue)){
						if (currentClassIndex >= 0){
							ClassDescriptor& dominantDescriptor = classDescriptors[currentClassIndex];
	
							if (aboveClassIndex >= 0){
								if (currentClassIndex != classDescriptors[aboveClassIndex].m_finalClassIndex){
									int indexToReduce = aboveClassIndex;
									Q_ASSERT(indexToReduce != currentClassIndex);

									ClassDescriptor& reducedDescriptor = classDescriptors[indexToReduce];

									dominantDescriptor.m_area += reducedDescriptor.m_area;
									dominantDescriptor.m_cummulatedX += reducedDescriptor.m_cummulatedX;
									dominantDescriptor.m_cummulatedY += reducedDescriptor.m_cummulatedY;
									dominantDescriptor.m_perimeter += reducedDescriptor.m_perimeter;

									reducedDescriptor.m_area = 0;
									reducedDescriptor.m_cummulatedX = 0;
									reducedDescriptor.m_cummulatedY = 0;
									reducedDescriptor.m_perimeter = 0;

									reducedDescriptor.m_finalClassIndex = currentClassIndex;
								}
							}
							else{
								dominantDescriptor.m_perimeter += 1;	// above
							}
						}
						else{
							if (aboveClassIndex >= 0){
								// overtake class from above
								currentClassIndex = classDescriptors[aboveClassIndex].m_finalClassIndex;

								ClassDescriptor& descriptor = classDescriptors[currentClassIndex];
								descriptor.m_perimeter += 1;	// left
							}
							else{
								// create new class
								currentClassIndex = classDescriptors.size();
								classDescriptors.push_back(ClassDescriptor(currentClassIndex));
								ClassDescriptor& descriptor = classDescriptors.last();
								descriptor.m_perimeter += 2;	// left and above
							}
						}

						ClassDescriptor& currentDescriptor = classDescriptors[currentClassIndex];
						currentDescriptor.m_area += 1;
						currentDescriptor.m_cummulatedX += x;
						currentDescriptor.m_cummulatedY += y;
					}
					else{
						if (currentClassIndex >= 0){
							ClassDescriptor& descriptor = classDescriptors[currentClassIndex];

							if (aboveClassIndex >= 0){
								descriptor.m_perimeter += 1;	// right
							}
							else{
								descriptor.m_perimeter += 2;	// right and above
							}

							currentClassIndex = -1;
						}
						else if (aboveClassIndex >= 0){
							ClassDescriptor& descriptor = classDescriptors[aboveClassIndex];

							descriptor.m_perimeter += 1;	// above
						}
					}

					classIndexLine[x] = currentClassIndex;
				}
			}
		}
	}

	return true;
}


// public methods

CPreciseBlobProcessorComp::CPreciseBlobProcessorComp()
{
	static imath::CFixedPointManip percentManip(3);

	imath::CGeneralUnitInfo unitInfo(imath::IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(0, 1), &percentManip);

	m_thresholdContraints.InsertValueInfo(QObject::tr("Threshold Min"), QObject::tr("Minimal Threshold Value"), "ThresholdMin", unitInfo);
	m_thresholdContraints.InsertValueInfo(QObject::tr("Threshold Max"), QObject::tr("Maximal Threshold Value"), "ThresholdMax", unitInfo);
}


// static public methods

bool CPreciseBlobProcessorComp::DoCalculateBlobs(
			const istd::CRange& valueRange,
			bool isRangeRelative,
			const iblob::IBlobFilterParams* filterParamsPtr,
			const iimg::CScanlineMask& imageMask,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& result,
			ilog::IMessageConsumer* resultConsumerPtr,
			ilog::IMessageConsumer* tempMessageConsumerPtr)
{
	iimg::IBitmap::PixelFormat pixelFormat = image.GetPixelFormat();

	QVector<ClassDescriptor> classDescriptors;

	bool retVal = false;

	switch (pixelFormat){
	case iimg::IBitmap::PF_GRAY:
		{
			istd::TRange<quint8> objectRange(qMax(quint8(0), quint8(valueRange.GetMinValue() * 255 + 0.5)), qMin(quint8(255), quint8(valueRange.GetMaxValue() * 255 + 0.5)));
			if (isRangeRelative){
				DoCalculateRange<quint8>(valueRange, imageMask, image, objectRange);
			}
			retVal = DoCalculateBlobsTemplate<quint8>(objectRange, imageMask, image, classDescriptors);
		}
		break;

	case iimg::IBitmap::PF_GRAY16:
		{
			istd::TRange<quint16> objectRange(qMax(quint16(0), quint16(valueRange.GetMinValue() * 0xffff + 0.5)), qMin(quint16(255), quint16(valueRange.GetMaxValue() * 0xffff + 0.5)));
			if (isRangeRelative){
				DoCalculateRange<quint16>(valueRange, imageMask, image, objectRange);
			}
			retVal = DoCalculateBlobsTemplate<quint16>(objectRange, imageMask, image, classDescriptors);
		}
		break;

	case iimg::IBitmap::PF_GRAY32:
		{
			istd::TRange<quint32> objectRange(qMax(quint32(0), quint32(valueRange.GetMinValue() * 0xffffffff + 0.5)), qMin(quint32(255), quint32(valueRange.GetMaxValue() * 0xffffffff + 0.5)));
			if (isRangeRelative){
				DoCalculateRange<quint32>(valueRange, imageMask, image, objectRange);
			}
			retVal = DoCalculateBlobsTemplate<quint32>(objectRange, imageMask, image, classDescriptors);
		}
		break;

	case iimg::IBitmap::PF_FLOAT32:
		{
			istd::TRange<float> objectRange(float(valueRange.GetMinValue()), float(valueRange.GetMaxValue()));
			if (isRangeRelative){
				DoCalculateRange<float>(valueRange, imageMask, image, objectRange);
			}
			retVal = DoCalculateBlobsTemplate<float>(objectRange, imageMask, image, classDescriptors);
		}
		break;

	case iimg::IBitmap::PF_FLOAT64:
		{
			istd::TRange<double> objectRange(valueRange.GetMinValue(), valueRange.GetMaxValue());
			if (isRangeRelative){
				DoCalculateRange<double>(valueRange, imageMask, image, objectRange);
			}
			retVal = DoCalculateBlobsTemplate<double>(objectRange, imageMask, image, classDescriptors);
		}
		break;

	default:
		if (resultConsumerPtr != NULL){
			ilog::CMessage* messagePtr = new ilog::CMessage(
						istd::IInformationProvider::IC_ERROR,
						0,
						QObject::tr("Input image format '%1' not supported").arg(iimg::CPixelFormatList::GetInstance().GetOptionName(pixelFormat)),
						QObject::tr("PreciseBlobProcessor"));
			resultConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(messagePtr));
		}
		return false;
	}

	if (retVal){
		istd::TDelPtr<ilog::CExtMessage> blobMessagePtr;
		if (resultConsumerPtr != NULL){
			blobMessagePtr.SetPtr(new ilog::CExtMessage(
						istd::IInformationProvider::IC_INFO,
						MI_FOUND_BLOB,
						"",
						"PreciseBlobProcessor"));
		}

		istd::TDelPtr<ilog::CExtMessage> ignoredBlobsMessagePtr;
		if (tempMessageConsumerPtr != NULL){
			ignoredBlobsMessagePtr.SetPtr(new ilog::CExtMessage(
						istd::IInformationProvider::IC_NONE,
						MI_FOUND_BLOB,
						"",
						"PreciseBlobProcessor"));
		}

		int blobsCount = 0;
		int ignoredBlobsCount = 0;

		double scale = 1.0;
		const i2d::ICalibration2d* calibrationPtr = filterParamsPtr != NULL ? filterParamsPtr->GetCalibration() : NULL;
		if (calibrationPtr != NULL){
			i2d::CAffine2d transform;
			if (calibrationPtr->GetLocalInvTransform(i2d::CVector2d(0, 0), transform)){
				scale = transform.GetDeformMatrix().GetApproxScale();
			}
		}

		for (QVector<ClassDescriptor>::ConstIterator iter = classDescriptors.constBegin(); iter != classDescriptors.constEnd(); ++iter){
			const ClassDescriptor& descriptor = *iter;
			if (descriptor.m_area > 0){
				i2d::CVector2d position(descriptor.m_cummulatedX / descriptor.m_area, descriptor.m_cummulatedY / descriptor.m_area);
				double area = descriptor.m_area;
				double perimeter = descriptor.m_perimeter;
				double circularity = 4 * M_PI * area / (perimeter * perimeter);

				area *= scale;
				perimeter *= scale;

				bool passedByFilter = true;

				if (filterParamsPtr != NULL){
					passedByFilter = IsBlobAcceptedByFilter(*filterParamsPtr, area, perimeter, circularity);
				}

				if (passedByFilter){
					++blobsCount;

					iblob::CBlobFeature* blobFeaturePtr = new iblob::CBlobFeature(area, perimeter, position);
					result.AddFeature(blobFeaturePtr);

					if (blobMessagePtr.IsValid()){
						i2d::CCircle* blobMessageCirclePtr = new imod::TModelWrap<i2d::CCircle>();
						blobMessageCirclePtr->SetPosition(position);
						blobMessageCirclePtr->SetRadius(qSqrt(area / I_PI));
						blobMessagePtr->InsertAttachedObject(blobMessageCirclePtr, QObject::tr("Blob %1, Pos.: (%2; %3), Area: %4").arg(blobsCount).arg(position.GetX()).arg(position.GetY()).arg(area));
					}
				}
				else{
					++ignoredBlobsCount;

					if (ignoredBlobsMessagePtr.IsValid()){
						i2d::CCircle* blobMessageCirclePtr = new imod::TModelWrap<i2d::CCircle>();
						blobMessageCirclePtr->SetPosition(position);
						blobMessageCirclePtr->SetRadius(qSqrt(area / I_PI));
						ignoredBlobsMessagePtr->InsertAttachedObject(blobMessageCirclePtr, QObject::tr("Ignored blob %1, Pos.: (%2; %3), Area: %4").arg(ignoredBlobsCount).arg(position.GetX()).arg(position.GetY()).arg(area));
					}
				}
			}
		}

		if (blobMessagePtr.IsValid()){
			Q_ASSERT(resultConsumerPtr != NULL);

			blobMessagePtr->SetText(QObject::tr("Found %1 blob(s)").arg(blobsCount));

			resultConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(blobMessagePtr.PopPtr()));
		}

		if (ignoredBlobsMessagePtr.IsValid() && (ignoredBlobsMessagePtr->GetAttachedObjectsCount() > 0)){
			Q_ASSERT(tempMessageConsumerPtr != NULL);

			ignoredBlobsMessagePtr->SetText(QObject::tr("Ignored %1 blob(s)").arg(ignoredBlobsCount));

			tempMessageConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(ignoredBlobsMessagePtr.PopPtr()));
		}
	}

	return retVal;
}


// protected methods

// reimplemented (iblob::CBlobProcessorCompBase)

bool CPreciseBlobProcessorComp::CalculateBlobs(
			const iprm::IParamsSet* paramsPtr,
			const iblob::IBlobFilterParams* filterParamsPtr,
			const i2d::IObject2d* aoiPtr,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& result)
{
	// create image mask
	istd::CIndex2d imageSize = image.GetImageSize();
	i2d::CRect clipArea(imageSize);

	iimg::CScanlineMask imageMask;
	if (aoiPtr != NULL){
		imageMask.SetCalibration(image.GetCalibration());

		if (!imageMask.CreateFromGeometry(*aoiPtr, &clipArea)){
			SendErrorMessage(0, QObject::tr("AOI type is not supported"));

			return false;
		}
	}
	else{
		imageMask.CreateFilled(clipArea);
	}

	istd::CRange valueRange(0, 0.5);
	iprm::TParamsPtr<imeas::INumericValue> thresholdValuePtr(paramsPtr, m_thresholdParamIdAttrPtr, m_defaultThresholdCompPtr, false);
	if (thresholdValuePtr.IsValid()){
		imath::CVarVector values = thresholdValuePtr->GetValues();
		if (values.GetElementsCount() >= 2){
			valueRange = istd::CRange(values[0], values[1]);
			valueRange.Validate();
		}
	}

	bool isRangeRelative = true;
	iprm::TParamsPtr<iprm::IEnableableParam> isThresholdRelativePtr(paramsPtr, m_isThresholdRelativeParamIdAttrPtr, m_defaultIsThresholdRelativeCompPtr, false);
	if (isThresholdRelativePtr.IsValid()){
		isRangeRelative = isThresholdRelativePtr->IsEnabled();
	}

	return DoCalculateBlobs(
				valueRange,
				isRangeRelative,
				filterParamsPtr,
				imageMask,
				image,
				result,
				m_resultConsumerCompPtr.GetPtr(),
				m_tempConsumerCompPtr.GetPtr());
}


} // namespace iblob


