#ifndef iipr_CCaliperFeature_included
#define iipr_CCaliperFeature_included


#include "imeas/CSimpleNumericValue.h"


namespace iipr
{


/**
	Implementation of INumericValue interface for caliper-extracted features.
*/
class CCaliperFeature: public imeas::CSimpleNumericValue
{
public:
	typedef imeas::CSimpleNumericValue BaseClass;

	enum EdgeMode
	{
		EM_FALLING,
		EM_RISING
	};

	CCaliperFeature(
				double weight,
				double position,
				int edgeMode);

	double GetPosition() const;
	double GetWeight() const;
	int GetEdgeMode() const;

	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const;
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_position;
	double m_weight;
	int m_edgeMode;
};


} // namespace iipr


#endif // !iipr_CCaliperFeature_included


