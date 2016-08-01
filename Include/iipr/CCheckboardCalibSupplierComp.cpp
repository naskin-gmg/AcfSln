#include "iipr/CCheckboardCalibSupplierComp.h"


// ACF include
#include "ilog/TExtMessage.h"
#include "i2d/CLine2d.h"
#include "i2d/CPosition2d.h"
#include "i2d/CPolypoint.h"
#include "iprm/TParamsPtr.h"

// ACF-Solutions includes
#include "iipr/CHoughSpace2d.h"
#include "iipr/CPerspCalibFinder.h"
#include "iimg/CBitmap.h"


namespace iipr
{


CCheckboardCalibSupplierComp::CCheckboardCalibSupplierComp()
:	m_checkboardParamContraints(this)
{
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CCheckboardCalibSupplierComp::GetCalibration() const
{
	return GetWorkProduct();
}


//protected methods

bool CCheckboardCalibSupplierComp::CalculateCalibration(const iimg::IBitmap& image, i2d::CPerspectiveCalibration2d& result) const
{
	int gridSize = *m_defaultGridSizeAttrPtr;
	double cellSize = *m_defaultCellSizeAttrPtr;
	const iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();
	iprm::TParamsPtr<imeas::INumericValue> checkboardParamsPtr(paramsSetPtr, m_checkboardParamsId, m_defaultCheckboardParamsCompPtr);
	if (checkboardParamsPtr.IsValid()){
		imath::CVarVector params = checkboardParamsPtr->GetValues();
		if (params.GetElementsCount() >= 1){
			gridSize = int(params.GetElement(0));
		}
		if (params.GetElementsCount() >= 2){
			cellSize = params.GetElement(1);
		}
	}

	int linesPerVanPoint = gridSize - 1;	// number of bidirectional lines per vanishing point

	result.Reset();

	if (!m_lineFinderCompPtr.IsValid()){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_CRITICAL, 0, QObject::tr("No processor set"), "CheckboardCalibSupplier"));

		return false;
	}

	LinesConsumer processedLines;
	if (m_lineFinderCompPtr->DoExtractFeatures(GetModelParametersSet(), image, processedLines) != iproc::IProcessor::TS_OK){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 0, QObject::tr("Cannot extract lines from image"), "CheckboardCalibSupplier"));

		return false;
	}

	QMultiMap<double, i2d::CLine2d> filteredLines;
	for (		Lines::ConstIterator iter1 = processedLines.lines.constBegin();
				iter1 != processedLines.lines.constEnd();
				++iter1){
		const i2d::CLine2d& line1 = iter1->line;

		for (		Lines::ConstIterator iter2 = iter1 + 1;
					iter2 != processedLines.lines.constEnd();
					++iter2){
			const i2d::CLine2d& line2 = iter2->line;

			double distance =
						line1.GetExtendedDistance(line2.GetPoint1()) +
						line1.GetExtendedDistance(line2.GetPoint2()) +
						line2.GetExtendedDistance(line1.GetPoint1()) +
						line2.GetExtendedDistance(line1.GetPoint2());

			if (distance < 10){
				i2d::CLine2d commonLine((line1.GetPoint1() + line2.GetPoint2()) * 0.5, (line1.GetPoint2() + line2.GetPoint1()) * 0.5);
				double commonWeight = iter1->weight * iter2->weight;

				filteredLines.insert(commonWeight, commonLine);
			}
		}
	}

	if (filteredLines.size() < linesPerVanPoint * 2){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 0, QObject::tr("No enough lines found"), "CheckboardCalibSupplier"));

		return false;
	}

	// search for vanishing points
	istd::CIndex2d vanSpaceSize(100, 100);
	double vanDistOffset = vanSpaceSize.GetY() * 0.5;
	double vanDistScale = 40000;

	iipr::CHoughSpace2d vanishingSpace;
	vanishingSpace.CreateHoughSpace(vanSpaceSize, true, false);

	i2d::CVector2d imageCenter = image.GetCenter();

	for (		QMultiMap<double, i2d::CLine2d>::ConstIterator iter1 = filteredLines.constBegin();
				iter1 != filteredLines.constEnd();
				++iter1){
		const i2d::CLine2d& line1 = iter1.value();

		for (		QMultiMap<double, i2d::CLine2d>::ConstIterator iter2 = iter1 + 1;
					iter2 != filteredLines.constEnd();
					++iter2){
			const i2d::CLine2d& line2 = iter2.value();

			i2d::CVector2d vanSpacePos;
			i2d::CVector2d vanishingPoint;
			if (line1.GetExtendedIntersection(line2, vanishingPoint)){
				i2d::CVector2d diffVector = vanishingPoint - imageCenter;

				vanSpacePos.SetX(vanSpaceSize.GetX() * (diffVector.GetAngle() / I_2PI + 1));
				vanSpacePos.SetY(vanDistOffset + vanDistScale / diffVector.GetLength());
			}
			else{
				vanSpacePos.SetX(vanSpaceSize.GetX() * (line2.GetDiffVector().GetAngle() / I_2PI + 1));
				vanSpacePos.SetY(vanDistOffset);
			}

			vanishingSpace.IncreaseValueAt(vanSpacePos, 1);
		}
	}
