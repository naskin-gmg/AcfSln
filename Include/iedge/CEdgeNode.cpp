#include "iedge/CEdgeNode.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iedge
{


CEdgeNode::CEdgeNode()
	:m_position(0.0, 0.0),
	m_weight(0)
{
}


CEdgeNode::CEdgeNode(const i2d::CVector2d& position, double weight)
	:m_position(position),
	m_weight(weight)
{
}


// reimplemented (iser::ISerializable)

bool CEdgeNode::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag positionTag("Position", "Position of edge node", iser::CArchiveTag::TT_GROUP);
	static iser::CArchiveTag weightTag("Weight", "Weight of edge node", iser::CArchiveTag::TT_LEAF);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(positionTag);
	retVal = retVal && m_position.Serialize(archive);
	retVal = retVal && archive.EndTag(positionTag);

	retVal = retVal && archive.BeginTag(weightTag);
	retVal = retVal && archive.Process(m_weight);
	retVal = retVal && archive.EndTag(weightTag);

	return retVal;
}


} // namespace iedge


