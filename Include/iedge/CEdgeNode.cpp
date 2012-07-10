#include "iedge/CEdgeNode.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iedge
{


CEdgeNode::CEdgeNode()
{
}


CEdgeNode::CEdgeNode(const i2d::CVector2d& position, const i2d::CVector2d& derivative, double weight)
:	m_position(position),
	m_derivative(derivative),
	m_weight(weight)
{
}


// reimplemented (iser::ISerializable)

bool CEdgeNode::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag positionTag("Position", "Position of edge node");
	static iser::CArchiveTag derivativeTag("Derivative", "Gradient (derivative) of edge node");
	static iser::CArchiveTag weightTag("Weight", "Weight of edge node");

	bool retVal = true;

	retVal = retVal && archive.BeginTag(positionTag);
	retVal = retVal && m_position.Serialize(archive);
	retVal = retVal && archive.EndTag(positionTag);

	retVal = retVal && archive.BeginTag(derivativeTag);
	retVal = retVal && m_derivative.Serialize(archive);
	retVal = retVal && archive.EndTag(derivativeTag);

	retVal = retVal && archive.BeginTag(weightTag);
	retVal = retVal && archive.Process(m_weight);
	retVal = retVal && archive.EndTag(weightTag);

	return retVal;
}


} // namespace iedge


