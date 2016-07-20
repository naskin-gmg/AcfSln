#include "iipr/CHoughLineFinderComp.h"


// ACF include
#include "iimg/CScanlineMask.h"
#include "iprm/TParamsPtr.h"
#include "ilog/TExtMessage.h"


namespace iipr
{


// reimplemented (iproc::IProcessor)

int CHoughLineFinderComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const iimg::IBitmap* imagePtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	IFeaturesConsumer* consumerPtr = dynamic_cast<IFeaturesConsumer*>(outputPtr);

	if (		(imagePtr == NULL) ||
				(consumerPtr == NULL)){
		return TS_INVALID;
	}

	return DoExtractFeatures(paramsPtr, *imagePtr, *consumerPtr, progressManagerPtr);
}


// reimplemented (iipr::IImageToFeatureProcessor)

int CHoughLineFinderComp::DoExtractFeatures(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& image,
			IFeaturesConsumer& results,
			ibase::IProgressManager* progressManagerPtr)
{
	ibase::CSize size = image.GetImageSize();
	if ((size.GetX() < 3) || (size.GetY() < 3)){
		SendErrorMessage(0, "Image too small to calculate edges");

		return TS_INVALID;
	}

	if (!CreateHoughSpace()){
		return TS_INVALID;
	}

	// calculate parameters
	int directionThreshold2 = 25 * 25;

	iimg::CScanlineMask mask;

	i2d::CRect clipArea(size);
	iprm::TParamsPtr<i2d::IObject2d> aoiObjectPtr(paramsPtr, m_aoiParamIdAttrPtr, m_defaultAoiCompPtr, false);
	if (aoiObjectPtr.IsValid()){
		mask.SetCalibration(image.GetCalibration());

		mask.CreateFromGeometry(*aoiObjectPtr.GetPtr(), &clipArea);
	}
	else{
		mask.CreateFilled(clipArea);
	}

	// simple erosion
	mask.Intersection(mask.GetTranslated(-1, 0));
	mask.Intersection(mask.GetTranslated(1, 0));
	mask.Intersection(mask.GetTranslated(0, -1));
	mask.Intersection(mask.GetTranslated(0, 1));

	i2d::CVector2d imageCenter = mask.GetCenter();

	int progressSessionIndex = -1;
	if (progressManagerPtr != NULL){
		progressSessionIndex = progressManagerPtr->BeginProgressSession("LineFinder", "Find lines with Hough", true);
	}

	const quint8* prevLinePtr = (const quint8*)image.GetLinePtr(0);
	const quint8* linePtr = (const quint8*)image.GetLinePtr(1);
	for (int y = 1; y < size.GetY() - 1; ++y){
		const quint8* nextLinePtr = (const quint8*)image.GetLinePtr(y + 1);

		const istd::CIntRanges* inputRangesPtr = mask.GetPixelRanges(y);
		if (inputRangesPtr != NULL){
			istd::CIntRanges::RangeList rangeList;
			inputRangesPtr->GetAsList(clipArea.GetHorizontalRange(), rangeList);

			for (istd::CIntRanges::RangeList::ConstIterator iter = rangeList.constBegin();
						iter != rangeList.constEnd();
						++iter){
				const istd::CIntRange& rangeH = *iter;

				for (int x = rangeH.GetMinValue(); x < rangeH.GetMaxValue(); ++x){
					int leftPixel = linePtr[x - 1];
					int rightPixel = linePtr[x + 1];
					int topPixel = prevLinePtr[x];
					int bottomPixel = nextLinePtr[x];

					int diffX = rightPixel - leftPixel;
					int diffY = bottomPixel - topPixel;

					if (diffX * diffX + diffY * diffY > directionThreshold2){
						i2d::CVector2d position(x - imageCenter.GetX(), y - imageCenter.GetY());
						i2d::CVector2d direction(diffX, diffY);

						UpdateHoughSpace(direction, position);
					}
				}
			}
		}

		prevLinePtr = linePtr;
		linePtr = nextLinePtr;

		if (progressSessionIndex >= 0){
			Q_ASSERT(progressManagerPtr != NULL);

			progressManagerPtr->OnProgress(progressSessionIndex, double(y) / size.GetY());

			if (progressManagerPtr->IsCanceled(progressSessionIndex)){
				break;
			}
		}
	}

	SmoothHoughSpace(3);

	WeightToHoughPosMap posMap;

	AnalyseHoughSpace(*m_defaultMaxLinesAttrPtr, 100, posMap);

	if (!posMap.isEmpty()){
		double bestWeight = posMap.firstKey();

		int lineIndex = 0;
		for (WeightToHoughPosMap::ConstIterator resultIter = posMap.constBegin(); resultIter != posMap.constEnd(); ++resultIter, ++lineIndex){
			const i2d::CVector2d& houghPos = resultIter.value();

			LineFeature* featurePtr = new LineFeature();

			i2d::CVector2d direction;
			direction.Init(houghPos.GetY() * I_2PI / m_angleVectors.size());
			i2d::CVector2d centralPoint = direction.GetOrthogonal() * houghPos.GetX();

			featurePtr->SetPoint1(imageCenter + centralPoint - direction * 100);
			featurePtr->SetPoint2(imageCenter + centralPoint + direction * 100);
			featurePtr->SetWeight(resultIter.key() / bestWeight);

			results.AddFeature(featurePtr);

			if (m_resultConsumerCompPtr.IsValid()){
				ilog::TExtMessageModel<i2d::CLine2d>* pointMessagePtr = new ilog::TExtMessageModel<i2d::CLine2d>(
							istd::IInformationProvider::IC_INFO,
							FOUND_LINE,
							QString("Line %1").arg(lineIndex + 1),
							"LineFinder");
				pointMessagePtr->SetPoint1(featurePtr->GetPoint1());
				pointMessagePtr->SetPoint2(featurePtr->GetPoint2());

				m_resultConsumerCompPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(pointMessagePtr));
			}

			results.AddFeature(featurePtr);
		}
	}

	if (progressSessionIndex >= 0){
		Q_ASSERT(progressManagerPtr != NULL);

		progressManagerPtr->EndProgressSession(progressSessionIndex);
	}

	return TS_INVALID;
}


