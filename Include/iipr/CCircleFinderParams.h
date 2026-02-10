// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_CCircleFinderParams_included
#define iipr_CCircleFinderParams_included


#include <iipr/ICircleFinderParams.h>


namespace iipr
{


class CCircleFinderParams: virtual public ICircleFinderParams
{
public:
	CCircleFinderParams();

	// reimplemented (iipr::ICircleFinderParams)
	virtual int GetCaliperMode() const override;
	virtual void SetCaliperMode(int caliperMode) override;
	virtual bool IsOutlierEliminationEnabled() const override;
	virtual void SetOutlierEliminationEnabled(bool isOutlierEliminationEnabled = true) override;
	virtual double GetMinOutlierDistance() const override;
	virtual void SetMinOutlierDistance(double minOutlierDistance) override;
	virtual const imath::IUnitInfo* GetDistanceUnitInfo() const override;
	virtual int GetRaysCount() const override;
	virtual void SetRaysCount(int raysCount) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	bool m_isOutlierEliminationEnabled;
	double m_minOutlierDistance;
	int m_raysCount;
	int m_caliperMode;
};


} // namespace iipr


#endif // !iipr_CCircleFinderParams_included


