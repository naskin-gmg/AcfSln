#include "iipr/CImagePolarTransformProcessorComp.h"


 // ACF includes
#include "istd/TChangeNotifier.h"
#include "ibase/CSize.h"
#include "iimg/IBitmap.h"
#include "iimg/CScanlineMask.h"


// IACF includes
#include "iipr/TImagePixelInterpolator.h"


namespace iipr
{


// protected methods

// reimplemented (CImageRegionProcessorCompBase)

bool CImagePolarTransformProcessorComp::ProcessImageRegion(
			const iimg::IBitmap& inputBitmap,
			const iprm::IParamsSet* /*paramsPtr*/,
			const i2d::IObject2d* aoiPtr,
			istd::IChangeable* outputPtr) const
{
	if (inputBitmap.IsEmpty()){
		return true;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		return false;
	}

	const i2d::IObject2d* realAoiPtr = aoiPtr;
	i2d::CRectangle imageRect(inputBitmap.GetImageSize());
	if (aoiPtr == NULL){
		realAoiPtr = &imageRect;
	}

	iimg::CScanlineMask bitmapRegion;
	i2d::CRect clipArea(inputBitmap.GetImageSize());
	if (!bitmapRegion.CreateFromGeometry(*realAoiPtr, &clipArea)){
		return false;
	}

	if (bitmapRegion.IsBitmapRegionEmpty()){
		SendWarningMessage(0, "Cannot process an empty region");
		
		return false;
	}

	i2d::CRect regionRect = bitmapRegion.GetBoundingBox();
	i2d::CVector2d aoiCenter = regionRect.GetCenter();
	i2d::CVector2d diffVector = aoiCenter - i2d::CVector2d(regionRect.GetLeftTop());
	int radius = int(qCeil(diffVector.GetLength()));
	int angleRange = int(radius * I_PI + 0.5);

	int r1 = 0;
	int r2 = radius;

	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(realAoiPtr);
	if (annulusPtr != NULL){
		r1 = int(qCeil(annulusPtr->GetInnerRadius()));
		r2 = int(qFloor(annulusPtr->GetOuterRadius()));
		radius = (r2 - r1);
	}

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(realAoiPtr);
	if (circlePtr != NULL){
		r1 = 0;
		r2 = int(qFloor(circlePtr->GetRadius()));
		radius = r2;
	}

	if (!outputBitmapPtr->CreateBitmap(inputBitmap.GetPixelFormat(), istd::CIndex2d(angleRange, radius))){
		return false;
	}

	iipr::TImagePixelInterpolator<quint8> pixelInterpolator(inputBitmap, iipr::IImageInterpolationParams::IM_NO_INTERPOLATION);
	int pixelComponentsCount = inputBitmap.GetComponentsCount();

	for (int componentIndex = 0; componentIndex < pixelComponentsCount; componentIndex++){
		for (int r = 0; r < radius; r++){
			quint8* outputImageBeginPtr = (quint8*)outputBitmapPtr->GetLinePtr(r);
		
			for (int alpha = 0; alpha < angleRange; alpha++){
				double angle = alpha / double(angleRange) * I_2PI;
		
				double x = (r + r1) * qCos(angle); 			
				double y = (r  + r1) * qSin(angle);

				x += aoiCenter.GetX();
				y += aoiCenter.GetY();

				*(outputImageBeginPtr +  alpha * pixelComponentsCount + componentIndex) = pixelInterpolator.GetInterpolatedValue(x, y, componentIndex);
			}
		}
	}

	return true;
}


} // namespace iipr


