// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iedge_CEdgeNode_included
#define iedge_CEdgeNode_included


// ACF includes
#include <iser/ISerializable.h>
#include <i2d/CVector2d.h>


namespace iedge
{


/**
	Description of single node of the edge.
	Node has position and weight.
*/
class CEdgeNode: virtual public iser::ISerializable
{
public:
	CEdgeNode();
	CEdgeNode(const i2d::CVector2d& position, double weight);

	/**
		Get position of node.
	*/
	const i2d::CVector2d& GetPosition() const;
	/**
		Set position of node.
	*/
	void SetPosition(const i2d::CVector2d& position);
	/**
		Get node weight.
		This value should be normalized (range [0, 1]).
	*/
	double GetWeight() const;
	/**
		Set node weight.
		This value should be normalized (range [0, 1]).
	*/
	void SetWeight(double weight);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	/**
		Position of edge node.
	*/
	i2d::CVector2d m_position;

	/**
		Weight value of this point.
	*/
	double m_weight;
};


// inline methods

inline const i2d::CVector2d& CEdgeNode::GetPosition() const
{
	return m_position;
}


inline void CEdgeNode::SetPosition(const i2d::CVector2d& position)
{
	m_position = position;
}


inline double CEdgeNode::GetWeight() const
{
	return m_weight;
}


inline void CEdgeNode::SetWeight(double weight)
{
	m_weight = weight;
}


} // namespace iedge


#endif // !iedge_CEdgeNode_included


