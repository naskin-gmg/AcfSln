// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF-Solutions includes
#include <icalib/CPointGrid.h>
#include <iipr/TWeightedFeatureWrap.h>


namespace iipr
{


class CPointGridFeature: public TWeightedFeatureWrap<icalib::CPointGrid>
{
public:
	typedef TWeightedFeatureWrap<icalib::CPointGrid> BaseClass;

	CPointGridFeature();
	bool AddChaurcoIds(const std::vector<int>& ids);
	const std::vector<int>& GetCharucoIds() const;

	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const override;
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const override;

	// reimplemented (istd::IChangeable)
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

private:
	std::vector<int> m_charucoIds;
};


} // namespace iipr


