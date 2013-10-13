#include "iedge/CEdgeLine.h"


// ACF includes
#include "istd/TDelPtr.h"


namespace iedge
{


CEdgeLine::CEdgeLine()
:	BaseClass(),
	m_isClosed(false),
	m_areVolatileValid(true),
	m_totalLength(0),
	m_center(0, 0),
	m_minWeight(0),
	m_maxWeight(0)
{
}


CEdgeLine::CEdgeLine(const CEdgeLine& edgeLine)
{
	m_nodes = edgeLine.m_nodes;
	m_isClosed = edgeLine.m_isClosed;
	m_areVolatileValid = edgeLine.m_areVolatileValid;
	m_center = edgeLine.m_center;
	m_totalLength = edgeLine.m_totalLength;
	m_minWeight  = edgeLine.m_minWeight;
	m_maxWeight = edgeLine.m_maxWeight;
	m_boundingBox = edgeLine.m_boundingBox;

	const i2d::ICalibration2d* calibrationPtr = edgeLine.GetCalibration();
	if (calibrationPtr != NULL){
		istd::TDelPtr<i2d::ICalibration2d> newCalibration;
		newCalibration.SetCastedOrRemove(calibrationPtr->CloneMe());

		if (newCalibration.IsValid()){
			SetCalibration(newCalibration.PopPtr(), true);
		}
	}
}


void CEdgeLine::Clear()
{
	istd::CChangeNotifier notifier(this);

	m_nodes.clear();
	m_isClosed = false;
	m_center.Reset();
	m_totalLength = 0;
	m_minWeight = 0;
	m_maxWeight = 0;

	SetCalibration(NULL);
}


const CEdgeNode& CEdgeLine::GetNode(int index) const
{
	return m_nodes[index];
}


CEdgeNode& CEdgeLine::GetNodeRef(int index)
{
	return m_nodes[index];
}


void CEdgeLine::SetNode(int index, const CEdgeNode& node)
{
	istd::CChangeNotifier notifier(this);

	m_nodes[index] = node;
}


void CEdgeLine::SetClosed(bool state)
{
	if (state != m_isClosed){
		istd::CChangeNotifier notifier(this);

		m_isClosed = state;
	}
}


bool CEdgeLine::InsertNode(const CEdgeNode& node)
{
	istd::CChangeNotifier notifier(this);

	m_nodes.push_back(node);

	return true;
}


void CEdgeLine::CopyFromPolyline(const i2d::CPolyline& polyline, double weight, const i2d::CAffine2d* transformPtr)
{
	istd::CChangeNotifier notifier(this);

	int size = polyline.GetNodesCount();

	if (size <= 1){
		m_nodes.clear();
		m_isClosed = false;

		return;
	}

	m_isClosed = polyline.IsClosed();

	i2d::CVector2d position;

	if (transformPtr != NULL){
		i2d::CVector2d prevPosition = transformPtr->GetApply(m_isClosed?
					polyline.GetNode(size - 1):
					polyline.GetNode(0));

		m_nodes.push_back(CEdgeNode(prevPosition, weight));

		for (int i = 1; i < size; ++i){
			const i2d::CVector2d& position = transformPtr->GetApply(polyline.GetNode(i));
			if (position != prevPosition){
				m_nodes.push_back(CEdgeNode(position, weight));
			}

			prevPosition = position;
		}
	}
	else{
		i2d::CVector2d prevPosition = m_isClosed?
					polyline.GetNode(size - 1):
					polyline.GetNode(0);

		m_nodes.push_back(CEdgeNode(prevPosition, weight));

		for (int i = 1; i < size; ++i){
			const i2d::CVector2d& position = polyline.GetNode(i);
			if (position != prevPosition){
				m_nodes.push_back(CEdgeNode(position, weight));
			}

			prevPosition = position;
		}
	}
}


void CEdgeLine::CopyToPolyline(i2d::CPolyline& polyline, const i2d::CAffine2d* transformPtr) const
{
	polyline.Clear();

	if (transformPtr != NULL){
		for (		Nodes::ConstIterator iter = m_nodes.constBegin();
					iter != m_nodes.constEnd();
					++iter){
			const CEdgeNode& node = *iter;

			polyline.InsertNode(transformPtr->GetApply(node.GetPosition()));
		}
	}
	else{
		for (		Nodes::ConstIterator iter = m_nodes.constBegin();
					iter != m_nodes.constEnd();
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
	EnsureVolatileValid();

	return m_center;
}


void CEdgeLine::MoveCenterTo(const i2d::CVector2d& position)
{
	EnsureVolatileValid();

	istd::CChangeNotifier notifier(this);

	i2d::CVector2d diffVector = position - m_center;

	for (		Nodes::Iterator iter = m_nodes.begin();
				iter != m_nodes.end();
				++iter){
		CEdgeNode& node = *iter;

		node.SetPosition(node.GetPosition() + diffVector);
	}
}


i2d::CRectangle CEdgeLine::GetBoundingBox() const
{
	EnsureVolatileValid();

	return m_boundingBox;
}


bool CEdgeLine::Transform(
			const i2d::ITransformation2d& transformation,
			i2d::ITransformation2d::ExactnessMode /*mode*/,
			double* errorFactorPtr)
{
	istd::CChangeNotifier notifier(this);

	int transFlag = transformation.GetTransformationFlags();
	if ((transFlag & i2d::ITransformation2d::TF_AFFINE) != 0){
		i2d::CAffine2d localTransform;
		transformation.GetLocalTransform(i2d::CVector2d(0, 0), localTransform);

		for (		Nodes::Iterator iter = m_nodes.begin();
					iter != m_nodes.end();
					++iter){
			CEdgeNode& node = *iter;

			node.SetPosition(localTransform.GetApply(node.GetPosition()));
		}
	}
	else{
		for (		Nodes::Iterator iter = m_nodes.begin();
					iter != m_nodes.end();
					++iter){
			CEdgeNode& node = *iter;

			const i2d::CVector2d& position = node.GetPosition();

			i2d::CAffine2d localTransform;
			transformation.GetLocalTransform(position, localTransform);

			i2d::CVector2d resultPosition;
			transformation.GetPositionAt(position, resultPosition);

			node.SetPosition(resultPosition);
		}
	}

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	return true;
}


bool CEdgeLine::InvTransform(
			const i2d::ITransformation2d& transformation,
			i2d::ITransformation2d::ExactnessMode /*mode*/,
			double* errorFactorPtr)
{
	istd::CChangeNotifier notifier(this);

	int transFlag = transformation.GetTransformationFlags();
	if ((transFlag & i2d::ITransformation2d::TF_AFFINE) != 0){
		i2d::CAffine2d localInvTransform;
		transformation.GetLocalInvTransform(i2d::CVector2d(0, 0), localInvTransform);

		for (		Nodes::Iterator iter = m_nodes.begin();
					iter != m_nodes.end();
					++iter){
			CEdgeNode& node = *iter;

			node.SetPosition(localInvTransform.GetApply(node.GetPosition()));
		}
	}
	else{
		for (		Nodes::Iterator iter = m_nodes.begin();
					iter != m_nodes.end();
					++iter){
			CEdgeNode& node = *iter;

			const i2d::CVector2d& position = node.GetPosition();

			i2d::CAffine2d localTransform;
			transformation.GetLocalTransform(position, localTransform);

			i2d::CVector2d resultPosition;
			transformation.GetInvPositionAt(position, resultPosition);
			node.SetPosition(resultPosition);
		}
	}

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

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

			for (		Nodes::ConstIterator iter = m_nodes.constBegin();
						iter != m_nodes.constEnd();
						++iter){
				const CEdgeNode& node = *iter;

				CEdgeNode resultNode(
							localTransform.GetApply(node.GetPosition()),
							node.GetWeight());

				resultEdgeLinePtr->InsertNode(resultNode);
			}
		}
		else{
			for (		Nodes::ConstIterator iter = m_nodes.constBegin();
						iter != m_nodes.constEnd();
						++iter){
				const CEdgeNode& node = *iter;

				const i2d::CVector2d& position = node.GetPosition();

				i2d::CAffine2d localTransform;
				transformation.GetLocalTransform(position, localTransform);

				i2d::CVector2d resultPosition;
				transformation.GetPositionAt(position, resultPosition);

				CEdgeNode resultNode(
							resultPosition,
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

			for (		Nodes::ConstIterator iter = m_nodes.constBegin();
						iter != m_nodes.constEnd();
						++iter){
				const CEdgeNode& node = *iter;

				CEdgeNode resultNode(
							localInvTransform.GetApply(node.GetPosition()),
							node.GetWeight());

				resultEdgeLinePtr->InsertNode(resultNode);
			}
		}
		else{
			for (		Nodes::ConstIterator iter = m_nodes.constBegin();
						iter != m_nodes.constEnd();
						++iter){
				const CEdgeNode& node = *iter;

				const i2d::CVector2d& position = node.GetPosition();

				i2d::CAffine2d localTransform;
				transformation.GetLocalTransform(position, localTransform);

				i2d::CVector2d resultPosition;
				transformation.GetInvPositionAt(position, resultPosition);

				CEdgeNode resultNode(
							resultPosition,
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
		int nodesCount = m_nodes.size();

		retVal = retVal && archive.BeginMultiTag(nodesTag, nodeTag, nodesCount);
		for (		Nodes::Iterator iter = m_nodes.begin();
					iter != m_nodes.end();
					++iter){
			CEdgeNode& node = *iter;

			retVal = retVal && archive.BeginTag(nodeTag);
			retVal = retVal && node.Serialize(archive);
			retVal = retVal && archive.EndTag(nodeTag);
		}

		retVal = retVal && archive.EndTag(nodesTag);

		retVal = retVal && archive.BeginTag(isClosedTag);
		retVal = retVal && archive.Process(m_isClosed);
		retVal = retVal && archive.EndTag(isClosedTag);
	}
	else{
		int nodesCount = 0;

		retVal = retVal && archive.BeginMultiTag(nodesTag, nodeTag, nodesCount);
		if (!retVal){
			return false;
		}

		istd::CChangeNotifier notifier(this);

#if QT_VERSION >= 0x040700
		m_nodes.reserve(nodesCount);
#endif
		for (int i = 0; i < nodesCount; ++i){
			m_nodes.push_back(CEdgeNode());
			CEdgeNode& node = m_nodes.back();

			retVal = retVal && archive.BeginTag(nodeTag);
			retVal = retVal && node.Serialize(archive);
			retVal = retVal && archive.EndTag(nodeTag);
		}

		retVal = retVal && archive.EndTag(nodesTag);

		retVal = retVal && archive.BeginTag(isClosedTag);
		retVal = retVal && archive.Process(m_isClosed);
		retVal = retVal && archive.EndTag(isClosedTag);
	}

	return retVal;
}


// protected methods

// protected members

void CEdgeLine::CalcVolatile() const
{
	m_totalLength = 0;

	int nodesCount = m_nodes.size();
	if (nodesCount > 0){
		const CEdgeNode& firstNode = m_nodes.first();
		double prevWeight = firstNode.GetWeight();
		i2d::CVector2d prevPosition = firstNode.GetPosition();

		m_minWeight = prevWeight;
		m_maxWeight = prevWeight;
		int segmentsCount = CEdgeLine::GetSegmentsCount();
		if (segmentsCount > 0){
			m_center.Reset();

			double centerWeightSum = 0;

			// check all segments
			for (		int segmentIndex = 0;
						segmentIndex < segmentsCount;
						++segmentIndex){
				const CEdgeNode& node = m_nodes.at((segmentIndex + 1) % nodesCount);

				const i2d::CVector2d& nextPosition = node.GetPosition();
				double nextWeight = node.GetWeight();

				i2d::CVector2d deltaLine = nextPosition - prevPosition;
				double deltaWeight = nextWeight - prevWeight;

				double segmentLength = deltaLine.GetLength();

				m_center += (prevPosition * prevWeight + (deltaLine * prevWeight + prevPosition * deltaWeight) / 2.0 + deltaLine * deltaWeight / 3.0) * segmentLength;
				centerWeightSum += (prevWeight + nextWeight) * segmentLength * 0.5;
				m_totalLength += segmentLength;

				if (nextWeight < m_minWeight){
					m_minWeight = nextWeight;
				}
				else if (nextWeight > m_maxWeight){
					m_maxWeight = nextWeight;
				}

				prevPosition = nextPosition;
				prevWeight = nextWeight;
			}

			if (centerWeightSum > I_BIG_EPSILON){
				m_center /= centerWeightSum;
			}
			else{
				m_center = prevPosition;
			}
		}
		else{
			m_center = prevPosition;
		}
	}
	else{
		m_center.Reset();
		m_minWeight = 0;
		m_maxWeight = 0;
	}

	// TODO: implement bounding box of edge line better
	i2d::CPolyline tempPolyline;
	CopyToPolyline(tempPolyline);

	m_boundingBox = tempPolyline.GetBoundingBox();
}


// reimplemented (istd::IChangeable)

void CEdgeLine::OnEndChanges(int /*changeFlags*/, istd::IPolymorphic* /*changeParamsPtr*/)
{
	m_areVolatileValid = false;
}


} // namespace iedge


