#include "iedge/CFastEdgesExtractorComp.h"


// STL includes
#include <cmath>

// QT includes
#include <QtCore/QVector>
#include <QtCore/qmath.h>

// ACF includes
#include "i2d/CPolypoint.h"
#include "i2d/CVector2d.h"
#include "iprm/TParamsPtr.h"

// ACF-Solutions includes
#include "imeas/CGeneralUnitInfo.h"

// QSF includes
#include "iedge/CEdgeNode.h"
#include "iedge/CEdgeLine.h"
#include "iedge/CEdgeLine.h"


namespace iedge
{


//	reimplemented (iedge::IEdgesExtractor)

bool CFastEdgesExtractorComp::DoContourExtraction(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& bitmap,
			CEdgeLine::Container& result) const
{
	result.Reset();

	ibase::CSize size = bitmap.GetImageSize();
	if ((size.GetX() < 3) || (size.GetY() < 3)){
		SendErrorMessage(0, "Image too small to calculate edges");

		return false;
	}

	imath::CVarVector thresholdValues;
	if ((paramsPtr != NULL) && m_thresholdParamIdAttrPtr.IsValid()){
		iprm::TParamsPtr<imeas::INumericValue> thresholdParamPtr(paramsPtr, *m_thresholdParamIdAttrPtr);
		if (thresholdParamPtr.IsValid()){
			thresholdValues = thresholdParamPtr->GetValues();
		}
	}
	else if (m_defaultThresholdParamCompPtr.IsValid()){
		thresholdValues = m_defaultThresholdParamCompPtr->GetValues();
	}

	double threshold = (thresholdValues.GetElementsCount() > 0)? thresholdValues.GetElement(0): 0.1;

	iprm::TParamsPtr<i2d::IObject2d> aoiObjectPtr;
	if ((paramsPtr != NULL) && m_aoiParamIdAttrPtr.IsValid()){
		aoiObjectPtr.Init(paramsPtr, *m_aoiParamIdAttrPtr);
	}
	else if (m_defaultAoiCompPtr.IsValid()){
		aoiObjectPtr.SetPtr(m_defaultAoiCompPtr.GetPtr());
	}

	const iimg::CScanlineMask* maskPtr = NULL;

	iimg::CScanlineMask mask;
	if (aoiObjectPtr.IsValid()){
		i2d::CRect clipArea(size);
		if (mask.CreateFromGeometry(*aoiObjectPtr.GetPtr(), &clipArea)){
			maskPtr = &mask;
		}
	}

	const quint32 threshold2Factor = quint32(threshold * threshold * THRESHOLD_FACTOR * THRESHOLD_FACTOR);

	int width = size.GetX();

	const quint8* sourceLinePtr = (const quint8*)bitmap.GetLinePtr(0);
	int sourceLineDiff = bitmap.GetLinesDifference();

	QVector<PixelDescriptor> destLine1(width - 1);
	QVector<PixelDescriptor> destLine2(width - 1);
	QVector<PixelDescriptor> destLine3(width - 1);

	PixelDescriptor* destLine1Ptr = &destLine1[0];
	PixelDescriptor* destLine2Ptr = &destLine2[0];
	PixelDescriptor* destLine3Ptr = &destLine3[0];

	memset(destLine1Ptr, 0, (width - 1) * sizeof(PixelDescriptor));
	memset(destLine2Ptr, 0, (width - 1) * sizeof(PixelDescriptor));
	memset(destLine3Ptr, 0, (width - 1) * sizeof(PixelDescriptor));

	InternalContainer container(*m_maxNodesCountAttrPtr);

	if (maskPtr != NULL){
		return false;
	}
	else{
		CalcFullDerivativeLine(sourceLinePtr, sourceLinePtr + sourceLineDiff, destLine1Ptr, width);
		sourceLinePtr += sourceLineDiff;
		CalcFullDerivativeLine(sourceLinePtr, sourceLinePtr + sourceLineDiff, destLine2Ptr, width);
		sourceLinePtr += sourceLineDiff;

		for (int y = 1; y < size.GetY() - 1; ++y){
			CalcLine(sourceLinePtr, sourceLinePtr + sourceLineDiff, destLine1Ptr, destLine2Ptr, destLine3Ptr, y, width, threshold2Factor, container);

			// move line address in rolling line buffer
			PixelDescriptor* storedDestLine1Ptr = destLine1Ptr;
			destLine1Ptr = destLine2Ptr;
			destLine2Ptr = destLine3Ptr;
			destLine3Ptr = storedDestLine1Ptr;

			sourceLinePtr += sourceLineDiff;
		}
	}

	container.ExtractContours(result);

	if (container.IsContainerFull()){
		SendErrorMessage(0, "Container of nodes is full");
	}

	return true;
}


// reimplemented (iproc::IProcessor)

int CFastEdgesExtractorComp::DoProcessing(
		const iprm::IParamsSet* paramsPtr,
		const istd::IPolymorphic* inputPtr,
		istd::IChangeable* outputPtr,
		iproc::IProgressManager* /*progressManagerPtr*/)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	CEdgeLine::Container* containerPtr = dynamic_cast<CEdgeLine::Container*>(outputPtr);

