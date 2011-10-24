#ifndef iipr_CCircleFinderParamsComp_included
#define iipr_CCircleFinderParamsComp_included


#include "icomp/CComponentBase.h"

#include "iipr/CCircleFinderParams.h"


namespace iipr
{


class CCircleFinderParamsComp: public icomp::CComponentBase, public iipr::CCircleFinderParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCircleFinderParamsComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(iipr::ICircleFinderParams);
		I_ASSIGN(m_isOutlierEliminationEnabledAttrPtr, "OutlierEliminationEnabled", "Enable outliers elimination", false, false);
		I_ASSIGN(m_minOutlierDistanceAttrPtr, "MinOutlierDistance", "Minimal outliers distance in pixel", false, 5.0);
		I_ASSIGN(m_raysCountAttrPtr, "RaysCount", "Maximal number of projection rays, -1 for automatic calculation", false, -1);
		I_ASSIGN(m_caliperModeAttrPtr, "CaliperMode", "Mode of caliper\n\t0 - first candidate\n\t1 - best candidate", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(bool, m_isOutlierEliminationEnabledAttrPtr);
	I_ATTR(double, m_minOutlierDistanceAttrPtr);
	I_ATTR(int, m_raysCountAttrPtr);
	I_ATTR(int, m_caliperModeAttrPtr);
};


} // namespace iipr


#endif // !iipr_CCircleFinderParamsComp_included


