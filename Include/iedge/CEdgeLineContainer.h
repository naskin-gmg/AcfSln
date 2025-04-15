#ifndef iedge_CEdgeLineContainer_included
#define iedge_CEdgeLineContainer_included


// ACF includes
#include <ibase/TSerializableContainer.h>
#include <i2d/CObject2dBase.h>

// ACF-Solutions includes
#include <iedge/CEdgeLine.h>


namespace iedge
{


class CEdgeLineContainer:
			public ibase::TSerializableContainer<CEdgeLine, std::vector<CEdgeLine> >,
			public i2d::CObject2dBase
{
public:
	typedef ibase::TSerializableContainer<CEdgeLine, std::vector<CEdgeLine> > BaseClass;
	typedef i2d::CObject2dBase BaseClass2;

	// reimplemented (ibase::TContainer)
	virtual void Reserve(int count) override;
	virtual void Resize(int count) override;

	// reimplemented (i2d::CObject2dBase)
	virtual void SetCalibration(const i2d::ICalibration2d* calibrationPtr, bool releaseFlag = false) override;

	// reimplemented (i2d::IObject2d)
	virtual i2d::CVector2d GetCenter() const override;
	virtual void MoveCenterTo(const i2d::CVector2d& position) override;
	virtual i2d::CRectangle GetBoundingBox() const override;
	virtual bool Transform(
				const i2d::ITransformation2d& transformation,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool InvTransform(
				const i2d::ITransformation2d& transformation,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool GetTransformed(
				const i2d::ITransformation2d& transformation,
				i2d::IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;
	virtual bool GetInvTransformed(
				const i2d::ITransformation2d& transformation,
				i2d::IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;

protected:
	// reimplemented (ibase::TSerializableContainer)
	virtual bool SerializeItem(CEdgeLine& item, iser::IArchive& archive, iser::CArchiveTag* parentTagPtr = NULL) override;
};


inline void CEdgeLineContainer::Reserve(int count)
{
	if (count >= 0){
		m_items.reserve(count);
	}
}


inline void CEdgeLineContainer::Resize(int count)
{
	if (count >= 0){
		m_items.resize(count);
	}
}


} // namespace iedge


#endif // !iedge_CEdgeLineContainer_included


