#include "iipr/CCircleFinderParamsComp.h"


namespace iipr
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CCircleFinderParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_isOutlierEliminationEnabledAttrPtr.IsValid()){
		SetOutlierEliminationEnabled(*m_isOutlierEliminationEnabledAttrPtr);
	}

	if (m_minOutlierDistanceAttrPtr.IsValid()){
		SetMinOutlierDistance(*m_minOutlierDistanceAttrPtr);
	}

	if (m_raysCountAttrPtr.IsValid()){
		SetRaysCount(*m_raysCountAttrPtr);
	}

	SetCaliperMode(*m_caliperModeAttrPtr);
}


} // namespace iipr


