#include <iipr/CLensCorrFindSupplierComp.h>


// STL includes
#include <cmath>

// ACF include
#include <imath/CVarMatrix.h>
#include <ilog/CExtMessage.h>
#include <i2d/CLine2d.h>
#include <i2d/CPosition2d.h>
#include <i2d/CPolypoint.h>
#include <iprm/TParamsPtr.h>

// ACF-Solutions includes
#include <iipr/CHoughSpace2d.h>
#include <iipr/CPerspCalibFinder.h>
#include <iimg/CBitmap.h>


namespace iipr
{


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CLensCorrFindSupplierComp::GetCalibration() const
{
	return GetWorkProduct();
}


//protected methods

bool CLensCorrFindSupplierComp::CalculateCalibration(const iimg::IBitmap& image, icalib::CSimpleLensCorrection& result) const
{
	result.Reset();

	if (!m_pointFinderCompPtr.IsValid()){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_CRITICAL, 0, QObject::tr("No processor set"), "LensCorrectionFinder"));

		return false;
	}

	FeaturesConsumer consumer;
	if (m_pointFinderCompPtr->DoExtractFeatures(GetModelParametersSet(), image, consumer) != iproc::IProcessor::TS_OK){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 0, QObject::tr("Cannot extract features from image"), "LensCorrectionFinder"));

		return false;
	}

	if (consumer.features.size() < 9){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 0, QObject::tr("No enough features found"), "LensCorrectionFinder"));

		return false;
	}

	CHoughSpace2d lineSpace;
	int angleGridSize = *m_defaultAngleResAttrPtr;
	int radiusGridSize = *m_defaultRadiusResAttrPtr;
	istd::CIndex2d lineSpaceSize(angleGridSize, radiusGridSize);

	if (!lineSpace.CreateHoughSpace(lineSpaceSize, true, false)){
		return false;
	}

	istd::CIndex2d imageSize = image.GetImageSize();
	i2d::CVector2d imageCenter(imageSize.GetX() * 0.5, imageSize.GetY() * 0.5);

	for (		Features::ConstIterator iter1 = consumer.features.constBegin();
				iter1 != consumer.features.constEnd();
				++iter1){
		const FeatureInfo& feature1 = *iter1;

		for (		Features::ConstIterator iter2 = iter1 + 1;
					iter2 != consumer.features.constEnd();
					++iter2){
			const FeatureInfo& feature2 = *iter2;

			UpdateHoughSpace(feature1.position, feature2.position, feature1.weight * feature2.weight, imageCenter, lineSpace);
		}
	}

	lineSpace.SmoothHoughSpace(*m_defaultSmoothKernelAttrPtr, *m_defaultSmoothKernelAttrPtr);

	iipr::CHoughSpace2d::WeightToHoughPosMap foundLines;
	lineSpace.AnalyseHoughSpace(*m_defaultMaxLinesAttrPtr, 1, 0.25, *m_defaultSmoothKernelAttrPtr, 0.01, foundLines);
	if (foundLines.size() < 2){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 0, QObject::tr("No lines found"), "LensCorrectionFinder"));

		return false;
	}

	istd::TDelPtr<ilog::CExtMessage> linesMessagePtr = new ilog::CExtMessage(
				istd::IInformationProvider::IC_INFO,
				0,
				"All detected lines",
				"LensCorrectionFinder");

	QSet<i2d::CVector2d> allUsedPoints;

	QList<CorrectionLineInfo> allCorrectionInfos;
	int allPointsCount = 0;

	double linePosTolerance = imageCenter.GetLength() * *m_defaultSmoothKernelAttrPtr / lineSpaceSize.GetY();

	for (		iipr::CHoughSpace2d::WeightToHoughPosMap::ConstIterator houghIter = foundLines.constBegin();
				houghIter != foundLines.constEnd();
				++houghIter){
		const i2d::CVector2d& foundSpacePos = houghIter.value();

		i2d::CLine2d line = CalcCorrespondingLine(foundSpacePos, imageCenter, lineSpaceSize);

		QSet<i2d::CVector2d> positionsOnLine;

		for (		Features::ConstIterator featureIter = consumer.features.constBegin();
					featureIter != consumer.features.constEnd();
					++featureIter){
			const FeatureInfo& feature = *featureIter;

			if (line.GetExtendedDistance(feature.position) < linePosTolerance){
				positionsOnLine.insert(feature.position);
			}
		}

		if (positionsOnLine.size() >= 4){
			i2d::CLine2d representationLine;
			if (!representationLine.ApproxFromPoints(positionsOnLine)){
				continue;
			}

			CorrectionLineInfo lineInfo;
			lineInfo.orthoVector = representationLine.GetNearestPoint(imageCenter) - imageCenter;
			if (lineInfo.orthoVector.GetLength() < *m_defaultMinDistanceAttrPtr){
				continue;
			}

			for (		QSet<i2d::CVector2d>::ConstIterator pointIter = positionsOnLine.constBegin();
						pointIter != positionsOnLine.constEnd();
						++pointIter){
				const i2d::CVector2d& pos = *pointIter;

				CorrectionInfo info;
				info.position = pos - imageCenter;

				if (representationLine.GetExtendedIntersection(i2d::CLine2d(imageCenter, pos), info.diff)){
					info.diff -= pos;

					lineInfo.infos.push_back(info);
					allPointsCount++;
				}
			}

			allCorrectionInfos.push_back(lineInfo);


			i2d::CLine2d* lineObjectPtr = new imod::TModelWrap<i2d::CLine2d>();
			*lineObjectPtr = representationLine;
			linesMessagePtr->InsertAttachedObject(lineObjectPtr, QObject::tr("Detected line %1 using %2 points at Hough position (%3, %4)")
						.arg(allCorrectionInfos.size() + 1)
						.arg(positionsOnLine.size())
						.arg(foundSpacePos.GetX())
						.arg(foundSpacePos.GetY()));

			allUsedPoints += positionsOnLine;
		}
	}

	if (linesMessagePtr->GetAttachedObjectsCount() > 0){
		AddMessage(linesMessagePtr.PopPtr(), MCT_TEMP);
	}

	if (allPointsCount < 3){
		return false;
	}

	int varCount = allCorrectionInfos.size() + 2;
	imath::CVarMatrix solutionMatrix;
	solutionMatrix.SetSizes(istd::CIndex2d(varCount, allPointsCount * 2));
	solutionMatrix.Clear();
	imath::CVarMatrix vectorY;
	vectorY.SetSizes(istd::CIndex2d(1, allPointsCount * 2));

	int pointIndex = 0;
	int lineIndex = 0;
	for (		QList<CorrectionLineInfo>::ConstIterator lineIter = allCorrectionInfos.constBegin();
				lineIter != allCorrectionInfos.constEnd();
				++lineIter, ++lineIndex){
		const CorrectionLineInfo& lineInfo = *lineIter;
		double orthoLengt2 = lineInfo.orthoVector.GetLength2();
		double weight = orthoLengt2 * orthoLengt2;
//		i2d::CVector2d normalizedOrhoVector = lineInfo.orthoVector.GetOrthogonal();

		for (		CorrectionInfos::ConstIterator pointIter = lineInfo.infos.constBegin();
					pointIter != lineInfo.infos.constEnd();
					++pointIter, ++pointIndex){
			const CorrectionInfo& pointInfo = *pointIter;

			i2d::CVector2d normalizedOrhoVector = pointInfo.position;

			double positionDist = pointInfo.position.GetLength();

			solutionMatrix.SetAt(istd::CIndex2d(0, pointIndex * 2), pointInfo.position.GetX() * weight);
			solutionMatrix.SetAt(istd::CIndex2d(1, pointIndex * 2), pointInfo.position.GetX() * positionDist * weight);
			solutionMatrix.SetAt(istd::CIndex2d(2 + lineIndex, pointIndex * 2), -normalizedOrhoVector.GetX() * weight);
			vectorY.SetAt(istd::CIndex2d(0, pointIndex * 2), (pointInfo.position.GetX() + pointInfo.diff.GetX()) * weight);

			solutionMatrix.SetAt(istd::CIndex2d(0, pointIndex * 2 + 1), pointInfo.position.GetY() * weight);
			solutionMatrix.SetAt(istd::CIndex2d(1, pointIndex * 2 + 1), pointInfo.position.GetY() * positionDist * weight);
			solutionMatrix.SetAt(istd::CIndex2d(2 + lineIndex, pointIndex * 2 + 1), -normalizedOrhoVector.GetY() * weight);
			vectorY.SetAt(istd::CIndex2d(0, pointIndex * 2 + 1), (pointInfo.position.GetY() + pointInfo.diff.GetY()) * weight);
		}
	}

	imath::CVarMatrix resolvedX;
	imath::CVarMatrix::SolveRobustLSP(solutionMatrix, vectorY, resolvedX);

	Q_ASSERT(resolvedX.GetSizes() == istd::CIndex2d(1, varCount));

	double scaleFactor = resolvedX.GetAt(istd::CIndex2d(0, 0));
	double distFactor = resolvedX.GetAt(istd::CIndex2d(0, 1)) / scaleFactor;
	result.SetScaleFactor(scaleFactor);
	result.SetDistortionFactor(distFactor);
	result.SetOpticalCenter(imageCenter);

	ilog::CExtMessage* resultMessagePtr = new ilog::CExtMessage(
				istd::IInformationProvider::IC_INFO,
				0,
				QObject::tr("Found lens coeeficients: dist=%1, scale=%2").arg(distFactor * 1000000).arg(scaleFactor),
				"LensCorrectionFinder");
	AddMessage(resultMessagePtr, MCT_RESULTS);

	return true;
}


