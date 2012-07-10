#ifndef iedge_CEdgeNode_included
#define iedge_CEdgeNode_included


// ACF includes
#include "iser/ISerializable.h"
#include "i2d/CVector2d.h"


namespace iedge
{


class CEdgeNode: virtual public iser::ISerializable
{
public:
	CEdgeNode();
	CEdgeNode(const i2d::CVector2d& position, const i2d::CVector2d& derivative, double weight);

	/**
		Get position of node.
	*/
	const i2d::CVector2d& GetPosition() const;
	/**
		Set position of node.
	*/
	void SetPosition(const i2d::CVector2d& position);
	/**
		Get direction (derivative) of node gradient.
	*/
	const i2d::CVector2d& GetDerivative() const;
	/**
		Set direction (derivative) of node gradient.
	*/
	void SetDerivative(const i2d::CVector2d& derivative);
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
	virtual bool Serialize(iser::IArchive& archive);

private:
	i2d::CVector2d m_position;
	/**	Derivative vector.
	*/
	i2d::CVector2d m_derivative;
	/**	Weight value of this point.
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


inline const i2d::CVector2d& CEdgeNode::GetDerivative() const
{
	return m_derivative;
}


inline void CEdgeNode::SetDerivative(const i2d::CVector2d& derivative)
{
	m_derivative = derivative;
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


