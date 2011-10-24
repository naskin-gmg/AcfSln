#include "iipr/CLineProjectionProcessorComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TSmartPtr.h"
#include "i2d/CRectangle.h"
#include "iimg/TPixelConversion.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"
#include "imeas/CSamplesInfo.h"

#include "iipr/IFeature.h"


namespace iipr
{


// local functions

template <class PixelConversion>
bool ProjectionFunction(
			const I_BYTE* firstPixelAddress,
			const istd::CIndex2d axisSizes,
			const istd::CIndex2d addressDiffs,
			const i2d::CLine2d& projectionLine,
			const i2d::CLine2d& clippedLine,
			const PixelConversion& conversion,
			imeas::IDataSequence& results)
{
	I_ASSERT(projectionLine.GetPoint1().GetX() <= projectionLine.GetPoint2().GetX());
	I_ASSERT(clippedLine.GetPoint1().GetX() <= clippedLine.GetPoint2().GetX());
	I_ASSERT(axisSizes[0] != 0);
	I_ASSERT(axisSizes[1] != 0);
	I_ASSERT(addressDiffs[0] != 0);
	I_ASSERT(addressDiffs[1] != 0);

	i2d::CVector2d delta = clippedLine.GetDiffVector();
	i2d::CVector2d beginPoint = clippedLine.GetPoint1();
	I_ASSERT(delta.GetX() >= 0);

	int axis1Begin = int(beginPoint.GetX() + I_BIG_EPSILON);
	int axis1End = int(beginPoint.GetX() + delta.GetX() - I_BIG_EPSILON) + 1;
	I_ASSERT(axis1Begin <= axis1End);
	I_ASSERT(axis1Begin >= 0);
	I_ASSERT(axis1End <= axisSizes[0]);

	istd::CChangeNotifier projectionNotifier(&results);

	int projectionSize = axis1End - axis1Begin;
	istd::CRange axis1CutLineRange(projectionLine.GetPoint1().GetX(), projectionLine.GetPoint2().GetX());
	istd::CRange resultProportionRange(
				axis1CutLineRange.GetAlphaFromValue(axis1Begin),
				axis1CutLineRange.GetAlphaFromValue(axis1End));
	if (results.CreateSequenceWithInfo(
				istd::TSmartPtr<const imeas::IDataSequenceInfo>(new imeas::CSamplesInfo(resultProportionRange)),
				projectionSize)){
		double axis2Delta = delta.GetY() / delta.GetX();
		double axis2Position = axis2Delta * (axis1Begin - beginPoint.GetX() + 0.5) + beginPoint.GetY() + 0.5;

		int sampleIndex = 0;
		for (int axis1Index = axis1Begin; axis1Index < axis1End; ++axis1Index, axis2Position += axis2Delta, ++sampleIndex){
			const I_BYTE* firstLinePixelAddress = firstPixelAddress + (axis1Index * addressDiffs[0]);

			int axis2Index = int(axis2Position);
			typename PixelConversion::CalcPixelType value;
			if (axis2Index <= 0){
				value = conversion.GetCalc(*(const typename PixelConversion::SourcePixelType*)(firstLinePixelAddress));
			}
			else{
				if (axis2Index >= axisSizes[1]){
					int axis2Offset = (axisSizes[1] - 1) * addressDiffs[1];
					value = conversion.GetCalc(*(const typename PixelConversion::SourcePixelType*)(firstLinePixelAddress + axis2Offset));
				}
				else{
					double alpha = axis2Position - axis2Index;
					I_ASSERT(alpha >= 0);
					I_ASSERT(alpha <= 1);

					int axis2Offset = axis2Index * addressDiffs[1];
					const I_BYTE* pixelPtr = (firstLinePixelAddress + axis2Offset);
					value =	typename PixelConversion::CalcPixelType(
								conversion.GetCalc(*(const typename PixelConversion::SourcePixelType*)(pixelPtr)) * alpha +
								conversion.GetCalc(*(const typename PixelConversion::SourcePixelType*)(pixelPtr - addressDiffs[1])) * (1 - alpha));
				}
			}

			results.SetSample(sampleIndex, 0, value);
		}

		return true;
	}

	return false;
}


// public methods

bool CLineProjectionProcessorComp::DoAutosizeProjection(
			const iimg::IBitmap& bitmap,
			const i2d::CLine2d& projectionLine,
			imeas::IDataSequence& results) const
{
	istd::CIndex2d axisSizes = bitmap.GetImageSize();
	if (axisSizes.IsSizeEmpty()){
		return false;
	}

	int bytesPerPixel = bitmap.GetPixelBitsCount() >> 3;
	if (bytesPerPixel <= 0){
		return false;
	}

	istd::CIndex2d addressDiffs(bytesPerPixel, bitmap.GetLinesDifference());
	const I_BYTE* firstPixelAddress = (const I_BYTE*)bitmap.GetLinePtr(0);
	i2d::CLine2d transformedLine = projectionLine;
	i2d::CVector2d diffVector = projectionLine.GetDiffVector();

	if (fabs(diffVector.GetY()) > fabs(diffVector.GetX())){	// switch X and Y axis
		axisSizes = istd::CIndex2d(axisSizes[1], axisSizes[0]);
		addressDiffs = istd::CIndex2d(addressDiffs[1], addressDiffs[0]);
		transformedLine.SetPoint1(i2d::CVector2d(
					transformedLine.GetPoint1().GetY(),
					transformedLine.GetPoint1().GetX()));
		transformedLine.SetPoint2(i2d::CVector2d(
					transformedLine.GetPoint2().GetY(),
					transformedLine.GetPoint2().GetX()));

		diffVector = i2d::CVector2d(diffVector[1], diffVector[0]);
	}

	if (diffVector[0] < 0){	// mirror X axis
		firstPixelAddress += addressDiffs[0] * (axisSizes[0] - 1);
		addressDiffs[0] = -addressDiffs[0];
		transformedLine.SetPoint1(i2d::CVector2d(
					axisSizes[0] - transformedLine.GetPoint1().GetX(),
					transformedLine.GetPoint1().GetY()));
		transformedLine.SetPoint2(i2d::CVector2d(
					axisSizes[0] - transformedLine.GetPoint2().GetX(),
					transformedLine.GetPoint2().GetY()));
	}

	i2d::CLine2d clippedLine = transformedLine.GetClipped(i2d::CRectangle(axisSizes));

	iimg::CGrayFloatPixelConversion conversion;
	return ProjectionFunction(
				firstPixelAddress,
				axisSizes,
				addressDiffs,
				transformedLine,
				clippedLine,
				conversion,
				results);
}


// reimplemented (iipr::IFeatureToImageMapper)

bool CLineProjectionProcessorComp::GetImagePosition(
			const IFeature& feature,
			const iprm::IParamsSet* paramsPtr,
			i2d::CVector2d& result) const
{
	const std::string& lineParamId = (*m_lineParamIdAttrPtr).ToString();

	if (m_featureMapperCompPtr.IsValid() && (paramsPtr != NULL) && !lineParamId.empty()){
		const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(paramsPtr->GetParameter(lineParamId));
		double position;
		if (		(linePtr != NULL) &&
					m_featureMapperCompPtr->GetProjectionPosition(feature, paramsPtr, position)){
			result = linePtr->GetPositionFromAlpha(position);

			return true;
		}
	}

	return false;
}


// reimplemented (iipr::ILineProjectionProcessor)

bool CLineProjectionProcessorComp::DoProjection(
			const iimg::IBitmap& bitmap,
			const i2d::CLine2d& projectionLine,
			const IProjectionParams* /*paramsPtr*/,
			imeas::IDataSequence& results)
{
	return DoAutosizeProjection(bitmap, projectionLine, results);
}


// reimplemented (iproc::IProcessor)

int CLineProjectionProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	imeas::IDataSequence* projectionPtr = dynamic_cast<imeas::IDataSequence*>(outputPtr);

	const std::string& lineParamId = (*m_lineParamIdAttrPtr).ToString();

	if (		(bitmapPtr == NULL) ||
				(projectionPtr == NULL) ||
				(paramsPtr == NULL) ||
				lineParamId.empty()){
		return TS_INVALID;
	}

	const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(paramsPtr->GetParameter(lineParamId));
	if (linePtr == NULL){
		return TS_INVALID;
	}

	return DoAutosizeProjection(*bitmapPtr, *linePtr, *projectionPtr)? TS_OK: TS_INVALID;
}


// reimplemented (iipr::IProjectionConstraints)

istd::CRange CLineProjectionProcessorComp::GetLineWidthRange() const
{
	return istd::CRange(-1, -1);
}


int CLineProjectionProcessorComp::GetMinProjectionSize() const
{
	return -1;
}


int CLineProjectionProcessorComp::GetMaxProjectionSize() const
{
	return -1;
}


bool CLineProjectionProcessorComp::IsAutoProjectionSizeSupported() const
{
	return true;
}


} // namespace iipr

