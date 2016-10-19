#ifndef iipr_IFeaturesContainer_included
#define iipr_IFeaturesContainer_included


#include <iipr/IFeaturesConsumer.h>


namespace iipr
{


/**
	Container allowing access to features.
*/
class IFeaturesProvider: virtual public istd::IChangeable
{
public:
	/**
		Get number of stored features.
	*/
	virtual int GetFeaturesCount() const = 0;
	/**
		Get access to single feature.
	*/
	virtual const imeas::INumericValue& GetFeature(int index) const = 0;
};


} // namespace iipr


#endif // !iipr_IFeaturesContainer_included