i2d::CVector2d CLensCorrFindSupplierComp::CalcHoughPos(
			const i2d::CVector2d& point1,
			const i2d::CVector2d& point2,
			const i2d::CVector2d& imageCenter,
			const istd::CIndex2d& spaceSize) const
{
	i2d::CVector2d angleVector = (point2 - point1).GetNormalized();
	double directionAngle = angleVector.GetAngle();
	double distanceToCenter = angleVector.GetCrossProductZ(point1 - imageCenter);
	if (distanceToCenter >= 0){
		directionAngle = std::fmod(directionAngle + I_2PI, I_2PI);
	}
	else{
		distanceToCenter = -distanceToCenter;
		directionAngle = std::fmod(directionAngle + I_PI, I_2PI);
	}
	Q_ASSERT(directionAngle >= 0);

	return i2d::CVector2d(directionAngle / I_2PI * spaceSize.GetX(), distanceToCenter * spaceSize.GetY() / imageCenter.GetLength());
}


i2d::CLine2d CLensCorrFindSupplierComp::CalcCorrespondingLine(
			const i2d::CVector2d& houghPos,
			const i2d::CVector2d& imageCenter,
			const istd::CIndex2d& spaceSize) const
{
	double directionAngle = houghPos.GetX() * I_2PI / spaceSize.GetX();
	double distanceToCenter = houghPos.GetY() * imageCenter.GetLength() / spaceSize.GetY();

	i2d::CVector2d angleVector;
	angleVector.Init(directionAngle);

	i2d::CVector2d lineCenter = imageCenter - angleVector.GetOrthogonal() * distanceToCenter;
	i2d::CVector2d lineDir = angleVector * 100;

	return i2d::CLine2d(lineCenter - lineDir, lineCenter + lineDir);
}


