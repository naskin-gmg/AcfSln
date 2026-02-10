// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iipr/CImageInterpolationParamsComp.h>


namespace iipr
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CImageInterpolationParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	istd::CChangeNotifier notifier(this);

	SetInterpolationMode(*m_interpolationModeAttrPtr);
}


} // namespace iipr


