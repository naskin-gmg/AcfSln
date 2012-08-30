#include "iedge/CEdgesReductorComp.h"


// ACF includes
#include "istd/istd.h"
#include "iprm/TParamsPtr.h"

// ACF-Solutions includes
#include "imeas/CGeneralUnitInfo.h"


namespace iedge
{


void CEdgesReductorComp::GetReducedLine(
			const CEdgeLine& edgeLine,
			double positionTolerance,
			double weightTolerance,
			CEdgeLine& result) const
{
	bool isClosed = edgeLine.IsClosed();

	int nodesCount = edgeLine.GetNodesCount();
	if (isClosed){
		if (nodesCount <= 3){
			result = edgeLine;

			return;	// not enough nodes to reduce
		}
	}
	else{
		if (nodesCount <= 2){
			result = edgeLine;

			return;	// not enough nodes to reduce
		}
	}

	NodesToRemove nodesToRemove(nodesCount, false);

	int firstInsideIndex = 0;
	int lastInsideIndex = 0;
	int reducedCount = ReduceNodes(
				edgeLine,
				positionTolerance,
				weightTolerance,
				0,
				isClosed? nodesCount: nodesCount - 1,
				nodesToRemove,
				firstInsideIndex,
				lastInsideIndex);

	if (nodesCount - reducedCount >= (isClosed? 3: 2)){
		// copy the nodes could not be reduced
		result.Clear();

		for (		int nodeIndex = 0;
					nodeIndex < nodesCount;
					++nodeIndex){
			if (!nodesToRemove[nodeIndex]){
				const CEdgeNode& node = edgeLine.GetNode(nodeIndex);

				result.InsertNode(node);
			}
		}

		result.SetClosed(isClosed);
	}
	else{
		// no reduction done, simple copy can be used
		result = edgeLine;
	}
}


void CEdgesReductorComp::GetReducedLines(
			const CEdgeLine::Container& edgeLines,
			double positionTolerance,
			double weightTolerance,
			CEdgeLine::Container& result) const
{
	result.Reset();

	int linesCount = edgeLines.GetItemsCount();
	for (int i = 0; i < linesCount; ++i){
		const CEdgeLine& edgeLine = edgeLines.GetAt(i);

		CEdgeLine& resultLine = result.PushBack(CEdgeLine());

		GetReducedLine(edgeLine, positionTolerance, weightTolerance, resultLine);
	}
}


// reimplemented (iedge::IEdgeLinesProcessor)

bool CEdgesReductorComp::DoLinesProcessing(
			const iprm::IParamsSet* paramsPtr,
			const CEdgeLine::Container& edgeLines,
			CEdgeLine::Container& result) const
{
	imath::CVarVector toleranceValues;
	if ((paramsPtr != NULL) && m_toleranceParamsIdAttrPtr.IsValid()){
		iprm::TParamsPtr<imeas::INumericValue> toleranceParamsPtr(paramsPtr, *m_toleranceParamsIdAttrPtr);
		if (toleranceParamsPtr.IsValid()){
			toleranceValues = toleranceParamsPtr->GetValues();
		}
	}
	else if (m_defaultToleranceParamsCompPtr.IsValid()){
		toleranceValues = m_defaultToleranceParamsCompPtr->GetValues();
	}

	double positionTolerance = 0.1;
	double weightTolerance = 0.1;

	if (toleranceValues.GetElementsCount() >= 1){
		positionTolerance = toleranceValues[0];
	}

	if (toleranceValues.GetElementsCount() >= 2){
		weightTolerance = toleranceValues[1];
	}

	GetReducedLines(edgeLines, positionTolerance, weightTolerance, result);

	return true;
}


// reimplemented (iproc::IProcessor)

int CEdgesReductorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const CEdgeLine::Container* edgeLinesPtr = dynamic_cast<const CEdgeLine::Container*>(inputPtr);
	CEdgeLine::Container* containerPtr = dynamic_cast<CEdgeLine::Container*>(outputPtr);

	if (		(edgeLinesPtr == NULL) ||
				(containerPtr == NULL)){
		return TS_INVALID;
	}

	return DoLinesProcessing(paramsPtr, *edgeLinesPtr, *containerPtr)? TS_OK: TS_INVALID;
}


// reimplemented (imeas::INumericConstraints)

int CEdgesReductorComp::GetNumericValuesCount() const
{
	return 2;
}


QString CEdgesReductorComp::GetNumericValueName(int index) const
{
	switch (index){
	case 1:
		return QObject::tr("Weight tolerance");

	default:
		return QObject::tr("Position tolerance");
	}
}


