#ifndef iipr_CCaliperDistanceFeature_included
#define iipr_CCaliperDistanceFeature_included


// ACF includes
#include <i2d/CLine2d.h>

// ACF-Solutions includes
#include <iipr/TWeightedFeatureWrap.h>


namespace iipr
{


/**
	Implementation of INumericValue interface for caliper based distance measurement feature.
*/
class CCaliperDistanceFeature: public iipr::TWeightedFeatureWrap<i2d::CLine2d>
{
public:
	typedef iipr::TWeightedFeatureWrap<i2d::CLine2d> BaseClass;

	CCaliperDistanceFeature(const i2d::CLine2d& caliperLine, double weight);

	double GetDistance() const;

	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const override;
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const override;
};


} // namespace iipr


#endif // !iipr_CCaliperDistanceFeature_included


