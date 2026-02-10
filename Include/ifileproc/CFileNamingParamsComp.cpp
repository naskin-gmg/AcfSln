// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifileproc/CFileNamingParamsComp.h>


namespace ifileproc
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CFileNamingParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_prefix = *m_prefixAttrPtr;
	m_suffix = *m_suffixAttrPtr;
	m_overwriteStrategy = OverwriteStrategy(*m_overwriteStrategyAttrPtr);
}


} // namespace ifileproc


