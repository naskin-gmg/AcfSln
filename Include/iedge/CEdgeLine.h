#ifndef iedge_CEdgeLine_included
#define iedge_CEdgeLine_included


// ACF includes
#include <ibase/TSerializableContainer.h>
#include <i2d/CObject2dBase.h>
#include <i2d/CPolyline.h>
#include <i2d/CAffine2d.h>

// ACF-Solutions includes
#include <iedge/CEdgeNode.h>


namespace iedge
{


class CEdgeLine: public i2d::CObject2dBase
{
public:
	typedef i2d::CObject2dBase BaseClass;

	CEdgeLine();
	CEdgeLine(const CEdgeLine& edgeLine);

	void Clear();

	/**
		Get number of nodes.
	*/
	int GetNodesCount() const;
	/**
		Get number of segments.
	*/
	int GetSegmentsCount() const;

	/**
		Set number of nodes in container without initializing it.
	*/
	void SetNodesCount(int count);

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

	double GetTotalLength() const;
	double GetMinWeight() const;
	double GetMaxWeight() const;

	bool InsertNode(const CEdgeNode& node);

	/**
		Create this line using polyline object.
		Please note, that no calibration will be copied.
		\param	polyline		source polyline object.
		\param	weight			weight used for all nodes.
		\param	transformPtr	optional transformation used to convert node positions.
	*/
	void CopyFromPolyline(const i2d::CPolyline& polyline, double weight = 1.0, const i2d::CAffine2d* transformPtr = NULL);
	/**
		Copy this line to polyline.
		Please note, that no calibration will be copied.
		\param	polyline		result polyline object.
		\param	transformPtr	optional transformation used to convert node positions.
	*/
	void CopyToPolyline(i2d::CPolyline& polyline, const i2d::CAffine2d* transformPtr = NULL) const;

	// reimplemented (i2d::IObject2d)
	virtual i2d::CVector2d GetCenter() const;
	virtual void MoveCenterTo(const i2d::CVector2d& position);
	virtual i2d::CRectangle GetBoundingBox() const;
	virtual bool Transform(
				const i2d::ITransformation2d& transformation,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool InvTransform(
				const i2d::ITransformation2d& transformation,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool GetTransformed(
				const i2d::ITransformation2d& transformation,
				IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;
	virtual bool GetInvTransformed(
				const i2d::ITransformation2d& transformation,
				IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	/**
		Make sure, that all volatile values are calculated and valid.
	*/
	void EnsureVolatileValid() const;

	virtual void CalcVolatile() const;

	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(const ChangeSet& changeSet);

private:
	// std::vector used instead of QList/QVector because it is generally faster
	// (up to 300% for MSVC)
	typedef std::vector<CEdgeNode> Nodes;

	Nodes m_nodes;

	bool m_isClosed;

	mutable bool m_areVolatileValid;
	mutable double m_totalLength;
	mutable i2d::CVector2d m_center;
	mutable double m_minWeight;
	mutable double m_maxWeight;
	mutable i2d::CRectangle m_boundingBox;
};


// inline methods

inline int CEdgeLine::GetNodesCount() const
{
	return int(m_nodes.size());
}


inline int CEdgeLine::GetSegmentsCount() const
{
	return m_isClosed? int(m_nodes.size()): int(m_nodes.size() - 1);
}


inline const CEdgeNode& CEdgeLine::GetNode(int index) const
{
	return m_nodes[index];
}


inline CEdgeNode& CEdgeLine::GetNodeRef(int index)
{
	return m_nodes[index];
}


inline double CEdgeLine::GetTotalLength() const
{
	EnsureVolatileValid();

	return m_totalLength;
}


inline double CEdgeLine::GetMinWeight() const
{
	EnsureVolatileValid();

	return m_minWeight;
}


inline double CEdgeLine::GetMaxWeight() const
{
	EnsureVolatileValid();

	return m_maxWeight;
}


inline bool CEdgeLine::IsClosed() const
{
	return m_isClosed;
}


// protected methods

inline void CEdgeLine::EnsureVolatileValid() const
{
	if (!m_areVolatileValid){
		CalcVolatile();

		m_areVolatileValid = true;
	}
}


} // namespace iedge


#endif // !iedge_CEdgeLine_included


