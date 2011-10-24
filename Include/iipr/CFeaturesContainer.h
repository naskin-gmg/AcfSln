#ifndef iipr_CFeaturesContainer_included
#define iipr_CFeaturesContainer_included


#include "istd/TPointerVector.h"

#include "iipr/IFeaturesConsumer.h"
#include "iipr/IFeaturesProvider.h"


namespace iipr
{


/**
	Basic implementation of a features container.
*/
class CFeaturesContainer:
			virtual public IFeaturesConsumer,
			virtual public IFeaturesProvider
{
public:
	// reimplemented (iipr::IFeaturesConsumer)
	virtual void ResetFeatures();
	virtual bool AddFeature(const iipr::IFeature* featurePtr, bool* isFullPtr = NULL);

	// reimplemented (iipr::IFeaturesProvider)
	virtual Features GetFeatures() const;

private:
	typedef istd::TPointerVector<iipr::IFeature> FeaturesList;

	FeaturesList m_featuresList;
};


} // namespace iipr


#endif // !iipr_CFeaturesContainer_included

