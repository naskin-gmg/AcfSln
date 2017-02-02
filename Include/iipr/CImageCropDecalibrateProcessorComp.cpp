#include "iipr/CImageCropDecalibrateProcessorComp.h"


// ACF includes
#include "istd/TArray.h"
#include "iprm/IParamsSet.h"
#include "iprm/TParamsPtr.h"
#include "iimg/IBitmap.h"


namespace iipr
{


bool CImageCropDecalibrateProcessorComp::CropImage(
			const i2d::CRectangle& sourceAoi,
			int cellSize,
			const iimg::IBitmap& inputBitmap,
			iimg::IBitmap& outputBitmap)
{
	if (cellSize < 2){
		return false;
	}

	istd::CIndex2d outputImageSize;
	if (!CalcOutputImageSize(sourceAoi, outputImageSize)){
		return false;
	}

	istd::CIndex2d gridSize((outputImageSize.GetX() + cellSize - 1) / cellSize + 1, (outputImageSize.GetY() + cellSize - 1) / cellSize + 1);
	if (!outputBitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, outputImageSize)){
		return false;
	}

	istd::TArray<i2d::CVector2d, 2> cornerArray;
	// MESF ACF-version compatibility
	cornerArray.SetSizes(gridSize);
	cornerArray.SetAllElements(i2d::CVector2d(qInf(), qInf()));

	const i2d::ICalibration2d* calibrationPtr = sourceAoi.GetCalibration();

