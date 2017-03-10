#include "iipr/CImageCropDecalibrateProcessorComp.h"


// ACF includes
#include "istd/TArray.h"
#include "iprm/IParamsSet.h"
#include "iprm/TParamsPtr.h"
#include "iimg/IBitmap.h"


namespace iipr
{


CImageCropDecalibrateProcessorComp::CImageCropDecalibrateProcessorComp()
{
	m_orientationModes.InsertOption(QObject::tr("Simple"), "Simple");
	m_orientationModes.InsertOption(QObject::tr("Visual"), "Visual");
	m_orientationModes.InsertOption(QObject::tr("No Reflexion"), "NoReflexion");

	m_interpolationModes.InsertOption(QObject::tr("Simple"), "Simple");
	m_interpolationModes.InsertOption(QObject::tr("Linear"), "Linear");
}


bool CImageCropDecalibrateProcessorComp::CropImage(
			const i2d::CRectangle& sourceAoi,
			int cellSize,
			const iimg::IBitmap& inputBitmap,
			iimg::IBitmap& outputBitmap,
			int interpolationMode,
			int orientationMode)
{
	// TODO: implement interpolation mode using interpolationMode

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

	bool flipHorizontal = false;
	bool flipVertical = false;
	if ((orientationMode == OM_NO_REFLEXION) || (orientationMode == OM_VISUAL)){
		if (calibrationPtr != NULL){
			i2d::CAffine2d approxTransform;
			if (calibrationPtr->GetLocalTransform(i2d::CVector2d::GetZero(), approxTransform)){
				if (orientationMode == OM_VISUAL){
					if (approxTransform.GetDeformMatrix().GetAxisX().GetX() < 0){
						flipHorizontal = true;
						flipVertical = true;
					}
				}

				if (approxTransform.GetDeformMatrix().GetDet() < 0){
					flipVertical = !flipVertical;
				}
			}
		}
	}

	istd::CIndex2d gridIndex;
	for (gridIndex[1] = 0; gridIndex[1] < gridSize[1]; ++gridIndex[1]){
		double propY = double(gridIndex.GetY() * cellSize) / outputImageSize.GetY();
		if (flipVertical){
			propY = 1 - propY;
		}
		double cornerDestY = sourceAoi.GetVerticalRange().GetValueFromAlpha(propY);

		for (gridIndex[0] = 0; gridIndex[0] < gridSize[0]; ++gridIndex[0]){
			double propX = double(gridIndex.GetX() * cellSize) / outputImageSize.GetX();
			if (flipHorizontal){
				propX = 1 - propX;
			}
			double cornerDestX = sourceAoi.GetHorizontalRange().GetValueFromAlpha(propX);

			if (calibrationPtr != NULL){
				i2d::CVector2d originalPos;
				if (calibrationPtr->GetPositionAt(i2d::CVector2d(cornerDestX, cornerDestY), originalPos)){
					cornerArray.SetAt(gridIndex, originalPos);
				}
			}
			else{
				cornerArray.SetAt(gridIndex, i2d::CVector2d(cornerDestX, cornerDestY));
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
			icalib::CAffineCalibration2d& outputCalib,
			int orientationMode)
{
	istd::CIndex2d outputImageSize;
	if (!CalcOutputImageSize(sourceAoi, outputImageSize)){
		return false;
	}

	double scaleX = outputImageSize.GetX() / sourceAoi.GetWidth();
	double scaleY = outputImageSize.GetY() / sourceAoi.GetHeight();

	bool flipHorizontal = false;
	bool flipVertical = false;
	if ((orientationMode == OM_NO_REFLEXION) || (orientationMode == OM_VISUAL)){
		const i2d::ICalibration2d* calibrationPtr = sourceAoi.GetCalibration();
		if (calibrationPtr != NULL){
			i2d::CAffine2d approxTransform;
			if (calibrationPtr->GetLocalTransform(i2d::CVector2d::GetZero(), approxTransform)){
				if (orientationMode == OM_VISUAL){
					if (approxTransform.GetDeformMatrix().GetAxisX().GetX() < 0){
						flipHorizontal = true;
						flipVertical = true;
					}
				}

				if (approxTransform.GetDeformMatrix().GetDet() < 0){
					flipVertical = !flipVertical;
				}
			}
		}
	}

	i2d::CMatrix2d scaleMatrix(flipHorizontal? -scaleX: scaleX, 0, 0, flipVertical? -scaleY: scaleY);

	i2d::CAffine2d transformation;
	if (flipHorizontal){
		if (flipVertical){
			transformation.SetTranslation(-scaleMatrix.GetMultiplied(sourceAoi.GetRightBottom()));
		}
		else{
			transformation.SetTranslation(-scaleMatrix.GetMultiplied(sourceAoi.GetRightTop()));
		}
	}
	else{
		if (flipVertical){
			transformation.SetTranslation(-scaleMatrix.GetMultiplied(sourceAoi.GetLeftBottom()));
		}
		else{
			transformation.SetTranslation(-scaleMatrix.GetMultiplied(sourceAoi.GetLeftTop()));
		}
	}
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

	int orientationMode = OM_SIMPLE;
	iprm::TParamsPtr<iprm::ISelectionParam> orientationModePtr(paramsPtr, m_orientationModeParamIdAttrPtr, m_defaultOrientationModeCompPtr, false);
	if (orientationModePtr.IsValid()){
		orientationMode = orientationModePtr->GetSelectedOptionIndex();
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		icalib::CAffineCalibration2d* outputCalibPtr = dynamic_cast<icalib::CAffineCalibration2d*>(outputPtr);
		if (outputCalibPtr != NULL){
			return CalcCalibration(*aoiParamPtr, *outputCalibPtr, orientationMode)? TS_OK: TS_INVALID;
		}

		return TS_INVALID;
	}

	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	int interpolationMode = IM_LINEAR;
	iprm::TParamsPtr<iprm::ISelectionParam> interpolationModePtr(paramsPtr, m_interpolationModeParamIdAttrPtr, m_defaultInterpolationModeCompPtr, false);
	if (interpolationModePtr.IsValid()){
		interpolationMode = interpolationModePtr->GetSelectedOptionIndex();
	}

	return CropImage(*aoiParamPtr, *m_cellSizeAttrPtr, *inputBitmapPtr, *outputBitmapPtr, interpolationMode, orientationMode)? TS_OK: TS_INVALID;
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
					calibrationPtr->GetDistance(inputLeftTopPos, inputLeftBottomPos, leftDist) &&
					calibrationPtr->GetDistance(inputRightTopPos, inputRightBottomPos, rightDist) &&
					calibrationPtr->GetDistance(inputLeftTopPos, inputRightTopPos, topDist) &&
					calibrationPtr->GetDistance(inputLeftBottomPos, inputRightBottomPos, bottomDist)){
			pixelSize = 0.5 * (leftDist + rightDist + topDist + bottomDist) / (sourceAoi.GetWidth() + sourceAoi.GetHeight());
		}
	}

	result = istd::CIndex2d(qCeil(sourceAoi.GetWidth() * pixelSize),  qCeil(sourceAoi.GetHeight() * pixelSize));

	return true;
}


} //!namespace iipr


