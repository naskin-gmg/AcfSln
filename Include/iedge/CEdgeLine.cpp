// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iedge/CEdgeLine.h>


// ACF includes
#include <istd/TDelPtr.h>


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
:	BaseClass(edgeLine)
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
		istd::TUniqueInterfacePtr<i2d::ICalibration2d> newCalibration;
		newCalibration.MoveCastedPtr(calibrationPtr->CloneMe());

		if (newCalibration.IsValid()){
			SetCalibration(newCalibration.PopInterfacePtr(), true);
		}
	}
}


CEdgeLine::~CEdgeLine()
{
	Clear();
}


void CEdgeLine::Clear()
{
	BeginChanges(GetAnyChange());

	m_nodes.clear();
	m_isClosed = false;
	m_center.Reset();
	m_totalLength = 0;
	m_minWeight = 0;
	m_maxWeight = 0;

	SetCalibration(NULL);

	EndChanges(GetAnyChange());
}


void CEdgeLine::SetNodesCount(int count)
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	m_nodes.resize(count);
}


void CEdgeLine::SetNode(int index, const CEdgeNode& node)
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	m_nodes[index] = node;
}


void CEdgeLine::SetClosed(bool state)
{
	if (state != m_isClosed){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_isClosed = state;
	}
}


bool CEdgeLine::InsertNode(const CEdgeNode& node)
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	m_nodes.push_back(node);

	return true;
}


void CEdgeLine::CopyFromPolyline(const i2d::CPolyline& polyline, double weight, const i2d::CAffine2d* transformPtr)
{
	istd::CChangeNotifier notifier(this, &GetAllChanges());
	Q_UNUSED(notifier);

	int size = polyline.GetNodesCount();

	if (size <= 1){
		m_nodes.clear();
		m_isClosed = false;

		return;
	}

	m_isClosed = polyline.IsClosed();

	// copy, transform and remove the consequent duplicates
	if (transformPtr != NULL){
		i2d::CVector2d prevPosition = transformPtr->GetApply(polyline.GetNodePos(size - 1));

		for (int i = 0; i < size; ++i){
			const i2d::CVector2d& position = transformPtr->GetApply(polyline.GetNodePos(i));
			if ((position != prevPosition) || (!m_isClosed && (i == 0))){
				m_nodes.push_back(CEdgeNode(position, weight));
			}

			prevPosition = position;
		}
	}
	else{
		i2d::CVector2d prevPosition = polyline.GetNodePos(size - 1);

		for (int i = 0; i < size; ++i){
			const i2d::CVector2d& position = polyline.GetNodePos(i);
			if ((position != prevPosition) || (!m_isClosed && (i == 0))){
				m_nodes.push_back(CEdgeNode(position, weight));
			}

			prevPosition = position;
		}
	}
}


