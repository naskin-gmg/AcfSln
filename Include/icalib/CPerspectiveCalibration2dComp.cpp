// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icalib/CPerspectiveCalibration2dComp.h>


// ACF incldues
#include <istd/CChangeGroup.h>


namespace icalib
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CPerspectiveCalibration2dComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	istd::CChangeGroup changeGroup(this);
	Q_UNUSED(changeGroup);

	SetArgumentArea(m_argumentAreaCompPtr.GetPtr());
	SetResultArea(m_resultAreaCompPtr.GetPtr());
	SetArgumentUnitInfo(m_argumentUnitInfoCompPtr.GetPtr());
	SetResultUnitInfo(m_resultUnitInfoCompPtr.GetPtr());
}


void CPerspectiveCalibration2dComp::OnComponentDestroyed()
{
	istd::CChangeGroup changeGroup(this);
	Q_UNUSED(changeGroup);

	Reset();

	SetArgumentArea(NULL);
	SetResultArea(NULL);
	SetArgumentUnitInfo(NULL);
	SetResultUnitInfo(NULL);

	BaseClass::OnComponentDestroyed();
}


} // namespace icalib


