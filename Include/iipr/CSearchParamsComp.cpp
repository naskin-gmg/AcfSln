// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iipr/CSearchParamsComp.h>


namespace iipr
{


// public methods

// reimplemented (iipr::ISearchParams)

const ISearchConstraints* CSearchParamsComp::GetSearchConstraints() const
{
	return m_searchConstraintsCompPtr.IsValid() ? m_searchConstraintsCompPtr.GetPtr() : nullptr;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CSearchParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetNominalModelsCount(*m_defaultModelOccuranceAttrPtr);

	if (!m_defaultMinRotationAngleAttrPtr.IsValid() || !m_defaultMaxRotationAngleAttrPtr.IsValid())
		return;

	auto searchConstraints = GetSearchConstraints();
	const double minAngle = searchConstraints
		? std::max(*m_defaultMinRotationAngleAttrPtr, searchConstraints->GetRotationRangeConstraints().GetMinValue())
		: *m_defaultMinRotationAngleAttrPtr;
	const double maxAngle = searchConstraints
		? std::min(*m_defaultMaxRotationAngleAttrPtr, searchConstraints->GetRotationRangeConstraints().GetMaxValue())
		: *m_defaultMaxRotationAngleAttrPtr;

	SetRotationRange(istd::CRange{ minAngle, maxAngle });
}


} // namespace iipr