/*
	{
		ilog::TExtMessageModel<iimg::CBitmap>* spaceMessagePtr = new ilog::TExtMessageModel<iimg::CBitmap>(
					istd::IInformationProvider::IC_INFO,
					0,
					QString("Vanishing Hough space"),
					"LineFinder");

		vanishingSpace.ExtractToBitmap(*spaceMessagePtr);

		AddMessage(spaceMessagePtr, MCT_TEMP);
	}
*/
	iipr::CHoughSpace2d::WeightToHoughPosMap foundVanPoints;
	vanishingSpace.AnalyseHoughSpace(2, 2, 0.2, 2, 0.1, foundVanPoints);
	Q_ASSERT(foundVanPoints.size() <= 2);
	if (foundVanPoints.size() < 2){
		AddMessage(new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 0, QObject::tr("No vanishing points found"), "CheckboardCalibSupplier"));

		return false;
	}

	QMap<double, i2d::CLine2d> sortedVanLines[2];

	int vanishingPointIndex = 0;
	for (		iipr::CHoughSpace2d::WeightToHoughPosMap::ConstIterator foundVanIter = foundVanPoints.constBegin();
				foundVanIter != foundVanPoints.constEnd();
				++foundVanIter, ++vanishingPointIndex){
		const i2d::CVector2d& foundVanSpacePos = foundVanIter.value();

		QSet<i2d::CLine2d> vanLines;
		i2d::CVector2d exactVanCummPos(0, 0);
		int exactCummPosCount = 0;
		
		for (		QMultiMap<double, i2d::CLine2d>::ConstIterator iter1 = filteredLines.constBegin();
					iter1 != filteredLines.constEnd();
					++iter1){
			const i2d::CLine2d& line1 = iter1.value();

			for (		QMultiMap<double, i2d::CLine2d>::ConstIterator iter2 = iter1 + 1;
						iter2 != filteredLines.constEnd();
						++iter2){
				const i2d::CLine2d& line2 = iter2.value();

				i2d::CVector2d vanSpacePos;
				i2d::CVector2d vanishingPoint;
				if (line1.GetExtendedIntersection(line2, vanishingPoint)){
					i2d::CVector2d diffVector = vanishingPoint - imageCenter;
					i2d::CVector2d  rawPosition(
								vanSpaceSize.GetX() * (diffVector.GetAngle() / I_2PI + 1),
								vanDistOffset + vanDistScale / diffVector.GetLength());

					if (!vanishingSpace.GetSpacePosition(rawPosition, vanSpacePos)){
						continue;
					}
				}
				else{
					i2d::CVector2d  rawPosition(
								vanSpaceSize.GetX() * (line2.GetDiffVector().GetAngle() / I_2PI + 1),
								vanDistOffset);

					if (!vanishingSpace.GetSpacePosition(rawPosition, vanSpacePos)){
						continue;
					}
				}

				if (vanSpacePos.GetDistance(foundVanSpacePos) < 5){
					vanLines.insert(line1);
					vanLines.insert(line2);

					exactVanCummPos += vanSpacePos;
					++exactCummPosCount;
				}
			}
		}

		i2d::CVector2d exactVanPos = exactVanCummPos / exactCummPosCount;

		i2d::CVector2d vanVector;
		vanVector.Init(exactVanPos.GetX() * I_2PI / vanSpaceSize.GetX(), vanDistScale / (exactVanPos.GetY() - vanDistOffset));

		i2d::CVector2d vanPoint = imageCenter + vanVector;

		for (		QSet<i2d::CLine2d>::ConstIterator iter = vanLines.constBegin();
					iter != vanLines.constEnd();
					++iter){
			const i2d::CLine2d& line = *iter;

			i2d::CVector2d diff = line.GetDiffVector();
			double projection1Value = diff.GetCrossProductZ(line.GetPoint1()) / diff.GetLength();
			if (diff.GetDotProduct(vanVector) < 0){
				projection1Value = -projection1Value;
			}

			sortedVanLines[vanishingPointIndex][projection1Value] = line;
		}

		int lineIndex = 0;
		for (		QMap<double, i2d::CLine2d>::ConstIterator iter = sortedVanLines[vanishingPointIndex].constBegin();
					iter != sortedVanLines[vanishingPointIndex].constEnd();
					++iter, ++lineIndex){
			const i2d::CLine2d& line = iter.value();

			ilog::TExtMessageModel<i2d::CLine2d>* pointMessagePtr = new ilog::TExtMessageModel<i2d::CLine2d>(
						istd::IInformationProvider::IC_INFO,
						0,
						QString("Vanishing point %1, line %1").arg(vanishingPointIndex + 1).arg(lineIndex + 1),
						"CheckboardCalibrator");
			pointMessagePtr->SetPoint1(vanPoint);
			pointMessagePtr->SetPoint2(line.GetCenter());

			AddMessage(pointMessagePtr, MCT_TEMP);
		}

		if (sortedVanLines[vanishingPointIndex].size() != linesPerVanPoint){
			AddMessage(new ilog::CMessage(
						istd::IInformationProvider::IC_ERROR,
						0,
						QObject::tr("Expected %1 lines for vanishing point %2, but found %3").arg(linesPerVanPoint).arg(vanishingPointIndex + 1).arg(sortedVanLines[vanishingPointIndex].size()),
						"CheckboardCalibSupplier"));

			return false;
		}
	}

	QVector<i2d::CVector2d> crossPositions(linesPerVanPoint * linesPerVanPoint);
	QVector<i2d::CVector2d> nominalPositions(linesPerVanPoint * linesPerVanPoint);

	ilog::TExtMessageModel<i2d::CPolypoint>* crossPointsMessagePtr = new ilog::TExtMessageModel<i2d::CPolypoint>(
				istd::IInformationProvider::IC_INFO,
				0,
				"",
				"CheckboardCalibrator");

	int line1Index = 0;
	for (		QMap<double, i2d::CLine2d>::ConstIterator iter1 = sortedVanLines[0].constBegin();
				iter1 != sortedVanLines[0].constEnd();
				++iter1, ++line1Index){
		const i2d::CLine2d& line1 = iter1.value();

		int line2Index = 0;
		for (		QMap<double, i2d::CLine2d>::ConstIterator iter2 = sortedVanLines[1].constBegin();
					iter2 != sortedVanLines[1].constEnd();
					++iter2, ++line2Index){
			const i2d::CLine2d& line2 = iter2.value();

			i2d::CVector2d crossPoint;
			line1.GetExtendedIntersection(line2, crossPoint);
			crossPositions[line2Index * linesPerVanPoint + line1Index] = crossPoint;

			i2d::CVector2d normalPos((line1Index - (linesPerVanPoint - 1) * 0.5) * cellSize, (line2Index - (linesPerVanPoint - 1) * 0.5) * cellSize);
			nominalPositions[line2Index * linesPerVanPoint + line1Index] = normalPos;

			crossPointsMessagePtr->InsertNode(crossPoint);
		}
	}

	iipr::CPerspCalibFinder calibFinder;
	bool retVal = calibFinder.FindPerspCalib(nominalPositions.constData(), crossPositions.constData(), linesPerVanPoint * linesPerVanPoint, result);

	if (retVal){
		crossPointsMessagePtr->SetText(QObject::tr("Calibration successfull found"));
	}
	else{
		crossPointsMessagePtr->SetText(QObject::tr("Calibration not found"));
	}

	AddMessage(crossPointsMessagePtr, MCT_RESULTS);

	return retVal;
}