QString CEdgesReductorComp::GetNumericValueDescription(int index) const
{
	switch (index){
	case 1:
		return QObject::tr("Tolerance of weight for edge node reduction");

	default:
		return QObject::tr("Tolerance of position for edge node reduction");
	}
}


const imeas::IUnitInfo& CEdgesReductorComp::GetNumericValueUnitInfo(int index) const
{
	static imeas::CGeneralUnitInfo positionUnitInfo(imeas::IUnitInfo::UT_RELATIVE, "px", 1, istd::CRange(0, 10));
	static imeas::CGeneralUnitInfo weightUnitInfo(imeas::IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(0.00, 1));

	switch (index){
	case 1:
		return weightUnitInfo;

	default:
		if (m_distanceUnitInfoCompPtr.IsValid()){
			return *m_distanceUnitInfoCompPtr;
		}
		else{
			return positionUnitInfo;
		}
	}
}


// protected methods

int CEdgesReductorComp::ReduceNodes(
			const CEdgeLine& edgeLine,
			double positionTolerance,
			double weightTolerance,
			int firstIndex,
			int lastIndex,
			NodesToRemove& nodesToRemove,
			int& firstInsideIndex,
			int& lastInsideIndex) const
{
	Q_ASSERT(firstIndex >= 0);
	Q_ASSERT(lastIndex > firstIndex);

	int removedPoints = 0;

	int nodesInsideCount = lastIndex - firstIndex - 1;

	if (nodesInsideCount >= 1){
		int nodesCount = edgeLine.GetNodesCount();

		int nodeIndex = firstIndex + nodesInsideCount / 2 + 1;
		int centerNodeIndex = nodeIndex % nodesCount;
		int leftNodeIndex = (nodeIndex - 1) % nodesCount;
		int rightNodeIndex = (nodeIndex + 1) % nodesCount;

		if (nodeIndex > firstIndex + 1){
			removedPoints += ReduceNodes(
						edgeLine,
						positionTolerance,
						weightTolerance,
						firstIndex,
						nodeIndex,
						nodesToRemove,
						firstInsideIndex,
						leftNodeIndex);
		}
		else{
			firstInsideIndex = firstIndex + 1;
		}

		if (nodeIndex < lastIndex - 1){
			removedPoints += ReduceNodes(
						edgeLine,
						positionTolerance,
						weightTolerance,
						nodeIndex,
						lastIndex,
						nodesToRemove,
						rightNodeIndex,
						lastInsideIndex);
		}
		else{
			lastInsideIndex = lastIndex - 1;
		}

		const CEdgeNode& node = edgeLine.GetNode(centerNodeIndex);
		const CEdgeNode& prevNode = edgeLine.GetNode(leftNodeIndex);
		const CEdgeNode& nextNode = edgeLine.GetNode(rightNodeIndex);

		i2d::CVector2d nextPrevDelta = nextNode.GetPosition() - prevNode.GetPosition();
		i2d::CVector2d prevDelta = node.GetPosition() - prevNode.GetPosition();

		double removedDistance = 0;
		double removedWeightProp = 0;

		double nextPrevDistance = nextPrevDelta.GetLength();
		if (nextPrevDistance >= I_BIG_EPSILON){
			removedDistance = qFabs(prevDelta.GetCrossProductZ(nextPrevDelta) / nextPrevDistance);

			double segmentAlpha = prevDelta.GetDotProduct(nextPrevDelta) / nextPrevDelta.GetLength2();
			double interpolatedWeight = (1 - segmentAlpha) * prevNode.GetWeight() + segmentAlpha * nextNode.GetWeight();

			removedWeightProp = node.GetWeight() / interpolatedWeight;
		}
		else{
			removedDistance = prevDelta.GetLength();

			double averageWeight = (prevNode.GetWeight() + nextNode.GetWeight()) * 0.5;

			removedWeightProp = node.GetWeight() / averageWeight;
		}

		if ((removedDistance < positionTolerance) && (qFabs(removedWeightProp - 1) < weightTolerance)){
			nodesToRemove[centerNodeIndex] = true;

			if (firstInsideIndex == nodeIndex){
				firstInsideIndex = rightNodeIndex;
			}

			if (lastInsideIndex == nodeIndex){
				lastInsideIndex = leftNodeIndex;
			}

			++removedPoints;
		}
	}
	else{
		firstInsideIndex = firstIndex + 1;
		lastInsideIndex = lastIndex - 1;
	}

	return removedPoints;
}


} // namespace iedge



