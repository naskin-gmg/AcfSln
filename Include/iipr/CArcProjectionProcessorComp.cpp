#include "iipr/CArcProjectionProcessorComp.h"


// Qt includes
#include <QtCore/QLineF>
#include <QtCore/QElapsedTimer>
#include <QtCore/QDebug>

// ACF includes
#include "istd/TSmartPtr.h"
#include "i2d/CRectangle.h"
#include "iimg/TPixelConversion.h"
#include "iprm/TParamsPtr.h"
#include "i2d/CVector2d.h"
#include "i2d/CArcPointsCalculator.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"
#include "imeas/CSamplesInfo.h"
#include "imeas/INumericValue.h"
#include "iipr/TImagePixelInterpolator.h"


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

	double radius = bitmapArc.GetRadius();
	i2d::CVector2d center = bitmapArc.GetPosition();

	iipr::TImagePixelInterpolator<quint8> pixelInterpolator(bitmap, iipr::IImageInterpolationParams::IM_BILINEAR);

	std::vector<int> xPoints, yPoints;
	i2d::CArcPointsCalculator::GetArcPoints(
				center.GetX(),
				center.GetY(), 
				radius,
				bitmapArc.GetStartAngle(), bitmapArc.GetEndAngle(), 
				xPoints,
				yPoints);

	results.CreateSequence(xPoints.size());

	for (uint i = 0; i < xPoints.size(); ++i){
		quint8 pixelValue = pixelInterpolator.GetInterpolatedValue(xPoints[i], yPoints[i], 0);

		results.SetSample(i, 0, pixelValue / 255.0);
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


// reimplemented (icomp::CComponentBase)

void CArcProjectionProcessorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// Force components initialization
	m_featureMapperCompPtr.EnsureInitialized();
}


} // namespace iipr