	if (		(bitmapPtr == NULL) ||
				(containerPtr == NULL)){
		return TS_INVALID;
	}

	return DoContourExtraction(paramsPtr, *bitmapPtr, *containerPtr)? TS_OK: TS_INVALID;
}


// reimplemented (imeas::INumericConstraints)

int CFastEdgesExtractorComp::GetNumericValuesCount() const
{
	return 1;
}


QString CFastEdgesExtractorComp::GetNumericValueName(int /*index*/) const
{
	return QObject::tr("Threshold", "Contour extraction threshold");
}


QString CFastEdgesExtractorComp::GetNumericValueDescription(int /*index*/) const
{
	return QObject::tr("Contour extraction threshold");
}


const imeas::IUnitInfo& CFastEdgesExtractorComp::GetNumericValueUnitInfo(int /*index*/) const
{
	static imeas::CGeneralUnitInfo thresholdUnitInfo(imeas::IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(0.01, 0.1));

	return thresholdUnitInfo;
}


// private static methods

__forceinline void CFastEdgesExtractorComp::TryConnectElements(
			PixelDescriptor& neightborPixel,
			PixelDescriptor& pixel)
{
	ExtNode* neightborNodePtr = neightborPixel.listReference;
	if (neightborNodePtr == NULL){
		return;
	}

	ExtNode* nodePtr = pixel.listReference;
	Q_ASSERT(nodePtr != NULL);

	const i2d::CVector2d& elementPosition = neightborNodePtr->position;
	i2d::CVector2d displacement = elementPosition - nodePtr->position;

	double derivativeDotProduct = nodePtr->derivative.GetDotProduct(neightborNodePtr->derivative);

	if (derivativeDotProduct > 0){
		double derivativeAngleCos = derivativeDotProduct / (nodePtr->derivative.GetLength() * neightborNodePtr->derivative.GetLength());

		double orientationFactor = (nodePtr->derivative + neightborNodePtr->derivative).GetCrossProductZ(displacement) / displacement.GetLength2();
		if (orientationFactor >= 0){
			double connectionWeight = orientationFactor * derivativeAngleCos;
			if (			(connectionWeight > nodePtr->nextWeight) &&
							((neightborNodePtr->prevPtr == NULL) || (neightborNodePtr->prevWeight < connectionWeight))){
				// disconnect neighbor from previous connection
				if (neightborNodePtr->prevPtr != NULL){
					Q_ASSERT(neightborNodePtr->prevPtr->nextPtr == neightborNodePtr);

					neightborNodePtr->prevPtr->nextPtr = NULL;
				}

				if (nodePtr->nextPtr != NULL){
					Q_ASSERT(nodePtr->nextPtr->prevPtr == nodePtr);

					nodePtr->nextPtr->prevPtr = NULL;
				}

				// connect both nodes
				nodePtr->nextPtr = neightborNodePtr;
				nodePtr->nextWeight = connectionWeight;
				neightborNodePtr->prevPtr = nodePtr;
				neightborNodePtr->prevWeight = connectionWeight;
			}
		}
		else{
			double connectionWeight = -orientationFactor * derivativeAngleCos;
			if (			(connectionWeight > nodePtr->prevWeight) &&
							((neightborNodePtr->nextPtr == NULL) || (neightborNodePtr->nextWeight < connectionWeight))){
				// disconnect neighbor from previous connection
				if (neightborNodePtr->nextPtr != NULL){
					Q_ASSERT(neightborNodePtr->nextPtr->prevPtr == neightborNodePtr);

					neightborNodePtr->nextPtr->prevPtr = NULL;
				}

				if (nodePtr->prevPtr != NULL){
					Q_ASSERT(nodePtr->prevPtr->nextPtr == nodePtr);

					nodePtr->prevPtr->nextPtr = NULL;
				}

				// connect both nodes
				nodePtr->prevPtr = neightborNodePtr;
				nodePtr->prevWeight = connectionWeight;
				neightborNodePtr->nextPtr = nodePtr;
				neightborNodePtr->nextWeight = connectionWeight;
			}
		}
	}
}


