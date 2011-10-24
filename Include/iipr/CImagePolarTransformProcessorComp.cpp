#include "iipr/CImagePolarTransformProcessorComp.h"


 // ACF includes
#include "istd/TChangeNotifier.h"

#include "ibase/CSize.h"

#include "iimg/CBitmapRegion.h"


// IACF includes
#include "iipr/TImagePixelInterpolator.h"


namespace iipr
{


// reimplemented (iproc::IProcessor)

int CImagePolarTransformProcessorComp::DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		return TS_INVALID;
	}

	const i2d::IObject2d* aoiPtr = NULL;
	if (paramsPtr != NULL && m_aoiParamsIdAttrPtr.IsValid()){
		aoiPtr = dynamic_cast<const i2d::IObject2d*>(paramsPtr->GetParameter((*m_aoiParamsIdAttrPtr).ToString()));
	}

	return ConvertImage(*inputBitmapPtr, aoiPtr, *outputBitmapPtr) ? TS_OK : TS_INVALID;
}


// private methods

bool CImagePolarTransformProcessorComp::ConvertImage(
			const iimg::IBitmap& input,
			const i2d::IObject2d* aoiPtr,
			iimg::IBitmap& outputBitmap) const
{
	if (input.IsEmpty()){
		return true;
	}

	const i2d::IObject2d* realAoiPtr = aoiPtr;
	i2d::CRectangle imageRect(input.GetImageSize());
	if (aoiPtr == NULL){
		realAoiPtr = &imageRect;
	}

	iimg::CBitmapRegion bitmapRegion(&input);
	if (!bitmapRegion.CreateFromGeometry(*realAoiPtr)){
		return false;
	}

	if (bitmapRegion.IsBitmapRegionEmpty()){
		SendWarningMessage(0, "Cannot process an empty region");
		
		return false;
	}

	i2d::CRectangle regionRect = bitmapRegion.GetBoundingBox();
	i2d::CVector2d aoiCenter = regionRect.GetCenter();
	i2d::CVector2d diffVector = aoiCenter - regionRect.GetTopLeft();
	int radius = int(::ceil(diffVector.GetLength()));
	int angleRange = int(radius * I_PI + 0.5);

	int r1 = 0;
	int r2 = radius;

	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(realAoiPtr);
	if (annulusPtr != NULL){
		r1 = int(::ceil(annulusPtr->GetInnerRadius()));
		r2 = int(::floor(annulusPtr->GetOuterRadius()));
		radius = (r2 - r1);
	}

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(realAoiPtr);
	if (circlePtr != NULL){
		r1 = 0;
		r2 = int(::floor(circlePtr->GetRadius()));
		radius = r2;
	}

	if (!outputBitmap.CreateBitmap(input.GetPixelFormat(), istd::CIndex2d(angleRange, radius))){
		return false;
	}

	iipr::TImagePixelInterpolator<I_BYTE> pixelInterpolator(input, iipr::IImageInterpolationParams::IM_NO_INTERPOLATION);
	int pixelComponentsCount = input.GetComponentsCount();

	for (int componentIndex = 0; componentIndex < pixelComponentsCount; componentIndex++){
		for (int r = 0; r < radius; r++){
			I_BYTE* outputImageBeginPtr = (I_BYTE*)outputBitmap.GetLinePtr(r);
		
			for (int alpha = 0; alpha < angleRange; alpha++){
				double angle = alpha / double(angleRange) * I_2PI;
		
				double x = (r + r1) * cos(angle); 			
				double y = (r  + r1) * sin(angle);

				x += aoiCenter.GetX();
				y += aoiCenter.GetY();

				*(outputImageBeginPtr +  alpha * pixelComponentsCount + componentIndex) = pixelInterpolator.GetInterpolatedValue(x, y, componentIndex);
			}
		}
	}

	return true;
}


} // namespace iipr


