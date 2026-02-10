// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icalib/CLUTCalibration2dComp.h>


// ACF incldues
#include <istd/CChangeGroup.h>


namespace icalib
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CLUTCalibration2dComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetArgumentUnitInfo(m_argumentUnitInfoCompPtr.GetPtr());
	SetResultUnitInfo(m_resultUnitInfoCompPtr.GetPtr());
}


void CLUTCalibration2dComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	istd::CChangeGroup changeGroup(this);
	Q_UNUSED(changeGroup);

	SetArgumentUnitInfo(NULL);
	SetResultUnitInfo(NULL);
}


} // namespace icalib


