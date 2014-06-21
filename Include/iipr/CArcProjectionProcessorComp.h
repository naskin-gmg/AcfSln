#ifndef iipr_CArcProjectionProcessorComp_included
#define iipr_CArcProjectionProcessorComp_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "icomp/CComponentBase.h"
#include "iproc/TSyncProcessorWrap.h"

#include "iipr/IArcProjectionProcessor.h"
#include "iipr/IProjectionConstraints.h"
#include "iipr/IFeatureToImageMapper.h"
#include "iipr/IFeatureToProjectionMapper.h"


namespace iipr
{


class CArcProjectionProcessorComp:
			public icomp::CComponentBase,
			public iproc::TSyncProcessorWrap<IArcProjectionProcessor>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef iproc::TSyncProcessorWrap<IArcProjectionProcessor> BaseClass2;

	I_BEGIN_COMPONENT(CArcProjectionProcessorComp);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_REGISTER_INTERFACE(IArcProjectionProcessor);
		I_ASSIGN(m_arcParamIdAttrPtr, "ArcParamId", "ID of arc parameter in parameter set", true, "ArcParam");
		I_ASSIGN(m_featureMapperCompPtr, "FeatureMapper", "Map result feature to projection position, it is needed for mapping operation", false, "FeatureMapper");
	I_END_COMPONENT;

	// reimplemented (iipr::IArcProjectionProcessor)
	virtual bool DoProjection(
			const iimg::IBitmap& bitmap,
			const i2d::CArc& projectionArc,
			const IProjectionParams* paramsPtr,
			imeas::IDataSequence& results);

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


