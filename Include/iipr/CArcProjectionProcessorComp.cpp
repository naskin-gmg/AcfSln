#include "iipr/CArcProjectionProcessorComp.h"


// Qt includes
#include <QtCore/QLineF>

// ACF includes
#include "istd/TSmartPtr.h"
#include "i2d/CRectangle.h"
#include "iimg/TPixelConversion.h"
#include "iprm/TParamsPtr.h"
#include "i2d/CVector2d.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"
#include "imeas/CSamplesInfo.h"
#include "imeas/INumericValue.h"


namespace iipr
{


// reimplemented (iipr::IArcProjectionProcessor)

bool CArcProjectionProcessorComp::DoProjection(
			const iimg::IBitmap& bitmap,
			const i2d::CArc& projectionLine,
			const IProjectionParams* /*paramsPtr*/,
			imeas::IDataSequence& results)
{
	i2d::CArc bitmapArc;
	bitmapArc.SetCalibration(bitmap.GetCalibration());
	bitmapArc.CopyFrom(projectionLine, istd::IChangeable::CM_CONVERT);

	double angleWidth = bitmapArc.GetEndAngle() - bitmapArc.GetStartAngle();

	int arcLength = M_PI * bitmapArc.GetRadius() * (angleWidth / 180);

	if (arcLength == 0) {
		return false;
	}
	else if (arcLength < 0) {
		arcLength *= -1;
	}

	results.CreateSequence(arcLength);

	double angleDiff = angleWidth / arcLength;

	double angleStart = bitmapArc.GetStartAngle();
	double radius = bitmapArc.GetRadius();
	i2d::CVector2d center = bitmapArc.GetPosition();
	i2d::CVector2d rotationPoint = center + i2d::CVector2d(radius, 0);

	double angle;
	int i;

	for (angle = angleStart, i = 0; angle < (angleStart + angleWidth), i < arcLength; angle += angleDiff, i++) {
		i2d::CVector2d line(0,0);
		line.Init(imath::GetRadianFromDegree(-angle), radius);
		line += center;

		double x = line.GetX();
		double y = line.GetY();

		Q_ASSERT(x > 0);
		Q_ASSERT(y > 0);

		double dummy;
		double deltaX = modf(x, &dummy);
		double deltaXNeg = 1-deltaX;
		double deltaY = modf(y, &dummy);
		double deltaYNeg = 1-deltaY;

		quint8 grayAll = 0, grayCenter, grayVertical = 0, grayHorizontal = 0, grayScew = 0;
		grayCenter = ((quint8*)bitmap.GetLinePtr((int)y))[(int)x];

		if (deltaX == 0 && deltaY == 0) {
			grayAll = grayCenter;
		}
		else if(deltaX == 0){
			grayHorizontal = ((quint8*)bitmap.GetLinePtr((int)y+(deltaY > 0 ? 1 : -1)))[(int)x];

			grayAll = (grayCenter * deltaY) + (grayHorizontal * deltaYNeg);
		}
		else if(deltaY == 0){
			grayVertical = ((quint8*)bitmap.GetLinePtr((int)y))[(int)x+(deltaX > 0 ? 1 : -1)];

			grayAll = (grayCenter * deltaX) + (grayVertical * deltaXNeg);
		}
		else{
			grayVertical = ((quint8*)bitmap.GetLinePtr((int)y))[(int)x+(deltaX > 0 ? 1 : -1)];
			grayHorizontal = ((quint8*)bitmap.GetLinePtr((int)y+(deltaY > 0 ? 1 : -1)))[(int)x];
			grayScew = ((quint8*)bitmap.GetLinePtr((int)y+(deltaY > 0 ? 1 : -1)))[(int)x+(deltaX > 0 ? 1 : -1)];

			grayAll = (grayCenter * deltaX * deltaY) + (grayVertical * deltaXNeg * deltaY) + (grayHorizontal * deltaX * deltaYNeg) + (grayScew * deltaXNeg * deltaYNeg);
		}

		results.SetSample(i, 0, grayAll);
	}

	return true;
}


// reimplemented (iipr::IFeatureToImageMapper)

bool CArcProjectionProcessorComp::GetImagePosition(
			const imeas::INumericValue& feature,
			const iprm::IParamsSet* paramsPtr,
			i2d::CVector2d& result) const
{
	if (m_featureMapperCompPtr.IsValid() && (paramsPtr != NULL)){
		iprm::TParamsPtr<i2d::CArc> arcPtr(paramsPtr, *m_arcParamIdAttrPtr);
		double position;
		if (		(arcPtr.IsValid()) &&
			m_featureMapperCompPtr->GetProjectionPosition(feature, paramsPtr, position)){
				// TODO: correct exactness of this mapping: DoAutosizeProjection return rough line exactness!
				result = arcPtr->GetPositionFromAlpha(position);

				return true;
		}
	}

	return false;
}


// reimplemented (iproc::IProcessor)

int CArcProjectionProcessorComp::DoProcessing(
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

	iprm::TParamsPtr<i2d::CArc> arcPtr(paramsPtr, *m_arcParamIdAttrPtr);
	if (!arcPtr.IsValid()){
		return TS_INVALID;
	}

	return DoProjection(*bitmapPtr, *arcPtr, NULL, *projectionPtr)? TS_OK: TS_INVALID;
}


} // namespace iipr