// reimplemented (iinsp::TSupplierCompWrap)

int CCheckboardCalibSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_bitmapProviderCompPtr.IsValid()){
		return WS_CRITICAL;
	}

	const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
	if (bitmapPtr == NULL){
		AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QObject::tr("Input image could not be provided"), "CheckboardCalibSupplier"));

		return WS_ERROR;
	}

	Timer performanceTimer(this, "Checkboard calibration");

	if (!CalculateCalibration(*bitmapPtr, result)){
		return WS_ERROR;
	}

	return WS_OK;
}


// reimplemented (icomp::CComponentBase)

void CCheckboardCalibSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapProviderSupplierCompPtr.GetPtr());
	}
}


// public methods of embedded class LinesConsumer

// reimplemented (iipr::IFeaturesConsumer)

void CCheckboardCalibSupplierComp::LinesConsumer::ResetFeatures()
{
	lines.clear();
}


bool CCheckboardCalibSupplierComp::LinesConsumer::AddFeature(const imeas::INumericValue* featurePtr, bool* /*isFullPtr*/)
{
	if (featurePtr != NULL){
		const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(featurePtr);
		if (linePtr != NULL){
			LineInfo info;

			info.line = *linePtr;
			info.weight = 1;

			lines.push_back(info);
		}

		imath::CVarVector vector = featurePtr->GetValues();
		if (vector.GetElementsCount() >= 4){
			LineInfo info;

			info.line = i2d::CLine2d(vector[0], vector[1], vector[2], vector[3]);
			if (vector.GetElementsCount() >= 5){
				info.weight = vector[5];
			}
			else{
				info.weight = 1;
			}

			lines.push_back(info);
		}
	}

	delete featurePtr;

	return true;
}


