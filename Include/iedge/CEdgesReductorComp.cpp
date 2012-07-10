#include "iedge/CEdgesReductorComp.h"

// ACF includes
#include "istd/istd.h"

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
	int nodesCount = edgeLine.GetNodesCount();
	if (nodesCount < 2){
		return;	// not enough nodes to reduce
	}

	bool isClosed = edgeLine.IsClosed();

	const CEdgeNode* prevNodePtr = NULL;
	const CEdgeNode* nodePtr = NULL;
	int nodeIndex = 0;
	int nextNodeIndex = 0;
	if (isClosed){
		prevNodePtr = &edgeLine.GetNode(nodesCount - 2);
		nodePtr = &edgeLine.GetNode(nodesCount - 1);
		nodeIndex = nodesCount - 1;
	}
	else{
		prevNodePtr = &edgeLine.GetNode(0);
		nodePtr = &edgeLine.GetNode(1);
		nodeIndex = 1;
		nextNodeIndex = 2;
	}

	NodeInfos nodeInfos(nodesCount - nextNodeIndex);

	for (		int nodeInfoIndex = 0;	// initialization of other index
				nextNodeIndex < nodesCount;
				++nodeInfoIndex, ++nodeIndex, ++nextNodeIndex){
		NodeInfo& nodeInfo = nodeInfos[nodeInfoIndex];

		nodeInfo.index = nodeIndex;

		const CEdgeNode* nextNodePtr = &edgeLine.GetNode(nextNodeIndex);

		i2d::CVector2d nextPrevDelta = nextNodePtr->GetPosition() - prevNodePtr->GetPosition();
		i2d::CVector2d prevDelta = nodePtr->GetPosition() - prevNodePtr->GetPosition();

		double nextPrevDistance = nextPrevDelta.GetLength();
		if (nextPrevDistance >= I_BIG_EPSILON){
			nodeInfo.removedDistance = qFabs(prevDelta.GetCrossProductZ(nextPrevDelta) / nextPrevDistance);

			double segmentAlpha = prevDelta.GetDotProduct(nextPrevDelta) / nextPrevDelta.GetLength2();
			double interpolatedWeight = (1 - segmentAlpha) * prevNodePtr->GetWeight() + segmentAlpha * nextNodePtr->GetWeight();

			nodeInfo.removedWeightDiff = qFabs(nodePtr->GetWeight() - interpolatedWeight);
		}
		else{
			nodeInfo.removedDistance = prevDelta.GetLength();

			double averageWeight = (prevNodePtr->GetWeight() + nextNodePtr->GetWeight()) * 0.5;

			nodeInfo.removedWeightDiff = qFabs(nodePtr->GetWeight() - averageWeight);
		}

		prevNodePtr = nodePtr;
		nodePtr = nextNodePtr;
	}

	if (ReduceNodes(edgeLine, positionTolerance, weightTolerance, nodeInfos.begin(), nodeInfos.end(), nodeInfos)){
		// copy the nodes could not be reduced
		result.Clear();

		if (!isClosed){	// first node is always copied for not closed edges
			result.InsertNode(edgeLine.GetNode(0));
		}

		for (		NodeInfos::ConstIterator nodeIter = nodeInfos.constBegin();
					nodeIter != nodeInfos.constEnd();
					++nodeIter){
			int nodeIndex = nodeIter->index;

			const CEdgeNode& node = edgeLine.GetNode(nodeIndex);

			result.InsertNode(node);
		}

		if (!isClosed){	// last node is always copied for not closed edges
			result.InsertNode(edgeLine.GetNode(nodesCount - 1));
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
		const imeas::INumericParams* toleranceParamsPtr = dynamic_cast<const imeas::INumericParams*>(paramsPtr->GetParameter(*m_toleranceParamsIdAttrPtr));
		if (toleranceParamsPtr != NULL){
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
	static imeas::CGeneralUnitInfo weightUnitInfo(imeas::IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(0.00, 0.1));

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

bool CEdgesReductorComp::ReduceNodes(
			const CEdgeLine& edgeLine,
			double positionTolerance,
			double weightTolerance,
			NodeInfos::Iterator firstIter,
			NodeInfos::Iterator endIter,
			NodeInfos& nodeInfos) const
{
	if (edgeLine.IsClosed() && (nodeInfos.size() <= 3)){
		return false;
	}

	bool retVal = false;

	NodeInfos::Iterator nodeIter = firstIter + (endIter - firstIter) / 2;

	if ((nodeIter->removedDistance < positionTolerance) && (nodeIter->removedWeightDiff < weightTolerance)){
		nodeIter = nodeInfos.erase(nodeIter);

		retVal = true;
	}

	if (nodeIter != firstIter){
		retVal = ReduceNodes(edgeLine, positionTolerance, weightTolerance, firstIter, nodeIter, nodeInfos) || retVal;
	}

	if (nodeIter != endIter){
		retVal = ReduceNodes(edgeLine, positionTolerance, weightTolerance, nodeIter + 1, endIter, nodeInfos) || retVal;
	}

	return retVal;
}


} // namespace iedge



