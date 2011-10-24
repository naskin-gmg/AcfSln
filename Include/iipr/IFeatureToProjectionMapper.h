#ifndef iipr_IFeatureToProjectionMapper_included
#define iipr_IFeatureToProjectionMapper_included


// ACF includes
#include "i2d/CVector2d.h"
#include "iprm/IParamsSet.h"


namespace iipr
{


class IFeature;


/**
	Interface allowing mapping of extracted features to native coordinate systems.
*/
class IFeatureToProjectionMapper: virtual public istd::IPolymorphic
{
public:
	/**
		Get position on projection using position extracted from projection.
	*/
	virtual bool GetProjectionPosition(
				const IFeature& feature,
				const iprm::IParamsSet* paramsPtr,
				double& result) const = 0;
};


} // namespace iipr


#endif // !iipr_IFeatureToProjectionMapper_included