// public methods of embedded class ChessboardParamsContraints

CCheckboardCalibSupplierComp::ChessboardParamsContraints::ChessboardParamsContraints(const CCheckboardCalibSupplierComp* parentPtr)
:	m_parentPtr(parentPtr),
	m_cellSizeUnit(imath::IUnitInfo::UT_PHYSICAL, "", 1, istd::CRange(0, 100))
{
	Q_ASSERT(m_parentPtr != NULL);

	if (m_parentPtr->m_distanceUnitInfoCompPtr.IsValid()){
		m_cellSizeUnit.SetUnitName(m_parentPtr->m_distanceUnitInfoCompPtr->GetUnitName());
		m_cellSizeUnit.SetUnitType(m_parentPtr->m_distanceUnitInfoCompPtr->GetUnitType());
		m_cellSizeUnit.SetDisplayMultiplicationFactor(m_parentPtr->m_distanceUnitInfoCompPtr->GetDisplayMultiplicationFactor());
	}
}


// reimplemented (imeas::INumericConstraints)

int CCheckboardCalibSupplierComp::ChessboardParamsContraints::GetNumericValuesCount() const
{
	return 2;
}


QString CCheckboardCalibSupplierComp::ChessboardParamsContraints::GetNumericValueName(int index) const
{
	if (index != 0){
		return QObject::tr("Cell Size");
	}
	else{
		return QObject::tr("Grid Size");
	}
}


QString CCheckboardCalibSupplierComp::ChessboardParamsContraints::GetNumericValueDescription(int index) const
{
	if (index != 0){
		return QObject::tr("Size of single cell");
	}
	else{
		return QObject::tr("Number of grid cells in each checkboard row and column");
	}
}


const imath::IUnitInfo* CCheckboardCalibSupplierComp::ChessboardParamsContraints::GetNumericValueUnitInfo(int index) const
{
	if (index != 0){
		return &m_cellSizeUnit;
	}
	else{
		return &s_gridSizeUnit;
	}
}


// static private attributes

imath::CFixedPointManip CCheckboardCalibSupplierComp::s_gridSizeValueManip(0);
imath::CGeneralUnitInfo CCheckboardCalibSupplierComp::s_gridSizeUnit(imath::IUnitInfo::UT_COUNTER, "", 1.0, istd::CRange(1, 30), &s_gridSizeValueManip);


} // namespace iipr


