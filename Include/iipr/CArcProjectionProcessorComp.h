#ifndef iipr_CArcProjectionProcessorComp_included
#define iipr_CArcProjectionProcessorComp_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "icomp/CComponentBase.h"
#include "iimg/IBitmap.h"
#include "i2d/CArc.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"
#include "iproc/TSyncProcessorWrap.h"
#include "iproc/TSyncProcessorCompBase.h"
#include "iipr/IProjectionConstraints.h"
#include "iipr/IFeatureToImageMapper.h"
#include "iipr/IFeatureToProjectionMapper.h"
#include "iipr/IProjectionParams.h"


namespace iipr
{


class CArcProjectionProcessorComp:
			public iproc::CSyncProcessorCompBase,
			virtual public IFeatureToImageMapper
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef iproc::CSyncProcessorCompBase BaseClass2;

	I_BEGIN_COMPONENT(CArcProjectionProcessorComp);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_REGISTER_INTERFACE(IFeatureToImageMapper);
		I_ASSIGN(m_arcParamIdAttrPtr, "ArcParamId", "ID of arc parameter in parameter set", true, "ArcParam");
		I_ASSIGN(m_featureMapperCompPtr, "FeatureMapper", "Map result feature to projection position, it is needed for mapping operation", false, "FeatureMapper");
	I_END_COMPONENT;

	// reimplemented (iipr::IArcProjectionProcessor)
	virtual bool DoProjection(
				const iimg::IBitmap& bitmap,
				const i2d::CArc& projectionArc,
				const IProjectionParams* paramsPtr,
				imeas::IDataSequence& results);

	// reimplemented (iipr::IFeatureToImageMapper)
	virtual bool GetImagePosition(
				const imeas::INumericValue& feature,
				const iprm::IParamsSet* paramsPtr,
				i2d::CVector2d& result) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

private:
	I_ATTR(QByteArray, m_arcParamIdAttrPtr);
	I_REF(IFeatureToProjectionMapper, m_featureMapperCompPtr);
};


} // namespace iipr


#endif // !iipr_CArcProjectionProcessorComp_included


