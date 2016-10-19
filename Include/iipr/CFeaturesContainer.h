#ifndef iipr_CFeaturesContainer_included
#define iipr_CFeaturesContainer_included


#include <istd/TPointerVector.h>

#include <iipr/IFeaturesConsumer.h>
#include <iipr/IFeaturesProvider.h>


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
	/**
		Remove feature at selected index.
		The following elements will be shift, size of this container will be decreased.
	*/
	void RemoveFeature(int featureIndex);
	/**
		Get access to stored feature.
	*/
	imeas::INumericValue& GetFeatureRef(int index);

	// reimplemented (iipr::IFeaturesConsumer)
	virtual void ResetFeatures();
	virtual bool AddFeature(const imeas::INumericValue* featurePtr, bool* isFullPtr = NULL);

	// reimplemented (iipr::IFeaturesProvider)
	virtual int GetFeaturesCount() const;
	virtual const imeas::INumericValue& GetFeature(int index) const;

private:
	typedef istd::TPointerVector<imeas::INumericValue> FeaturesList;

	FeaturesList m_featuresList;
};


} // namespace iipr


#endif // !iipr_CFeaturesContainer_included

