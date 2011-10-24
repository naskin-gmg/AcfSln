#ifndef iipr_CCaliperFeature_included
#define iipr_CCaliperFeature_included


#include "iipr/CFeatureBase.h"


namespace iipr
{


/**
	Implementation of IFeature interface for caliper-extracted features.
*/
class CCaliperFeature: public CFeatureBase
{
public:
	typedef CFeatureBase BaseClass;

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
	int GetEdgeMode() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_position;
	int m_edgeMode;
};


} // namespace iipr


#endif // !iipr_CCaliperFeature_included


