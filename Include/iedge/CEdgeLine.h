#ifndef iedge_CEdgeLine_included
#define iedge_CEdgeLine_included


// ACF includes
#include "iser/ISerializable.h"
#include "ibase/TSerializableContainer.h"
#include "i2d/CPolyline.h"
#include "i2d/CAffine2d.h"

// QSF includes
#include "iedge/CEdgeNode.h"


namespace iedge
{


class CEdgeLine: virtual public iser::ISerializable
{
public:
	class Container;

	CEdgeLine();
	CEdgeLine(const CEdgeLine& iedge);

	void Clear();

	/**
		Return node at specified index.
		\param	index	an index in node table.
	*/
	const CEdgeNode& GetNode(int index) const;
	/**
		Return reference to node at specified index.
		\param	index	an index in node table.
	*/
	CEdgeNode& GetNodeRef(int index);

	/**
		Set node at specified index.
		\param	index	index in node table.
		\param	node	new node position.
	*/
	void SetNode(int index, const CEdgeNode& node);

	bool IsClosed() const;
	void SetClosed(bool state = true);

	int GetNodesCount() const;
	int GetSegmentsCount() const;

	double GetMinWeight() const;
	double GetMaxWeight() const;

	bool InsertNode(const CEdgeNode& node);

	void CopyFromPolyline(const i2d::CPolyline& polyline, const i2d::CAffine2d& transform, double weight = 1.0);
	void CopyToPolyline(i2d::CPolyline& polyline, const i2d::CAffine2d& transform) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	virtual void CalcVolatile() const;

private:
	typedef QList<CEdgeNode> Nodes;

	Nodes m_edgeLines;

	bool m_isClosed;

	mutable bool m_areVolatileValid;
	mutable double m_minWeight;
	mutable double m_maxWeight;
};


// embedded class definitions

class CEdgeLine::Container: public ibase::TSerializableContainer<CEdgeLine>
{
protected:
	// reimplemented (ibase::TSerializableContainer)
	virtual bool SerializeItem(CEdgeLine& item, iser::IArchive& archive);
};


// inline methods

inline int CEdgeLine::GetNodesCount() const
{
	return m_edgeLines.size();
}


inline int CEdgeLine::GetSegmentsCount() const
{
	return m_isClosed? m_edgeLines.size(): m_edgeLines.size() - 1;
}


inline double CEdgeLine::GetMinWeight() const
{
	if (!m_areVolatileValid){
		CalcVolatile();
	}

	return m_minWeight;
}


inline double CEdgeLine::GetMaxWeight() const
{
	if (!m_areVolatileValid){
		CalcVolatile();
	}

	return m_maxWeight;
}


inline bool CEdgeLine::IsClosed() const
{
	return m_isClosed;
}


inline void CEdgeLine::SetClosed(bool state)
{
	m_isClosed = state;
}


} // namespace iedge


#endif // !iedge_CEdgeLine_included


