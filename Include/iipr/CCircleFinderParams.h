#ifndef iipr_CCircleFinderParams_included
#define iipr_CCircleFinderParams_included


#include "iipr/ICircleFinderParams.h"


namespace iipr
{


class CCircleFinderParams: virtual public ICircleFinderParams
{
public:
	CCircleFinderParams();

	// reimplemented (iipr::ICircleFinderParams)
	virtual int GetCaliperMode() const;
	virtual void SetCaliperMode(int caliperMode);
	virtual bool IsOutlierEliminationEnabled() const;
	virtual void SetOutlierEliminationEnabled(bool isOutlierEliminationEnabled = true);
	virtual double GetMinOutlierDistance() const;
	virtual void SetMinOutlierDistance(double minOutlierDistance);
	virtual int GetRaysCount() const;
	virtual void SetRaysCount(int raysCount);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	bool m_isOutlierEliminationEnabled;
	double m_minOutlierDistance;
	int m_raysCount;
	int m_caliperMode;
};


} // namespace iipr


#endif // !iipr_CCircleFinderParams_included


