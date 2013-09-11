#include "iipr/CLineProjectionProcessorComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TSmartPtr.h"
#include "i2d/CRectangle.h"
#include "iimg/TPixelConversion.h"
#include "iprm/TParamsPtr.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"
#include "imeas/CSamplesInfo.h"
#include "imeas/INumericValue.h"


namespace iipr
{


// local functions

template <class PixelConversion>
bool ProjectionFunction(
			const quint8* firstPixelAddress,
			const istd::CIndex2d axisSizes,
			const istd::CIndex2d addressDiffs,
			const i2d::CLine2d& bitmapLine,
			const i2d::CLine2d& clippedLine,
			const PixelConversion& conversion,
			imeas::IDataSequence& results)
{
	Q_ASSERT(bitmapLine.GetPoint1().GetX() <= bitmapLine.GetPoint2().GetX());
	Q_ASSERT(clippedLine.GetPoint1().GetX() <= clippedLine.GetPoint2().GetX());
	Q_ASSERT(axisSizes[0] != 0);
	Q_ASSERT(axisSizes[1] != 0);
	Q_ASSERT(addressDiffs[0] != 0);
	Q_ASSERT(addressDiffs[1] != 0);

	i2d::CVector2d delta = clippedLine.GetDiffVector();
	i2d::CVector2d beginPoint = clippedLine.GetPoint1();
	Q_ASSERT(delta.GetX() >= 0);

	int axis1Begin = int(beginPoint.GetX() + I_BIG_EPSILON);
	int axis1End = int(beginPoint.GetX() + delta.GetX() - I_BIG_EPSILON) + 1;
	Q_ASSERT(axis1Begin <= axis1End);
	Q_ASSERT(axis1Begin >= 0);
	Q_ASSERT(axis1End <= axisSizes[0]);

	istd::CChangeNotifier projectionNotifier(&results);

	int projectionSize = axis1End - axis1Begin;
	istd::CRange axis1CutLineRange(bitmapLine.GetPoint1().GetX(), bitmapLine.GetPoint2().GetX());
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
			const quint8* firstLinePixelAddress = firstPixelAddress + (axis1Index * addressDiffs[0]);

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
					Q_ASSERT(alpha >= 0);
					Q_ASSERT(alpha <= 1);

					int axis2Offset = axis2Index * addressDiffs[1];
					const quint8* pixelPtr = (firstLinePixelAddress + axis2Offset);
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
			const i2d::CLine2d& bitmapLine,
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
	const quint8* firstPixelAddress = (const quint8*)bitmap.GetLinePtr(0);
	i2d::CLine2d transformedLine = bitmapLine;
	i2d::CVector2d diffVector = bitmapLine.GetDiffVector();

	if (qAbs(diffVector.GetY()) > qAbs(diffVector.GetX())){	// switch X and Y axis
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
			const imeas::INumericValue& feature,
			const iprm::IParamsSet* paramsPtr,
			i2d::CVector2d& result) const
{
	if (m_featureMapperCompPtr.IsValid() && (paramsPtr != NULL)){
		iprm::TParamsPtr<i2d::CLine2d> linePtr(paramsPtr, *m_lineParamIdAttrPtr);
		double position;
		if (		(linePtr.IsValid()) &&
					m_featureMapperCompPtr->GetProjectionPosition(feature, paramsPtr, position)){
			// TODO: correct exactness of this mapping: DoAutosizeProjection return rough line exactness!
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
	i2d::CLine2d bitmapLine;
	bitmapLine.SetCalibration(bitmap.GetCalibration());
	bitmapLine.CopyFrom(projectionLine, istd::IChangeable::CM_CONVERT);

	return DoAutosizeProjection(bitmap, bitmapLine, results);
}


// reimplemented (iproc::IProcessor)

int CLineProjectionProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	imeas::IDataSequence* projectionPtr = dynamic_cast<imeas::IDataSequence*>(outputPtr);

	if (		(bitmapPtr == NULL) ||
				(projectionPtr == NULL) ||
				(paramsPtr == NULL)){
		return TS_INVALID;
	}

	iprm::TParamsPtr<i2d::CLine2d> linePtr(paramsPtr, *m_lineParamIdAttrPtr);
	if (!linePtr.IsValid()){
		return TS_INVALID;
	}

	return DoProjection(*bitmapPtr, *linePtr, NULL, *projectionPtr)? TS_OK: TS_INVALID;
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

