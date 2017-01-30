#include "iipr/CImageCropDecalibrateProcessorComp.h"


// ACF includes
#include "istd/TArray.h"
#include "iprm/IParamsSet.h"
#include "iprm/TParamsPtr.h"
#include "iimg/IBitmap.h"


namespace iipr
{


bool CImageCropDecalibrateProcessorComp::CropImage(
			const iimg::IBitmap& inputBitmap,
			const i2d::CRectangle& sourceAoi,
			iimg::IBitmap& outputBitmap)
{
	istd::CIndex2d imageSize = inputBitmap.GetImageSize();
	if (!outputBitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, imageSize)){
		return false;
	}

	const i2d::ICalibration2d* calibrationPtr = sourceAoi.GetCalibration();

	const int cellSize = 8;

	istd::CIndex2d gridSize(imageSize.GetX() / cellSize + 1, imageSize.GetY() / cellSize + 1);
	istd::TArray<i2d::CVector2d, 2> cornerArray;
	// MESF ACF-version compatibility
	cornerArray.SetSizes(gridSize);
	cornerArray.SetAllElements(i2d::CVector2d(qInf(), qInf()));

	istd::CIndex2d gridIndex;
	for (gridIndex[1] = 0; gridIndex[1] < gridSize[1]; ++gridIndex[1]){
		for (gridIndex[0] = 0; gridIndex[0] < gridSize[0]; ++gridIndex[0]){
			i2d::CVector2d cornerDestPos(
						sourceAoi.GetLeft() + sourceAoi.GetWidth() * gridIndex.GetX() * cellSize / imageSize.GetX(),
						sourceAoi.GetTop() + sourceAoi.GetHeight() * gridIndex.GetY() * cellSize / imageSize.GetY());

			i2d::CVector2d originalPos;

			if (calibrationPtr != NULL){
				if (calibrationPtr->GetPositionAt(cornerDestPos, originalPos)){
					cornerArray.SetAt(gridIndex, originalPos);
				}
			}
			else{
				cornerArray.SetAt(gridIndex, cornerDestPos);
			}
		}
	}

	const quint8* sourceBufferPtr = (const quint8*)inputBitmap.GetLinePtr(0);
	int sourceLineDiff = inputBitmap.GetLinesDifference();

	quint8* destBufferPtr = (quint8*)outputBitmap.GetLinePtr(0);
	int destLineDiff = outputBitmap.GetLinesDifference();

	i2d::CRectangle inputPosRectangle(I_BIG_EPSILON, I_BIG_EPSILON, imageSize.GetX() - 2 * I_BIG_EPSILON, imageSize.GetY() - 2 * I_BIG_EPSILON);

	for (gridIndex[1] = 0; gridIndex[1] < gridSize[1] - 1; ++gridIndex[1]){
		for (gridIndex[0] = 0; gridIndex[0] < gridSize[0] - 1; ++gridIndex[0]){
			const i2d::CVector2d& posLeftTop = cornerArray.GetAt(gridIndex);
			const i2d::CVector2d& posRightTop = cornerArray.GetAt(istd::CIndex2d(gridIndex.GetX() + 1, gridIndex.GetY()));
			const i2d::CVector2d& posLeftBottom = cornerArray.GetAt(istd::CIndex2d(gridIndex.GetX(), gridIndex.GetY() + 1));
			const i2d::CVector2d& posRightBottom = cornerArray.GetAt(istd::CIndex2d(gridIndex.GetX() + 1, gridIndex.GetY() + 1));

			if ((posRightTop.GetX() == qInf()) || (posRightBottom.GetX() == qInf())){
				++gridIndex[0];
				continue;
			}

			if ((posLeftTop.GetX() == qInf()) || (posLeftBottom.GetX() == qInf())){
				continue;
			}

			if (		inputPosRectangle.Contains(posLeftTop) &&
						inputPosRectangle.Contains(posRightTop) &&
						inputPosRectangle.Contains(posLeftBottom) &&
						inputPosRectangle.Contains(posRightBottom)){
				for (int y = 0; y < cellSize; ++y){
					quint8* destLinePtr = destBufferPtr + gridIndex[0] * cellSize + (gridIndex[1] * cellSize + y) * destLineDiff;

					i2d::CVector2d normPosLeft = (posLeftTop * (cellSize - y) + posLeftBottom * y) / (cellSize * cellSize);
					i2d::CVector2d normPosRight = (posRightTop * (cellSize - y) + posRightBottom * y) / (cellSize * cellSize);

					for (int x = 0; x < cellSize; ++x){
						i2d::CVector2d sourcePos = normPosLeft * (cellSize - x) + normPosRight * x;

						int sourceX = int(sourcePos.GetX());
						int sourceY = int(sourcePos.GetY());

						destLinePtr[x] = sourceBufferPtr[sourceX + sourceY * sourceLineDiff];
					}
				}
			}
			else{
				for (int y = 0; y < cellSize; ++y){
					quint8* destLinePtr = destBufferPtr + gridIndex[0] * cellSize + (gridIndex[1] * cellSize + y) * destLineDiff;

					i2d::CVector2d normPosLeft = (posLeftTop * (cellSize - y) + posLeftBottom * y) / (cellSize * cellSize);
					i2d::CVector2d normPosRight = (posRightTop * (cellSize - y) + posRightBottom * y) / (cellSize * cellSize);

					for (int x = 0; x < cellSize; ++x){
						i2d::CVector2d sourcePos = normPosLeft * (cellSize - x) + normPosRight * x;

						if (inputPosRectangle.Contains(sourcePos)){
							int sourceX = int(sourcePos.GetX());
							int sourceY = int(sourcePos.GetY());

							destLinePtr[x] = sourceBufferPtr[sourceX + sourceY * sourceLineDiff];
						}
						else{
							destLinePtr[x] = 0;
						}
					}
				}
			}
		}
	}

	return true;
}


// reimplemented (iproc::IProcessor)

int CImageCropDecalibrateProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		return TS_INVALID;
	}

	iprm::TParamsPtr<i2d::CRectangle> aoiParamPtr(paramsPtr, m_aoiParamIdAttrPtr, m_defaultAoiCompPtr, true);
	if (!aoiParamPtr.IsValid()){
		SendErrorMessage(MI_BAD_PARAMS, QObject::tr("No AOI rectangle found"));
		return TS_INVALID;
	}

	return CropImage(*inputBitmapPtr, *aoiParamPtr, *outputBitmapPtr) ? TS_OK : TS_INVALID;
}


} //!namespace iipr