void CLensCorrFindSupplierComp::UpdateHoughSpace(
			const i2d::CVector2d& point1,
			const i2d::CVector2d& point2,
			double weight,
			const i2d::CVector2d& imageCenter,
			CHoughSpace2d& space) const
{
	istd::CIndex2d spaceSize = space.GetImageSize();

	i2d::CVector2d houghPos = CalcHoughPos(point1, point2, imageCenter, spaceSize);
	int radiusIndex = qMin(qFloor(houghPos.GetX()), spaceSize.GetX() - 1);
	Q_ASSERT(radiusIndex >= 0);

	int angleIndex = qFloor(houghPos.GetY());

	if ((angleIndex >= 0) && (angleIndex < spaceSize.GetY())){
		quint32* angleLinePtr = (quint32*)space.GetLinePtr(angleIndex);
		double kernelFactor = *m_defaultSmoothKernelAttrPtr;
		double posFactor = qSqrt(qAbs(point1.GetCrossProductZ(point2)));
		angleLinePtr[radiusIndex] += int(weight * posFactor * kernelFactor * kernelFactor);
	}
}


// reimplemented (iinsp::TSupplierCompWrap)

int CLensCorrFindSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_bitmapProviderCompPtr.IsValid()){
		return WS_CRITICAL;
	}

	const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
	if (bitmapPtr == NULL){
		AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QObject::tr("Input image could not be provided"), "LensCorrectionFinder"));

		return WS_ERROR;
	}

	Timer performanceTimer(this, "Lens correction finder");

	if (!CalculateCalibration(*bitmapPtr, result)){
		return WS_ERROR;
	}

	return WS_OK;
}


// reimplemented (icomp::CComponentBase)

void CLensCorrFindSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapProviderSupplierCompPtr.GetPtr());
	}
}


// public methods of embedded class FeaturesConsumer

// reimplemented (iipr::IFeaturesConsumer)

void CLensCorrFindSupplierComp::FeaturesConsumer::ResetFeatures()
{
	features.clear();
}


bool CLensCorrFindSupplierComp::FeaturesConsumer::AddFeature(const imeas::INumericValue* featurePtr, bool* /*isFullPtr*/)
{
	if (featurePtr != NULL){
		const i2d::CPosition2d* positionPtr = dynamic_cast<const i2d::CPosition2d*>(featurePtr);
		if (positionPtr != NULL){
			FeatureInfo info;

			info.position = positionPtr->GetPosition();
			info.weight = 1;

			features.push_back(info);
		}
		else{
			imath::CVarVector vector = featurePtr->GetValues();
			if (vector.GetElementsCount() >= 2){
				FeatureInfo info;

				info.position = i2d::CVector2d(vector[0], vector[1]);
				if (vector.GetElementsCount() > 2){
					info.weight = vector[2];
				}
				else{
					info.weight = 1;
				}

				features.push_back(info);
			}
		}
	}

	delete featurePtr;

	return true;
}


} // namespace iipr


