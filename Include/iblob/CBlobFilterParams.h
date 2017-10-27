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
	virtual const iprm::IOptionsList* GetSupportedProperties() const;
	virtual bool IsFiltersEnabled() const;
	virtual void SetFiltersEnabled(bool enabled = true);
	virtual int GetFiltersCount() const;
	virtual Filter& GetFilterAt(int index);
	virtual const Filter& GetFilterAt(int index) const;
	virtual void RemoveFilterAt(int index);
	virtual void ResetFilters();
	virtual void AddFilter(const Filter& filterInfo);

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual bool Serialize(iser::IArchive& archive);

private:
	typedef QList<Filter> FilterList;
	FilterList m_filters;

	bool m_filtersEnabled;

	const iprm::IOptionsList* m_supportedFeaturesListPtr;
};


} // namespace iblob


