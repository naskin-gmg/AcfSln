#ifndef iipr_IFeature_included
#define iipr_IFeature_included


// ACF includes
#include "iser/ISerializable.h"

#include "iipr/iipr.h"


namespace iipr
{


/**
	Base interface for all features can be consumed by feature consumer.
*/
class IFeature: virtual public iser::ISerializable
{
public:
	/**
		Get weigth of this feature.
	*/
	virtual double GetWeight() const = 0;
};


} // namespace iipr


#endif // !iipr_IFeature_included


