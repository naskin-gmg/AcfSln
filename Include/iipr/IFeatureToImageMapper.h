#ifndef iipr_IFeatureToImageMapper_included
#define iipr_IFeatureToImageMapper_included


// ACF includes
#include "i2d/CVector2d.h"
#include "iprm/IParamsSet.h"


namespace iipr
{


class IFeature;


/**
	Interface allowing mapping of extracted features to native coordinate systems.
*/
class IFeatureToImageMapper: virtual public istd::IPolymorphic
{
public:
	/**
		Get position in image coordination system using position extracted from projection.
	*/
	virtual bool GetImagePosition(
				const IFeature& feature,
				const iprm::IParamsSet* paramsPtr,
				i2d::CVector2d& result) const = 0;
};


} // namespace iipr


#endif // !iipr_IFeatureToImageMapper_included


