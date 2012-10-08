#include "iedge/CEdgeLine.h"


namespace iedge
{


CEdgeLine::CEdgeLine()
:	m_isClosed(false),
	m_totalLength(0),
	m_minWeight(0),
	m_maxWeight(0)
{
}


CEdgeLine::CEdgeLine(const CEdgeLine& edge)
:	m_edgeLines(edge.m_edgeLines),
	m_isClosed(false),
	m_areVolatileValid(true),
	m_totalLength(0),
	m_minWeight(0),
	m_maxWeight(0)
{
}


void CEdgeLine::Clear()
{
	m_edgeLines.clear();
	m_isClosed = false;
	m_totalLength = 0;
	m_minWeight = 0;
	m_maxWeight = 0;
}


const CEdgeNode& CEdgeLine::GetNode(int index) const
{
	return m_edgeLines[index];
}


CEdgeNode& CEdgeLine::GetNodeRef(int index)
{
	return m_edgeLines[index];
}


void CEdgeLine::SetNode(int index, const CEdgeNode& node)
{
	m_edgeLines[index] = node;

	m_areVolatileValid = false;
}


bool CEdgeLine::InsertNode(const CEdgeNode& node)
{
	m_edgeLines.push_back(node);

	m_areVolatileValid = false;

	return true;
}


void CEdgeLine::CopyFromPolyline(const i2d::CPolyline& polyline, double weight, const i2d::CAffine2d* transformPtr)
{
	int size = polyline.GetNodesCount();

	if (size <= 1){
		m_edgeLines.clear();
		m_isClosed = false;
		m_areVolatileValid = false;

		return;
	}

	m_isClosed = polyline.IsClosed();

	i2d::CVector2d prevDelta;
	i2d::CVector2d position;

	if (transformPtr != NULL){
		if (m_isClosed){
			i2d::CVector2d prevPosition = transformPtr->GetApply(polyline.GetNode(size - 1));
			position = transformPtr->GetApply(polyline.GetNode(0));
			prevDelta = position - prevPosition;
		}
		else{
			position = transformPtr->GetApply(polyline.GetNode(0));
			prevDelta = i2d::CVector2d(0, 0);
		}

		for (int i = 0; i < size; ++i){
			const i2d::CVector2d& nextPosition = transformPtr->GetApply(polyline.GetNode(m_isClosed? (i + 1) % size: qMin(i + 1, size - 1)));
			i2d::CVector2d prevDelta = nextPosition - position;
			i2d::CVector2d orthogonal = prevDelta.GetOrthogonal() + prevDelta.GetOrthogonal();
			if ((orthogonal.GetX() != 0) || (orthogonal.GetY() != 0)){
				double orthogonalLength = orthogonal.GetLength();
				i2d::CVector2d derivative = orthogonal * (weight / orthogonalLength);

				CEdgeNode node(position, derivative, weight);

				m_edgeLines.push_back(node);
			}
			position = nextPosition;
			prevDelta = prevDelta;
		}
	}
	else{
		if (m_isClosed){
			i2d::CVector2d prevPosition = polyline.GetNode(size - 1);
			position = polyline.GetNode(0);
			prevDelta = position - prevPosition;
		}
		else{
			position = polyline.GetNode(0);
			prevDelta = i2d::CVector2d(0, 0);
		}

		for (int i = 0; i < size; ++i){
			const i2d::CVector2d& nextPosition = polyline.GetNode(m_isClosed? (i + 1) % size: qMin(i + 1, size - 1));
			i2d::CVector2d prevDelta = nextPosition - position;
			i2d::CVector2d orthogonal = prevDelta.GetOrthogonal() + prevDelta.GetOrthogonal();
			if ((orthogonal.GetX() != 0) || (orthogonal.GetY() != 0)){
				double orthogonalLength = orthogonal.GetLength();
				i2d::CVector2d derivative = orthogonal * (weight / orthogonalLength);

				CEdgeNode node(position, derivative, weight);

				m_edgeLines.push_back(node);
			}
			position = nextPosition;
			prevDelta = prevDelta;
		}
	}

	m_areVolatileValid = false;
}


void CEdgeLine::CopyToPolyline(i2d::CPolyline& polyline, const i2d::CAffine2d* transformPtr) const
{
	polyline.Clear();

	if (transformPtr != NULL){
		for (		Nodes::ConstIterator iter = m_edgeLines.constBegin();
					iter != m_edgeLines.constEnd();
					++iter){
			const CEdgeNode& node = *iter;

			polyline.InsertNode(transformPtr->GetApply(node.GetPosition()));
		}
	}
	else{
		for (		Nodes::ConstIterator iter = m_edgeLines.constBegin();
					iter != m_edgeLines.constEnd();
					++iter){
			const CEdgeNode& node = *iter;

			polyline.InsertNode(node.GetPosition());
		}
	}

	polyline.SetClosed(IsClosed());
}


// reimplemented (i2d::IObject2d)

i2d::CVector2d CEdgeLine::GetCenter() const
{
	if (!m_areVolatileValid){
		CalcVolatile();
	}

	return m_center;
}


void CEdgeLine::MoveCenterTo(const i2d::CVector2d& position)
{
	if (!m_areVolatileValid){
		CalcVolatile();
	}

	i2d::CVector2d diffVector = position - m_center;

	for (		Nodes::Iterator iter = m_edgeLines.begin();
				iter != m_edgeLines.end();
				++iter){
		CEdgeNode& node = *iter;

		node.SetPosition(node.GetPosition() + diffVector);
	}
}


bool CEdgeLine::Transform(
			const i2d::ITransformation2d& transformation,
			i2d::ITransformation2d::ExactnessMode /*mode*/,
			double* errorFactorPtr)
{
	int transFlag = transformation.GetTransformationFlags();
	if ((transFlag & i2d::ITransformation2d::TF_AFFINE) != 0){
		i2d::CAffine2d localTransform;
		transformation.GetLocalTransform(i2d::CVector2d(0, 0), localTransform);

		for (		Nodes::Iterator iter = m_edgeLines.begin();
					iter != m_edgeLines.end();
					++iter){
			CEdgeNode& node = *iter;

			node.SetPosition(localTransform.GetApply(node.GetPosition()));
			node.SetDerivative(localTransform.GetApplyToDelta(node.GetDerivative()));
		}
	}
	else{
		for (		Nodes::Iterator iter = m_edgeLines.begin();
					iter != m_edgeLines.end();
					++iter){
			CEdgeNode& node = *iter;

			const i2d::CVector2d& position = node.GetPosition();

			i2d::CAffine2d localTransform;
			transformation.GetLocalTransform(position, localTransform);

			node.SetDerivative(localTransform.GetApplyToDelta(node.GetDerivative()));

			i2d::CVector2d resultPosition;
			transformation.GetPositionAt(position, resultPosition);

			node.SetPosition(resultPosition);
		}
	}

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	m_areVolatileValid = false;

	return true;
}


bool CEdgeLine::InvTransform(
			const i2d::ITransformation2d& transformation,
			i2d::ITransformation2d::ExactnessMode /*mode*/,
			double* errorFactorPtr)
{
	int transFlag = transformation.GetTransformationFlags();
	if ((transFlag & i2d::ITransformation2d::TF_AFFINE) != 0){
		i2d::CAffine2d localInvTransform;
		transformation.GetLocalInvTransform(i2d::CVector2d(0, 0), localInvTransform);

		for (		Nodes::Iterator iter = m_edgeLines.begin();
					iter != m_edgeLines.end();
					++iter){
			CEdgeNode& node = *iter;

			node.SetPosition(localInvTransform.GetApply(node.GetPosition()));
			node.SetDerivative(localInvTransform.GetApplyToDelta(node.GetDerivative()));
		}
	}
	else{
		for (		Nodes::Iterator iter = m_edgeLines.begin();
					iter != m_edgeLines.end();
					++iter){
			CEdgeNode& node = *iter;

			const i2d::CVector2d& position = node.GetPosition();

			i2d::CAffine2d localTransform;
			transformation.GetLocalTransform(position, localTransform);

			node.SetDerivative(localTransform.GetDeformMatrix().GetInvMultiplied(node.GetDerivative()));

			i2d::CVector2d resultPosition;
			transformation.GetInvPositionAt(position, resultPosition);
			node.SetPosition(resultPosition);
		}
	}

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	m_areVolatileValid = false;

	return true;
}


bool CEdgeLine::GetTransformed(
			const i2d::ITransformation2d& transformation,
			IObject2d& result,
			i2d::ITransformation2d::ExactnessMode /*mode*/,
			double* errorFactorPtr) const
{
	CEdgeLine* resultEdgeLinePtr = dynamic_cast<CEdgeLine*>(&result);
	if (resultEdgeLinePtr != NULL){
		resultEdgeLinePtr->Clear();

		int transFlag = transformation.GetTransformationFlags();
		if ((transFlag & i2d::ITransformation2d::TF_AFFINE) != 0){
			i2d::CAffine2d localTransform;
			transformation.GetLocalTransform(i2d::CVector2d(0, 0), localTransform);

			for (		Nodes::ConstIterator iter = m_edgeLines.constBegin();
						iter != m_edgeLines.constEnd();
						++iter){
				const CEdgeNode& node = *iter;

				CEdgeNode resultNode(
							localTransform.GetApply(node.GetPosition()),
							localTransform.GetApplyToDelta(node.GetDerivative()),
							node.GetWeight());

				resultEdgeLinePtr->InsertNode(resultNode);
			}
		}
		else{
			for (		Nodes::ConstIterator iter = m_edgeLines.constBegin();
						iter != m_edgeLines.constEnd();
						++iter){
				const CEdgeNode& node = *iter;

				const i2d::CVector2d& position = node.GetPosition();

				i2d::CAffine2d localTransform;
				transformation.GetLocalTransform(position, localTransform);

				i2d::CVector2d resultPosition;
				transformation.GetPositionAt(position, resultPosition);

				CEdgeNode resultNode(
							resultPosition,
							localTransform.GetApplyToDelta(node.GetDerivative()),
							node.GetWeight());

				resultEdgeLinePtr->InsertNode(resultNode);
			}
		}

		if (errorFactorPtr != NULL){
			*errorFactorPtr = 0;
		}

		return true;
	}

	i2d::CPolyline* resultPolylinePtr = dynamic_cast<i2d::CPolyline*>(&result);
	if (resultPolylinePtr != NULL){
		int transFlag = transformation.GetTransformationFlags();
		if ((transFlag & i2d::ITransformation2d::TF_AFFINE) != 0){
			i2d::CAffine2d localTransform;
			transformation.GetLocalTransform(i2d::CVector2d(0, 0), localTransform);

			CopyToPolyline(*resultPolylinePtr, &localTransform);

			if (errorFactorPtr != NULL){
				*errorFactorPtr = 0;
			}

			return true;
		}
	}

	return false;
}


bool CEdgeLine::GetInvTransformed(
			const i2d::ITransformation2d& transformation,
			IObject2d& result,
			i2d::ITransformation2d::ExactnessMode /*mode*/,
			double* errorFactorPtr) const
{
	CEdgeLine* resultEdgeLinePtr = dynamic_cast<CEdgeLine*>(&result);
	if (resultEdgeLinePtr != NULL){
		resultEdgeLinePtr->Clear();

		int transFlag = transformation.GetTransformationFlags();
		if ((transFlag & i2d::ITransformation2d::TF_AFFINE) != 0){
			i2d::CAffine2d localInvTransform;
			transformation.GetLocalInvTransform(i2d::CVector2d(0, 0), localInvTransform);

			for (		Nodes::ConstIterator iter = m_edgeLines.constBegin();
						iter != m_edgeLines.constEnd();
						++iter){
				const CEdgeNode& node = *iter;

				CEdgeNode resultNode(
							localInvTransform.GetApply(node.GetPosition()),
							localInvTransform.GetApplyToDelta(node.GetDerivative()),
							node.GetWeight());

				resultEdgeLinePtr->InsertNode(resultNode);
			}
		}
		else{
			for (		Nodes::ConstIterator iter = m_edgeLines.constBegin();
						iter != m_edgeLines.constEnd();
						++iter){
				const CEdgeNode& node = *iter;

				const i2d::CVector2d& position = node.GetPosition();

				i2d::CAffine2d localTransform;
				transformation.GetLocalTransform(position, localTransform);

				i2d::CVector2d resultPosition;
				transformation.GetInvPositionAt(position, resultPosition);

				CEdgeNode resultNode(
							resultPosition,
							localTransform.GetDeformMatrix().GetInvMultiplied(node.GetDerivative()),
							node.GetWeight());

				resultEdgeLinePtr->InsertNode(resultNode);
			}
		}

		if (errorFactorPtr != NULL){
			*errorFactorPtr = 0;
		}

		return true;
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CEdgeLine::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag nodesTag("Nodes", "List of edge nodes");
	static iser::CArchiveTag nodeTag("Node", "Single edge node");
	static iser::CArchiveTag isClosedTag("IsClosed", "Flag indicating if edge is closed");

	bool retVal = true;

	if (archive.IsStoring()){
		int nodesCount = m_edgeLines.size();

		retVal = retVal && archive.BeginMultiTag(nodesTag, nodeTag, nodesCount);
		for (		Nodes::Iterator iter = m_edgeLines.begin();
					iter != m_edgeLines.end();
					++iter){
			CEdgeNode& node = *iter;

			retVal = retVal && archive.BeginTag(nodeTag);
			retVal = retVal && node.Serialize(archive);
			retVal = retVal && archive.EndTag(nodeTag);
		}

		retVal = retVal && archive.EndTag(nodesTag);
	}
	else{
		int nodesCount = 0;

		retVal = retVal && archive.BeginMultiTag(nodesTag, nodeTag, nodesCount);
		if (!retVal){
			return false;
		}

		m_edgeLines.reserve(nodesCount);

		for (int i = 0; i < nodesCount; ++i){
			m_edgeLines.push_back(CEdgeNode());
			CEdgeNode& node = m_edgeLines.back();

			retVal = retVal && archive.BeginTag(nodeTag);
			retVal = retVal && node.Serialize(archive);
			retVal = retVal && archive.EndTag(nodeTag);
		}

		retVal = retVal && archive.EndTag(nodesTag);

		m_areVolatileValid = false;
	}

	retVal = retVal && archive.BeginTag(isClosedTag);
	retVal = retVal && archive.Process(m_isClosed);
	retVal = retVal && archive.EndTag(isClosedTag);

	return retVal;
}


// protected methods

// protected members

void CEdgeLine::CalcVolatile() const
{
	m_totalLength = 0;

	if (!m_edgeLines.isEmpty()){
		const CEdgeNode& firstNode = m_edgeLines.first();
		double firstWeight = firstNode.GetWeight();
		i2d::CVector2d lastPosition = firstNode.GetPosition();
		m_center = lastPosition * firstWeight;
		m_minWeight = firstWeight;
		m_maxWeight = firstWeight;

		double weightsSum = firstWeight;

		for (		Nodes::ConstIterator iter = m_edgeLines.constBegin() + 1;
					iter != m_edgeLines.constEnd();
					++iter){
			const CEdgeNode& node = *iter;

			const i2d::CVector2d& position = node.GetPosition();

			m_totalLength += lastPosition.GetDistance(position);
			double weight = node.GetWeight();

			m_center = position * weight;

			if (weight < m_minWeight){
				m_minWeight = weight;
			}
			else if (weight > m_maxWeight){
				m_maxWeight = weight;
			}

			lastPosition = position;
		}

		if (weightsSum > I_BIG_EPSILON){
			m_center /= weightsSum;
		}
		else{
			m_center.Reset();
		}
	}
	else{
		m_center.Reset();
		m_minWeight = 0;
		m_maxWeight = 0;
	}

	m_areVolatileValid = true;
}


// protected methods of embedded class Container

// reimplemented (ibase::TSerializableContainer)

bool CEdgeLine::Container::SerializeItem(CEdgeLine& item, iser::IArchive& archive)
{
	return item.Serialize(archive);
}


} // namespace iedge


