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
	istd::CIndex2d lineSpaceSize(radiusGridSize, angleGridSize);

	if (!lineSpace.CreateHoughSpace(lineSpaceSize, false, true)){
		return false;
	}

	for (		Features::ConstIterator iter1 = consumer.features.constBegin();
				iter1 != consumer.features.constEnd();
				++iter1){
		const FeatureInfo& feature1 = *iter1;

		for (		Features::ConstIterator iter2 = iter1 + 1;
					iter2 != consumer.features.constEnd();
					++iter2){
			const FeatureInfo& feature2 = *iter2;

			UpdateHoughSpace(feature1.position, feature2.position, feature1.weight * feature2.weight, lineSpace);
		}
	}

	istd::CIndex2d imageSize = image.GetImageSize();
	i2d::CVector2d opticalCenter(imageSize.GetX() * 0.5, imageSize.GetY() * 0.5);

	lineSpace.SmoothHoughSpace(*m_defaultSmoothKernelAttrPtr);

	iipr::CHoughSpace2d::WeightToHoughPosMap foundLines;
	lineSpace.AnalyseHoughSpace(*m_defaultMaxLinesAttrPtr, 3, 0.5, 10.0, 0.2, foundLines);
	if (foundLines.size() < 2){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 0, QObject::tr("No lines found"), "LensCorrectionFinder"));

		return false;
	}

	istd::TDelPtr<ilog::CExtMessage> linesMessagePtr = new ilog::CExtMessage(
				istd::IInformationProvider::IC_INFO,
				0,
				"",
				"LensCorrectionFinder");

	QList<CorrectionLineInfo> allCorrectionInfos;
	int allPointsCount = 0;

	for (		iipr::CHoughSpace2d::WeightToHoughPosMap::ConstIterator houghIter = foundLines.constBegin();
				houghIter != foundLines.constEnd();
				++houghIter){
		const i2d::CVector2d& foundSpacePos = houghIter.value();

		i2d::CLine2d line = CalcCorrespondingLine(foundSpacePos, lineSpaceSize);
		
		QSet<i2d::CVector2d> positionsOnLine;

		for (		Features::ConstIterator featureIter = consumer.features.constBegin();
					featureIter != consumer.features.constEnd();
					++featureIter){
			const FeatureInfo& feature = *featureIter;

			if (line.GetExtendedDistance(foundSpacePos) < *m_defaultSmoothKernelAttrPtr){
				positionsOnLine.insert(feature.position);
			}
		}

		if (positionsOnLine.size() >= 3){
			// calulate of position center
			i2d::CVector2d sumPos(0, 0);
			for (		QSet<i2d::CVector2d>::ConstIterator pointIter = positionsOnLine.constBegin();
						pointIter != positionsOnLine.constEnd();
						++pointIter){
				const i2d::CVector2d& pos = *pointIter;

				sumPos += pos;
			}
			i2d::CVector2d centerPos = sumPos / positionsOnLine.size();

			// calulate correlation matrix
			i2d::CMatrix2d correlationMatrix(0, 0, 0, 0);
			for (		QSet<i2d::CVector2d>::ConstIterator pointIter = positionsOnLine.constBegin();
						pointIter != positionsOnLine.constEnd();
						++pointIter){
				const i2d::CVector2d& pos = *pointIter;

				i2d::CVector2d diff = pos - centerPos;
				correlationMatrix.GetAtRef(0, 0) += diff.GetX() * diff.GetX();
				correlationMatrix.GetAtRef(0, 1) += diff.GetX() * diff.GetY();
				correlationMatrix.GetAtRef(1, 1) += diff.GetY() * diff.GetY();
			}
			correlationMatrix.SetAt(1, 0, correlationMatrix.GetAt(1, 0));

			// take first principial vector
			i2d::CVector2d eigenVector1;
			i2d::CVector2d eigenVector2;
			double eigenValue1;
			double eigenValue2;
			if (!correlationMatrix.GetEigenVectors(eigenVector1, eigenVector2, eigenValue1, eigenValue2)){
				continue;
			}
			eigenVector1.Normalize(eigenValue1);

			i2d::CLine2d representationLine(centerPos - eigenVector1, centerPos - eigenVector2);

			i2d::CLine2d* lineObjectPtr = new imod::TModelWrap<i2d::CLine2d>();
			*lineObjectPtr = representationLine;
			linesMessagePtr->InsertAttachedObject(lineObjectPtr, QObject::tr("Detected line %1").arg(allCorrectionInfos.size() + 1));

			CorrectionLineInfo lineInfo;
			lineInfo.orthoVector = representationLine.GetNearestPoint(i2d::CVector2d::GetZero());

			bool isLineUsefull = true;
			for (		QSet<i2d::CVector2d>::ConstIterator pointIter = positionsOnLine.constBegin();
						pointIter != positionsOnLine.constEnd();
						++pointIter){
				const i2d::CVector2d& pos = *pointIter;

				if (lineInfo.orthoVector.GetDotProduct(pos) < I_BIG_EPSILON){	// there are point on this line lying of the other side
					isLineUsefull = false;
					break;
				}

				CorrectionInfo info;
				info.position = pos - opticalCenter;
				info.diff = representationLine.GetExtendedNearestPoint(pos) - pos;

				lineInfo.infos.push_back(info);

				allPointsCount++;
			}

			if (!isLineUsefull){
				continue;
			}

			allCorrectionInfos.push_back(lineInfo);
		}
	}

	int varCount = allCorrectionInfos.size() * 2 + 2;
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

		for (		CorrectionInfos::ConstIterator pointIter = lineInfo.infos.constBegin();
					pointIter != lineInfo.infos.constEnd();
					++pointIter, ++pointIndex){
			const CorrectionInfo& pointInfo = *pointIter;

			double positionDist = pointInfo.position.GetLength();

			solutionMatrix.SetAt(istd::CIndex2d(0, pointIndex * 2), pointInfo.position.GetX());
			solutionMatrix.SetAt(istd::CIndex2d(1, pointIndex * 2), pointInfo.position.GetX() * positionDist);
			solutionMatrix.SetAt(istd::CIndex2d(2 + lineIndex, pointIndex * 2), -lineInfo.orthoVector.GetX());
			vectorY.SetAt(istd::CIndex2d(0, pointIndex * 2), pointInfo.position.GetX() + pointInfo.diff.GetX());

			solutionMatrix.SetAt(istd::CIndex2d(0, pointIndex * 2 + 1), pointInfo.position.GetY());
			solutionMatrix.SetAt(istd::CIndex2d(1, pointIndex * 2 + 1), pointInfo.position.GetY() * positionDist);
			solutionMatrix.SetAt(istd::CIndex2d(2 + lineIndex, pointIndex * 2 + 1), -lineInfo.orthoVector.GetY());
			vectorY.SetAt(istd::CIndex2d(0, pointIndex * 2 + 1), pointInfo.position.GetY() + pointInfo.diff.GetY());
		}
	}

	imath::CVarMatrix resolvedX;
	if (!solutionMatrix.GetSolvedLSP(vectorY, resolvedX)){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 0, QObject::tr("Cannot resolve equations"), "LensCorrectionFinder"));

		return false;
	}

	Q_ASSERT(resolvedX.GetSizes() == istd::CIndex2d(1, varCount));

	double scale = resolvedX.GetAt(istd::CIndex2d(0, 0));
	double distFactor = resolvedX.GetAt(istd::CIndex2d(0, 1)) / scale;
	result.SetDistortionFactor(distFactor);
	result.SetOpticalCenter(opticalCenter);

	ilog::CExtMessage* resultMessagePtr = new ilog::CExtMessage(
				istd::IInformationProvider::IC_INFO,
				0,
				QObject::tr("Found lens coeeficients: dist=%1, scale=%2").arg(distFactor).arg(scale),
				"LensCorrectionFinder");
	AddMessage(resultMessagePtr, MCT_RESULTS);

	return true;
}


