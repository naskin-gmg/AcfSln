#pragma once


// ACF-Solutions includes
#include <icalib/CPointGrid.h>
#include <iipr/TWeightedFeatureWrap.h>


namespace iipr
{


class CPointGridFeature: public TWeightedFeatureWrap<icalib::CPointGrid>
{
public:
	typedef TWeightedFeatureWrap<icalib::CPointGrid> BaseClass;

	CPointGridFeature();

	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const;
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const;

	// reimplemented (istd::IChangeable)
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
};


} // namespace iipr


