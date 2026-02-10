// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icalib/CSimpleLensCorrectionComp.h>


namespace icalib
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CSimpleLensCorrectionComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetDistortionFactor(*m_defaultDistortionFactorAttrPtr);
	SetOpticalCenter(i2d::CVector2d(*m_defaultOpticalCenterXAttrPtr, *m_defaultOpticalCenterYAttrPtr));
}


} // namespace icalib


