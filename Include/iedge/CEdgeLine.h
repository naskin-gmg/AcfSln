#ifndef iedge_CEdgeLine_included
#define iedge_CEdgeLine_included


// ACF includes
#include "ibase/TSerializableContainer.h"
#include "i2d/CObject2dBase.h"
#include "i2d/CPolyline.h"
#include "i2d/CAffine2d.h"

// ACF-Solutions includes
#include "iedge/CEdgeNode.h"


namespace iedge
{


class CEdgeLine: public i2d::CObject2dBase
{
public:
	typedef i2d::CObject2dBase BaseClass;

	CEdgeLine();

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
	virtual void CalcVolatile() const;

private:
	typedef QList<CEdgeNode> Nodes;

	Nodes m_nodes;

	bool m_isClosed;

	mutable bool m_areVolatileValid;
	mutable double m_totalLength;
	mutable i2d::CVector2d m_center;
	mutable double m_minWeight;
	mutable double m_maxWeight;
};


// inline methods

inline int CEdgeLine::GetNodesCount() const
{
	return m_nodes.size();
}


inline int CEdgeLine::GetSegmentsCount() const
{
	return m_isClosed? m_nodes.size(): m_nodes.size() - 1;
}


inline double CEdgeLine::GetTotalLength() const
{
	if (!m_areVolatileValid){
		CalcVolatile();
	}

	return m_totalLength;
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


} // namespace iedge


#endif // !iedge_CEdgeLine_included


