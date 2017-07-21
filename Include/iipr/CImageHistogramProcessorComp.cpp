#include <iipr/CImageHistogramProcessorComp.h>


// STL includes
#include <cstring>

// Qt includes
#include <QtCore/QtMath>
#include <QtCore/QCoreApplication>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <ibase/CSize.h>
#include <iimg/IBitmap.h>
#include <iimg/CScanlineMask.h>

// ACF-Solutions includes
#include <imeas/CGeneralDataSequenceInfo.h>


namespace iipr
{


// static attributes

static const imath::CGeneralUnitInfo s_channelUnitInfo(imath::IUnitInfo::UT_TECHNICAL);


// protected methods

// reimplemented (CImageRegionProcessorCompBase)

bool CImageHistogramProcessorComp::ProcessImageRegion(
			const iimg::IBitmap& inputBitmap,
			const iprm::IParamsSet* /*paramsPtr*/,
			const i2d::IObject2d* aoiPtr,
			istd::IChangeable* outputPtr) const
{
	imeas::IDiscreteDataSequence* histogramPtr = dynamic_cast<imeas::IDiscreteDataSequence*>(outputPtr);
	if (histogramPtr == NULL){
		return false;
	}

	if (inputBitmap.IsEmpty()){
		histogramPtr->ResetSequence();

		return true;
	}


	int componentsBitCount = inputBitmap.GetComponentBitsCount();
	if (componentsBitCount != 8){
		SendWarningMessage(0, "Only 8-bit images are supported");

		return false;
	}

	ibase::CSize inputBitmapSize(inputBitmap.GetImageSize());
	i2d::CRectangle realArea = i2d::CRectangle(inputBitmapSize);

	const i2d::IObject2d* usedAoiPtr = (aoiPtr != NULL)? aoiPtr: &realArea;

	iimg::CScanlineMask bitmapRegion;
	i2d::CRect clipArea(inputBitmap.GetImageSize());
	if (!bitmapRegion.CreateFromGeometry(*usedAoiPtr, &clipArea)){
		SendWarningMessage(0, "Cannot create the region");

		return false;
	}

	if (bitmapRegion.IsBitmapRegionEmpty()){
		SendWarningMessage(0, "Cannot process an empty region");
	
		return false;
	}

	i2d::CRect regionRect = bitmapRegion.GetBoundingBox();
	int regionTop = qMax(regionRect.GetTop(), 0);
	int regionBottom = qMin(regionRect.GetBottom(), inputBitmapSize.GetY());

	int pixelBytesCount = inputBitmap.GetComponentsCount();
	int usedColorComponents = pixelBytesCount;

	QStringList channelNames;

	int pixelFormat = inputBitmap.GetPixelFormat();
	switch (pixelFormat){
		case iimg::IBitmap::PF_GRAY:
			usedColorComponents = 1;
			channelNames << QCoreApplication::tr("Gray");
			break;

		case iimg::IBitmap::PF_RGB:
			usedColorComponents = 3;
			channelNames << QCoreApplication::tr("Red") << QCoreApplication::tr("Green") << QCoreApplication::tr("Blue");
			break;

		case iimg::IBitmap::PF_RGBA:
			usedColorComponents = 4;
			channelNames << QCoreApplication::tr("Alpha") << QCoreApplication::tr("Red") << QCoreApplication::tr("Green") << QCoreApplication::tr("Blue");
			break;
	}

	int histogramSize = 256 * usedColorComponents;

	istd::TDelPtr<quint32, istd::ArrayAccessor<quint32> > histogramDataPtr(new quint32[histogramSize]);
	quint32* histogramDataBufferPtr = histogramDataPtr.GetPtr();

	std::memset(histogramDataBufferPtr, 0, histogramSize * sizeof(quint32));
	int pixelCount = 0;

	for (int y = regionTop; y < regionBottom; y++){
		const quint8* inputLinePtr = (quint8*)inputBitmap.GetLinePtr(y);

		istd::CIntRanges::RangeList rangesList;
		const istd::CIntRanges* rangesPtr = bitmapRegion.GetPixelRanges(y);
		if (rangesPtr == NULL){
			continue;
		}

		rangesPtr->GetAsList(clipArea.GetHorizontalRange(), rangesList);

		for (		istd::CIntRanges::RangeList::ConstIterator iter = rangesList.begin();
					iter != rangesList.end();
					++iter){
			const istd::CIntRange& pixelRange = *iter;

			int rangeStart = qMax(pixelRange.GetMinValue(), 0);
			int rangeEnd = qMin(pixelRange.GetMaxValue(), inputBitmapSize.GetX());

			if (rangeEnd > rangeStart){
				const quint8* inputPixelPtr = inputLinePtr + rangeStart * pixelBytesCount;

				for (int x = rangeStart; x < rangeEnd; ++x){
					for (int componentIndex = 0; componentIndex < usedColorComponents; ++componentIndex){
						quint8 pixelComponentValue = inputPixelPtr[componentIndex];

						int mappedIndex = usedColorComponents - componentIndex - 1;

						++histogramDataBufferPtr[mappedIndex + pixelComponentValue * usedColorComponents];
					}

					inputPixelPtr += pixelBytesCount;
				}

				pixelCount += rangeEnd - rangeStart;
			}
		}
	}

	double normFactor = qPow(2.0, histogramPtr->GetSampleDepth()) - 1;

	for (int histIndex = 0; histIndex < histogramSize; histIndex++){
		double normHist = histogramDataBufferPtr[histIndex] / double(pixelCount);

		histogramDataBufferPtr[histIndex] = quint32(normHist * normFactor + 0.5);
	}

	imeas::CGeneralDataSequenceInfo* sequenceInfoPtr = new imeas::CGeneralDataSequenceInfo(
				channelNames.size(),
				histogramSize,
				imeas::IDataSequenceInfo::WM_NONE,
				imeas::IDataSequenceInfo::SIF_CHANNELS_COUNT_FIXED);

	for (QStringList::ConstIterator channelNameIter = channelNames.constBegin(); channelNameIter != channelNames.constEnd(); ++channelNameIter){
		const QString& channelName = *channelNameIter;

		sequenceInfoPtr->InsertValueInfo(channelName, channelName, channelName.toUtf8(), s_channelUnitInfo);
	}

	istd::CChangeNotifier notifier(histogramPtr);
	Q_UNUSED(notifier);

	return histogramPtr->CreateDiscreteSequenceWithInfo(
					istd::TSmartPtr<const imeas::IDataSequenceInfo>(sequenceInfoPtr),
					256,
					histogramDataPtr.PopPtr(),
					true,
					0,
					0,
					sizeof(quint32) * 8,
					usedColorComponents);
}


} // namespace iipr


