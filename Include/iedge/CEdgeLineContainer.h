#ifndef iedge_CEdgeLineContainer_included
#define iedge_CEdgeLineContainer_included


// ACF includes
#include "ibase/TSerializableContainer.h"
#include "i2d/CObject2dBase.h"

// ACF-Solutions includes
#include "iedge/CEdgeLine.h"


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
	virtual void Reserve(int count);
	virtual void Resize(int count);

	// reimplemented (i2d::CObject2dBase)
	virtual void SetCalibration(const i2d::ICalibration2d* calibrationPtr, bool releaseFlag = false);

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
				i2d::IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;
	virtual bool GetInvTransformed(
				const i2d::ITransformation2d& transformation,
				i2d::IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

protected:
	// reimplemented (ibase::TSerializableContainer)
	virtual bool SerializeItem(CEdgeLine& item, iser::IArchive& archive);
};


} // namespace iedge


#endif // !iedge_CEdgeLineContainer_included


