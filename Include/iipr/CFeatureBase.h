#ifndef iipr_CFeatureBase_included
#define iipr_CFeatureBase_included


#include "iipr/IFeature.h"


namespace iipr
{


/**
	Base implementation of IFeature interface to reduce overhead by interface changes.
*/
class CFeatureBase: virtual public iipr::IFeature
{
public:
	CFeatureBase(double weight = 1.0);

	void SetWeight(double weight);

	// reimplemented (iipr::IFeature)
	virtual double GetWeight() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_weight;
};


} // namespace iipr


#endif // !iipr_CFeatureBase_included


