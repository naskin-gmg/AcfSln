#include "iedge/CEdgeLine.h"


namespace iedge
{


CEdgeLine::CEdgeLine()
:	m_isClosed(false)
{
}


CEdgeLine::CEdgeLine(const CEdgeLine& edge)
:	m_edgeLines(edge.m_edgeLines),
	m_isClosed(false)
{
}


void CEdgeLine::Clear()
{
	m_edgeLines.clear();
	m_isClosed = false;
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
}


bool CEdgeLine::InsertNode(const CEdgeNode& node)
{
	m_edgeLines.push_back(node);

	m_areVolatileValid = false;

	return true;
}


void CEdgeLine::CopyFromPolyline(const i2d::CPolyline& polyline, const i2d::CAffine2d& transform, double weight)
{
	int size = polyline.GetNodesCount();

	if (size <= 1){
		m_edgeLines.clear();
		m_isClosed = false;

		return;
	}

	m_isClosed = polyline.IsClosed();

	i2d::CVector2d prevDelta;
	i2d::CVector2d position;

	if (m_isClosed){
		i2d::CVector2d prevPosition = transform.GetApply(polyline.GetNode(size - 1));
		position = transform.GetApply(polyline.GetNode(0));
		prevDelta = position - prevPosition;
	}
	else{
		position = transform.GetApply(polyline.GetNode(0));
		prevDelta = i2d::CVector2d(0, 0);
	}

	for (int i = 0; i < size; ++i){
		const i2d::CVector2d& nextPosition = transform.GetApply(polyline.GetNode(m_isClosed? (i + 1) % size: qMin(i + 1, size - 1)));
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


void CEdgeLine::CopyToPolyline(i2d::CPolyline& polyline, const i2d::CAffine2d& transform) const
{
	polyline.Clear();

	for (		Nodes::ConstIterator iter = m_edgeLines.constBegin();
				iter != m_edgeLines.constBegin();
				++iter){
		const CEdgeNode& node = *iter;

		polyline.InsertNode(transform.GetApply(node.GetPosition()));
	}

	polyline.SetClosed(IsClosed());
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
					iter != m_edgeLines.begin();
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
	if (!m_edgeLines.isEmpty()){
		const CEdgeNode& firstNode = m_edgeLines.first();
		m_minWeight = firstNode.GetWeight();
		m_maxWeight = m_minWeight;

		for (		Nodes::ConstIterator iter = m_edgeLines.constBegin() + 1;
					iter != m_edgeLines.constBegin();
					++iter){
			const CEdgeNode& node = *iter;
			double weight = node.GetWeight();

			if (weight < m_minWeight){
				m_minWeight = weight;
			}
			else if (weight > m_maxWeight){
				m_maxWeight = weight;
			}
		}
	}
	else{
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


