// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iipr/CCaliperParamsComp.h>


namespace iipr
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CCaliperParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	istd::CChangeNotifier notifier(this);

	SetWeightThreshold(*m_weightThresholdAttrPtr);
	SetPolarityMode(*m_polarityModeAttrPtr);
	SetDirectionMode(*m_directionModeAttrPtr);
}


} // namespace iipr