	istd::CIndex2d gridIndex;
	for (gridIndex[1] = 0; gridIndex[1] < gridSize[1]; ++gridIndex[1]){
		for (gridIndex[0] = 0; gridIndex[0] < gridSize[0]; ++gridIndex[0]){
			i2d::CVector2d cornerDestPos(
						sourceAoi.GetLeft() + sourceAoi.GetWidth() * gridIndex.GetX() * cellSize / outputImageSize.GetX(),
						sourceAoi.GetTop() + sourceAoi.GetHeight() * gridIndex.GetY() * cellSize / outputImageSize.GetY());

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

	istd::CIndex2d inputImageSize = inputBitmap.GetImageSize();
	i2d::CRectangle inputPosRectangle(I_BIG_EPSILON, I_BIG_EPSILON, inputImageSize.GetX() - 2 * I_BIG_EPSILON, inputImageSize.GetY() - 2 * I_BIG_EPSILON);

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
				int maxY = qMin(cellSize, outputImageSize.GetY() - gridIndex[1] * cellSize);
				for (int y = 0; y < maxY; ++y){
					quint8* destLinePtr = destBufferPtr + gridIndex[0] * cellSize + (gridIndex[1] * cellSize + y) * destLineDiff;

					i2d::CVector2d normPosLeft = (posLeftTop * (cellSize - y) + posLeftBottom * y) / (cellSize * cellSize);
					i2d::CVector2d normPosRight = (posRightTop * (cellSize - y) + posRightBottom * y) / (cellSize * cellSize);

					int maxX = qMin(cellSize, outputImageSize.GetX() - gridIndex[0] * cellSize);
					for (int x = 0; x < maxX; ++x){
						i2d::CVector2d sourcePos = normPosLeft * (cellSize - x) + normPosRight * x;

						int sourceX = int(sourcePos.GetX());
						int sourceY = int(sourcePos.GetY());

						destLinePtr[x] = sourceBufferPtr[sourceX + sourceY * sourceLineDiff];
					}
				}
			}
			else{
				int maxY = qMin(cellSize, outputImageSize.GetY() - gridIndex[1] * cellSize);
				for (int y = 0; y < maxY; ++y){
					quint8* destLinePtr = destBufferPtr + gridIndex[0] * cellSize + (gridIndex[1] * cellSize + y) * destLineDiff;

					i2d::CVector2d normPosLeft = (posLeftTop * (cellSize - y) + posLeftBottom * y) / (cellSize * cellSize);
					i2d::CVector2d normPosRight = (posRightTop * (cellSize - y) + posRightBottom * y) / (cellSize * cellSize);

					int maxX = qMin(cellSize, outputImageSize.GetX() - gridIndex[0] * cellSize);
					for (int x = 0; x < maxX; ++x){
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


bool CImageCropDecalibrateProcessorComp::CalcCalibration(
			const i2d::CRectangle& sourceAoi,
			icalib::CAffineCalibration2d& outputCalib)
{
	istd::CIndex2d outputImageSize;
	if (!CalcOutputImageSize(sourceAoi, outputImageSize)){
		return false;
	}

	double scaleX = outputImageSize.GetX() / sourceAoi.GetWidth();
	double scaleY = outputImageSize.GetY() / sourceAoi.GetHeight();
	i2d::CMatrix2d scaleMatrix(scaleX, 0, 0, scaleY);

	i2d::CAffine2d transformation;
	transformation.SetTranslation(-scaleMatrix.GetMultiplied(sourceAoi.GetLeftTop()));
	transformation.SetDeformMatrix(scaleMatrix);

	outputCalib.SetTransformation(transformation);

	return true;
}


// reimplemented (iproc::IProcessor)

int CImageCropDecalibrateProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	iprm::TParamsPtr<i2d::CRectangle> aoiParamPtr(paramsPtr, m_aoiParamIdAttrPtr, m_defaultAoiCompPtr, true);
	if (!aoiParamPtr.IsValid()){
		SendErrorMessage(MI_BAD_PARAMS, QObject::tr("No AOI rectangle found"));
		return TS_INVALID;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		icalib::CAffineCalibration2d* outputCalibPtr = dynamic_cast<icalib::CAffineCalibration2d*>(outputPtr);
		if (outputCalibPtr != NULL){
			return CalcCalibration(*aoiParamPtr, *outputCalibPtr)? TS_OK: TS_INVALID;
		}

		return TS_INVALID;
	}

	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	return CropImage(*aoiParamPtr, *m_cellSizeAttrPtr, *inputBitmapPtr, *outputBitmapPtr)? TS_OK: TS_INVALID;
}


// protected methods

bool CImageCropDecalibrateProcessorComp::CalcOutputImageSize(
			const i2d::CRectangle& sourceAoi,
			istd::CIndex2d& result)
{
	const i2d::ICalibration2d* calibrationPtr = sourceAoi.GetCalibration();

	double pixelSize = 1;

	i2d::CVector2d inputLeftTopPos = sourceAoi.GetLeftTop();
	i2d::CVector2d inputRightTopPos = sourceAoi.GetRightTop();
	i2d::CVector2d inputLeftBottomPos = sourceAoi.GetLeftBottom();
	i2d::CVector2d inputRightBottomPos = sourceAoi.GetRightBottom();
	if (calibrationPtr != NULL){
		double leftDist = 0;
		double rightDist = 0;
		double topDist = 0;
		double bottomDist = 0;
		if (
					calibrationPtr->GetDistance(sourceAoi.GetLeftTop(), sourceAoi.GetLeftBottom(), leftDist) &&
					calibrationPtr->GetDistance(sourceAoi.GetRightTop(), sourceAoi.GetRightBottom(), rightDist) &&
					calibrationPtr->GetDistance(sourceAoi.GetLeftTop(), sourceAoi.GetRightTop(), topDist) &&
					calibrationPtr->GetDistance(sourceAoi.GetLeftBottom(), sourceAoi.GetRightBottom(), bottomDist)){
			pixelSize = 0.5 * (leftDist + rightDist + topDist + bottomDist) / (sourceAoi.GetWidth() + sourceAoi.GetHeight());
		}
	}

	result = istd::CIndex2d(qCeil(sourceAoi.GetWidth() * pixelSize),  qCeil(sourceAoi.GetHeight() * pixelSize));

	return true;
}


} //!namespace iipr


