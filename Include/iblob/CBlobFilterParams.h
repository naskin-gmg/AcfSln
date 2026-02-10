// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iblob/IBlobFilterParams.h>


namespace iblob
{


class CBlobFilterParams: virtual public IBlobFilterParams
{
public:
	CBlobFilterParams();

	/**
		Set list of supported features.
	*/
	void SetSupportedProperties(const iprm::IOptionsList* featureListPtr);

	// reimplemented (IBlobFilterParams)
	virtual bool IsValueAcceptedByFilter(const int filterIdx, const double& value) const override;
	virtual const iprm::IOptionsList* GetSupportedProperties() const override;
	virtual bool IsFiltersEnabled() const override;
	virtual void SetFiltersEnabled(bool enabled = true) override;
	virtual int GetFiltersCount() const override;
	virtual Filter& GetFilterAt(int index) override;
	virtual const Filter& GetFilterAt(int index) const override;
	virtual void RemoveFilterAt(int index) override;
	virtual void ResetFilters() override;
	virtual void AddFilter(const Filter& filterInfo) override;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const override;

	// reimplemented (i2d::ICalibrationProvider)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	typedef QList<Filter> FilterList;
	FilterList m_filters;

	bool m_filtersEnabled;

	const iprm::IOptionsList* m_supportedFeaturesListPtr;
};


} // namespace iblob