__forceinline CFastEdgesExtractorComp::ExtNode* CFastEdgesExtractorComp::AddPointToContour(
				double posX,
				double posY,
				double derivativeX,
				double derivativeY,
				double weight,
				PixelDescriptor* destLine1,
				PixelDescriptor* destLine2,
				int x,
				int /*y*/,
				InternalContainer& container){
	ExtNode* nodePtr = container.AddElementToList();
	if (nodePtr != NULL){
		PixelDescriptor& pixelDescriptor = destLine2[x];

		nodePtr->position = i2d::CVector2d(posX, posY);
		nodePtr->derivative = i2d::CVector2d(derivativeX, derivativeY);
		nodePtr->weight = weight;
		nodePtr->prevPtr = NULL;
		nodePtr->nextPtr = NULL;
		nodePtr->prevWeight = 0;
		nodePtr->nextWeight = 0;

		pixelDescriptor.listReference = nodePtr;

		// try connects with all already calculated neighbours
		TryConnectElements(destLine1[x - 1], pixelDescriptor);
		TryConnectElements(destLine1[x], pixelDescriptor);
		TryConnectElements(destLine1[x + 1], pixelDescriptor);
		TryConnectElements(destLine2[x - 1], pixelDescriptor);
	}

	return nodePtr;
}


__forceinline void CFastEdgesExtractorComp::CalcFullDerivative(
			const quint8* sourceLine1,
			const quint8* sourceLine2,
			PixelDescriptor* destLine,
			int x){
	int pixel11 = sourceLine1[x];
	int pixel21 = sourceLine1[x + 1];
	int pixel12 = sourceLine2[x];
	int pixel22 = sourceLine2[x + 1];

	int dx = pixel11 + pixel12 - pixel21 - pixel22;
	int dy = pixel11 + pixel21 - pixel12 - pixel22;

	PixelDescriptor& pixelDescr = destLine[x];

	pixelDescr.brightness = qint16(pixel11 + pixel21 + pixel12 + pixel22);
	pixelDescr.dx = qint16(dx);
	pixelDescr.dy = qint16(dy);
	pixelDescr.dirLength2 = quint32(dx * dx + dy * dy);
	pixelDescr.listReference = NULL;
}


__forceinline void CFastEdgesExtractorComp::CalcFullDerivativeLine(
			const quint8* sourceLine1,
			const quint8* sourceLine2,
			PixelDescriptor* destLine,
			int sourceWidth){
	for (int x = 0; x < sourceWidth - 1; ++x){
		CalcFullDerivative(sourceLine1, sourceLine2, destLine, x);
	}
}


__forceinline void CFastEdgesExtractorComp::CalcPoint(
			const quint8* sourceLine1,
			const quint8* sourceLine2,
			PixelDescriptor* destLine1,
			PixelDescriptor* destLine2,
			PixelDescriptor* destLine3,
			int x,
			int y,
			quint32 threshold2Factor,
			InternalContainer& container)
{
	CalcFullDerivative(sourceLine1, sourceLine2, destLine3, x + 1);	// shift 1 line

	PixelDescriptor& pixelDescriptor = destLine2[x];

	ExtNode* nodePtr = NULL;

	quint32 dirLength2 = pixelDescriptor.dirLength2;

	if (dirLength2 > threshold2Factor){
		const PixelDescriptor& leftPixelDescriptor = destLine2[x - 1];
		const PixelDescriptor& rightPixelDescriptor = destLine2[x + 1];
		const PixelDescriptor& topPixelDescriptor = destLine1[x];
		const PixelDescriptor& bottomPixelDescriptor = destLine3[x];

		int horizEdgeStrength = 0;
		if ((dirLength2 > leftPixelDescriptor.dirLength2) && (dirLength2 >= rightPixelDescriptor.dirLength2)){
			horizEdgeStrength = dirLength2 * 2 - leftPixelDescriptor.dirLength2 - rightPixelDescriptor.dirLength2;
		}

		int verticalEdgeStrength = 0;
		if ((dirLength2 > topPixelDescriptor.dirLength2) && (dirLength2 >= bottomPixelDescriptor.dirLength2)){
			verticalEdgeStrength = dirLength2 * 2 - topPixelDescriptor.dirLength2 - bottomPixelDescriptor.dirLength2;
		}

		if (horizEdgeStrength > verticalEdgeStrength){
			double weight = std::sqrt(double(dirLength2));
			double derivative1 = weight - std::sqrt(double(leftPixelDescriptor.dirLength2));
			double derivative2 = weight - std::sqrt(double(rightPixelDescriptor.dirLength2));
			double shiftX = derivative1 / (derivative1 + derivative2);

			nodePtr = AddPointToContour(
						x + shiftX, y + 0.5,
						double(pixelDescriptor.dx) / THRESHOLD_FACTOR, double(pixelDescriptor.dy) / THRESHOLD_FACTOR,
						weight / THRESHOLD_FACTOR,
						destLine1, destLine2,
						x, y,
						container);
		}
		else if (verticalEdgeStrength > 0){
			double weight = std::sqrt(double(dirLength2));
			double derivative1 = weight - std::sqrt(double(topPixelDescriptor.dirLength2));
			double derivative2 = weight - std::sqrt(double(bottomPixelDescriptor.dirLength2));
			double shiftY = derivative1 / (derivative1 + derivative2);

			nodePtr = AddPointToContour(
						x + 0.5, y + shiftY,
						double(pixelDescriptor.dx) / THRESHOLD_FACTOR, double(pixelDescriptor.dy) / THRESHOLD_FACTOR,
						weight / THRESHOLD_FACTOR,
						destLine1, destLine2,
						x, y,
						container);
		}
	}
}