void CEdgeLine::CopyToPolyline(i2d::CPolyline& polyline, const i2d::CAffine2d* transformPtr) const
{
	istd::CChangeNotifier notifier(&polyline, &GetAllChanges());
	Q_UNUSED(notifier);

	polyline.SetNodesCount(int(m_nodes.size()));
	int index = 0;

	if (transformPtr != NULL){
		for (		Nodes::const_iterator iter = m_nodes.begin();
					iter != m_nodes.end();
					++iter){
			const CEdgeNode& node = *iter;

			polyline.SetNodePos(index++, transformPtr->GetApply(node.GetPosition()));
		}
	}
	else{
		for (		Nodes::const_iterator iter = m_nodes.begin();
					iter != m_nodes.end();
					++iter){
			const CEdgeNode& node = *iter;

			polyline.SetNodePos(index++, node.GetPosition());
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
	Q_UNUSED(notifier);

	i2d::CVector2d diffVector = position - m_center;

	for (		Nodes::iterator iter = m_nodes.begin();
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
	Q_UNUSED(notifier);

	int transFlag = transformation.GetTransformationFlags();
	if ((transFlag & i2d::ITransformation2d::TF_AFFINE) != 0){
		i2d::CAffine2d localTransform;
		transformation.GetLocalTransform(i2d::CVector2d(0, 0), localTransform);

		for (		Nodes::iterator iter = m_nodes.begin();
					iter != m_nodes.end();
					++iter){
			CEdgeNode& node = *iter;

			node.SetPosition(localTransform.GetApply(node.GetPosition()));
		}
	}
	else{
		for (		Nodes::iterator iter = m_nodes.begin();
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
	Q_UNUSED(notifier);

	int transFlag = transformation.GetTransformationFlags();
	if ((transFlag & i2d::ITransformation2d::TF_AFFINE) != 0){
		i2d::CAffine2d localInvTransform;
		transformation.GetLocalInvTransform(i2d::CVector2d(0, 0), localInvTransform);

		for (		Nodes::iterator iter = m_nodes.begin();
					iter != m_nodes.end();
					++iter){
			CEdgeNode& node = *iter;

			node.SetPosition(localInvTransform.GetApply(node.GetPosition()));
		}
	}
	else{
		for (		Nodes::iterator iter = m_nodes.begin();
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
		istd::CChangeNotifier notifier(resultEdgeLinePtr);
		Q_UNUSED(notifier);

		resultEdgeLinePtr->SetNodesCount(int(m_nodes.size()));

		int transFlag = transformation.GetTransformationFlags();
		if ((transFlag & i2d::ITransformation2d::TF_AFFINE) != 0){
			i2d::CAffine2d localTransform;
			transformation.GetLocalTransform(i2d::CVector2d(0, 0), localTransform);

			int index = 0;
			for (		Nodes::const_iterator iter = m_nodes.begin();
						iter != m_nodes.end();
						++iter){
				const CEdgeNode& node = *iter;

				CEdgeNode resultNode(
							localTransform.GetApply(node.GetPosition()),
							node.GetWeight());

				resultEdgeLinePtr->m_nodes[index++] = resultNode;
			}
		}
		else{
			int index = 0;
			for (		Nodes::const_iterator iter = m_nodes.begin();
						iter != m_nodes.end();
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

				resultEdgeLinePtr->m_nodes[index++] = resultNode;
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
		istd::CChangeNotifier notifier(resultEdgeLinePtr);
		Q_UNUSED(notifier);

		resultEdgeLinePtr->SetNodesCount(int(m_nodes.size()));

		int transFlag = transformation.GetTransformationFlags();
		if ((transFlag & i2d::ITransformation2d::TF_AFFINE) != 0){
			i2d::CAffine2d localInvTransform;
			transformation.GetLocalInvTransform(i2d::CVector2d(0, 0), localInvTransform);

			int index = 0;
			for (		Nodes::const_iterator iter = m_nodes.begin();
						iter != m_nodes.end();
						++iter){
				const CEdgeNode& node = *iter;

				CEdgeNode resultNode(
							localInvTransform.GetApply(node.GetPosition()),
							node.GetWeight());

				resultEdgeLinePtr->m_nodes[index++] = resultNode;
			}
		}
		else{
			int index = 0;
			for (		Nodes::const_iterator iter = m_nodes.begin();
						iter != m_nodes.end();
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

				resultEdgeLinePtr->m_nodes[index++] = resultNode;
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
	static iser::CArchiveTag nodesTag("Nodes", "List of edge nodes", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag nodeTag("Node", "Single edge node", iser::CArchiveTag::TT_GROUP, &nodesTag);
	static iser::CArchiveTag isClosedTag("IsClosed", "Flag indicating if edge is closed", iser::CArchiveTag::TT_LEAF, &nodesTag);

	bool retVal = true;

	if (archive.IsStoring()){
		int nodesCount = int(m_nodes.size());

		retVal = retVal && archive.BeginMultiTag(nodesTag, nodeTag, nodesCount);
		for (		Nodes::iterator iter = m_nodes.begin();
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


// protected members

void CEdgeLine::CalcVolatile() const
{
	m_totalLength = 0;

	int nodesCount = int(m_nodes.size());
	if (nodesCount > 0){
		const CEdgeNode& firstNode = m_nodes[0];
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

void CEdgeLine::OnEndChanges(const ChangeSet& /*changeSet*/)
{
	m_areVolatileValid = false;
}


} // namespace iedge