i2d::CVector2d CLensCorrFindSupplierComp::CalcHoughPos(const i2d::CVector2d& point1, const i2d::CVector2d& point2, const istd::CIndex2d& spaceSize) const
{
	i2d::CVector2d angleVector = (point2 - point1).GetNormalized();
	i2d::CVector2d spaceCenter(spaceSize.GetX() * 0.5, spaceSize.GetY() * 0.5);
	double directionAngle = angleVector.GetAngle();

	return i2d::CVector2d(std::fmod(directionAngle / I_2PI + 10, 1) * spaceSize.GetY(), angleVector.GetDotProduct(point1 - spaceCenter));
}


void CLensCorrFindSupplierComp::UpdateHoughSpace(const i2d::CVector2d& point1, const i2d::CVector2d& point2, double weight, CHoughSpace2d& space) const
{
	istd::CIndex2d spaceSize = space.GetImageSize();

	i2d::CVector2d houghPos = CalcHoughPos(point1, point2, spaceSize);
	int radiusIndex = qFloor(houghPos.GetX());
	int angleIndex = qFloor(houghPos.GetY());

	quint32* angleLinePtr = (quint32*)space.GetLinePtr(angleIndex);
	angleLinePtr[radiusIndex] += int(weight);
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

	delete featurePtr;

	return true;
}


} // namespace iipr


