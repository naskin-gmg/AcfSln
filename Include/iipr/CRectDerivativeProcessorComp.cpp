// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iipr/CRectDerivativeProcessorComp.h>


namespace iipr
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CRectDerivativeProcessorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(m_filterParamsIdAttrPtr.IsValid());	// isObligatory was set to 'true'
	SetFilterParamsId(*m_filterParamsIdAttrPtr);

	UseDoublePrecision(*m_doublePrecisionAttrPtr);
}


} // namespace iipr

