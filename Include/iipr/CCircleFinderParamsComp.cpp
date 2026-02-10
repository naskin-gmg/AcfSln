// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iipr/CCircleFinderParamsComp.h>


namespace iipr
{


// reimplemented (iipr::ICircleFinderParams)

const imath::IUnitInfo* CCircleFinderParamsComp::GetDistanceUnitInfo() const
{
	return m_distanceUnitInfoCompPtr.GetPtr();
}


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