__forceinline void CFastEdgesExtractorComp::CalcLine(
			const quint8* sourceLine1,
			const quint8* sourceLine2,
			PixelDescriptor* destLine1,
			PixelDescriptor* destLine2,
			PixelDescriptor* destLine3,
			int y,
			int sourceWidth,
			quint32 threshold2Factor,
			InternalContainer& container){
	CalcFullDerivative(sourceLine1, sourceLine2, destLine3, 0);	// no shift
	CalcFullDerivative(sourceLine1, sourceLine2, destLine3, 1);	// no shift
	for (int x = 1; x < sourceWidth - 2; x++){
		CalcPoint(sourceLine1, sourceLine2, destLine1, destLine2, destLine3, x, y, threshold2Factor, container);	// no shift
	}
}


// inline methods of embedded class InternalContainer

__forceinline CFastEdgesExtractorComp::ExtNode* CFastEdgesExtractorComp::InternalContainer::AddElementToList()
{
	if (m_freeIndex < m_buffer.size()){
		return &m_buffer[m_freeIndex++];
	}
	else{
		m_isContainerFull = true;

		return NULL;
	}
}


// public methods of embedded class InternalContainer

CFastEdgesExtractorComp::InternalContainer::InternalContainer(int size)
:	m_buffer(size),
	m_freeIndex(0),
	m_isContainerFull(false)
{
}


bool CFastEdgesExtractorComp::InternalContainer::IsContainerFull() const
{
	return m_isContainerFull;
}


void CFastEdgesExtractorComp::InternalContainer::ExtractContours(CEdgeLine::Container& result)
{
	// mark all as not extracted
	for (int i = 0; i < m_freeIndex; ++i){
		ExtNode& node = m_buffer[i];
		node.isExtracted = false;
	}

	// looking for opened edges
	for (int i = 0; i < m_freeIndex; ++i){
		ExtNode& node = m_buffer[i];

		if (!node.isExtracted && (node.prevPtr == NULL)){	// check if it was not processed yet and is first node
			CEdgeLine& resultLine = result.PushBack(CEdgeLine());
			resultLine.SetClosed(false);

			for (		const ExtNode* nodeElementPtr = &node;
						nodeElementPtr != NULL;
						nodeElementPtr = nodeElementPtr->nextPtr){
				Q_ASSERT((nodeElementPtr->nextPtr == NULL) || (nodeElementPtr->nextPtr->prevPtr == nodeElementPtr));
				Q_ASSERT((nodeElementPtr->prevPtr == NULL) || (nodeElementPtr->prevPtr->nextPtr == nodeElementPtr));

				nodeElementPtr->isExtracted = true;

				CEdgeNode node(
							nodeElementPtr->position,
							nodeElementPtr->derivative,
							nodeElementPtr->weight);

				resultLine.InsertNode(node);
			}
		}
	}

	// looking for closed edges
	for (int i = 0; i < m_freeIndex; ++i){
		ExtNode& node = m_buffer[i];

		if (!node.isExtracted){	// check if it was not processed yet
			CEdgeLine& resultLine = result.PushBack(CEdgeLine());
			resultLine.SetClosed(true);

			const ExtNode* nodeElementPtr = &node;
			do{
				Q_ASSERT(nodeElementPtr->nextPtr != NULL);
				Q_ASSERT(nodeElementPtr->prevPtr != NULL);
				Q_ASSERT(nodeElementPtr->nextPtr->prevPtr == nodeElementPtr);
				Q_ASSERT(nodeElementPtr->prevPtr->nextPtr == nodeElementPtr);

				nodeElementPtr->isExtracted = true;

				CEdgeNode node(
							nodeElementPtr->position,
							nodeElementPtr->derivative,
							nodeElementPtr->weight);

				resultLine.InsertNode(node);

				nodeElementPtr = nodeElementPtr->nextPtr;
			} while (nodeElementPtr != &node);
		}
	}
}


} // namespace iedge