// protected methods

bool CHoughLineFinderComp::CreateHoughSpace()
{
	int angleSize = *m_defaultAngleResAttrPtr;
	if (m_houghSpace.CreateBitmap(iimg::IBitmap::PF_GRAY32, istd::CIndex2d(*m_defaultRadiusResAttrPtr, angleSize))){
		m_angleVectors.resize(angleSize);

		for (int i = 0; i < angleSize; ++i){
			double angle = i * I_2PI / angleSize;

			m_angleVectors[i].Init(angle);
		}

		return true;
	}
	else{
		m_angleVectors.clear();

		return false;
	}
}


void CHoughLineFinderComp::UpdateHoughSpace(const i2d::CVector2d& position, const i2d::CVector2d& direction)
{
	istd::CIndex2d spaceSize = m_houghSpace.GetImageSize();

	double directionAngle = direction.GetAngle();
	int firstAngleIndex = int((directionAngle / I_2PI + 1.75) * spaceSize.GetY() + 1) % spaceSize.GetY();
	int lastAngleIndex = int((directionAngle / I_2PI + 2.25) * spaceSize.GetY()) % spaceSize.GetY();

	for (int angleIndex = firstAngleIndex; angleIndex != lastAngleIndex; ++angleIndex){
		quint32* angleLinePtr = (quint32*)m_houghSpace.GetLinePtr(angleIndex);

		if (angleIndex >= spaceSize.GetY()){
			angleIndex = 0;
		}

		const i2d::CVector2d& angleVector = m_angleVectors[angleIndex];

		double radius = angleVector.GetCrossProductZ(position);
		if (radius >= 0){
			int radiusIndex = int(radius + 0.5);
			if (radiusIndex < spaceSize.GetX()){
				double weight = angleVector.GetDotProduct(direction);
				Q_ASSERT(weight >= 0);

				angleLinePtr[radiusIndex] += int(weight);
			}
		}
	}
}


void CHoughLineFinderComp::SmoothHoughSpace(int /*iterations*/)
{
	// TODO: implement it
}


void CHoughLineFinderComp::AnalyseHoughSpace(int maxPoints, int minWeight, WeightToHoughPosMap& result)
{
	istd::CIndex2d spaceSize = m_houghSpace.GetImageSize();

	for (int angleIndex = 0; angleIndex < spaceSize.GetY(); ++angleIndex){
		const quint32* angleLinePtr = (const quint32*)m_houghSpace.GetLinePtr((angleIndex + spaceSize.GetY() - 1) % spaceSize.GetY());
		const quint32* prevAngleLinePtr = (const quint32*)m_houghSpace.GetLinePtr(angleIndex);
		const quint32* nextAngleLinePtr = (const quint32*)m_houghSpace.GetLinePtr((angleIndex + 1) % spaceSize.GetY());

		int prevRadiusIndex = 0;
		int radiusIndex = 0;
		for (int nextRadiusIndex = 0; nextRadiusIndex < spaceSize.GetX(); ++nextRadiusIndex){
			quint32 value = angleLinePtr[radiusIndex];
			if (		(value >= quint32(minWeight)) &&
						(value >= prevAngleLinePtr[prevRadiusIndex]) &&
						(value > prevAngleLinePtr[radiusIndex]) &&
						(value > prevAngleLinePtr[nextRadiusIndex]) &&
						(value >= angleLinePtr[prevRadiusIndex]) &&
						(value > angleLinePtr[nextRadiusIndex]) &&
						(value >= nextAngleLinePtr[prevRadiusIndex]) &&
						(value >= nextAngleLinePtr[radiusIndex]) &&
						(value > nextAngleLinePtr[nextRadiusIndex])){
				int diffLeft = value - angleLinePtr[prevRadiusIndex];
				int diffRight = value - angleLinePtr[nextRadiusIndex];
				int diffTop = value - prevAngleLinePtr[radiusIndex];
				int diffBottom = value - nextAngleLinePtr[radiusIndex];
				double correctionX = double(diffLeft) / (diffLeft + diffRight);
				double correctionY = double(diffTop) / (diffTop + diffBottom);

				i2d::CVector2d resultPos(radiusIndex + correctionX, angleIndex + correctionY);

				result.insert(value, resultPos);

				if (result.size() > maxPoints){
					WeightToHoughPosMap::Iterator lastIter = result.end()--;

					minWeight = lastIter.key();

					result.erase(lastIter);	// remove the weekest element
				}
			}

			prevRadiusIndex = radiusIndex;
			radiusIndex = nextRadiusIndex;
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CHoughLineFinderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	CreateHoughSpace();
}


} // namespace iipr


