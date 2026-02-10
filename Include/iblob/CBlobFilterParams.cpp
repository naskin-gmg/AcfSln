// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iblob/CBlobFilterParams.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace iblob
{


CBlobFilterParams::CBlobFilterParams()
:	m_filtersEnabled(false),
	m_supportedFeaturesListPtr(NULL)
{
}


void CBlobFilterParams::SetSupportedProperties(const iprm::IOptionsList* featureListPtr)
{
	m_supportedFeaturesListPtr = featureListPtr;
}


// reimplemented (IBlobFilterParams)

bool CBlobFilterParams::IsValueAcceptedByFilter(const int filterIdx, const double & value) const
{
	if (filterIdx < 0 || filterIdx >= m_filters.size()) {
		return false;
	}
	const auto& filter = m_filters[filterIdx];

	bool isGreater = value > filter.valueRange.GetMinValue();
	bool isLess = value < filter.valueRange.GetMinValue();
	bool isEqual = qFuzzyCompare(value, filter.valueRange.GetMinValue());
	bool isGreaterEqual = isGreater || isEqual;
	bool isLessEqual = isLess || isEqual;

	switch (filter.condition) {
	case iblob::IBlobFilterParams::FC_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isEqual : !isEqual;

	case iblob::IBlobFilterParams::FC_NOT_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? !isEqual : isEqual;

	case iblob::IBlobFilterParams::FC_BETWEEN:
	{
		if (!filter.valueRange.IsValid())
			return false;
		bool valueInRange = filter.valueRange.Contains(value);
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? valueInRange : !valueInRange;
	}

	case iblob::IBlobFilterParams::FC_OUTSIDE:
	{
		if (!filter.valueRange.IsValid())
			return false;
		bool valueInRange = filter.valueRange.Contains(value);
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? !valueInRange : valueInRange;
	}

	case iblob::IBlobFilterParams::FC_GREATER:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isGreater : !isGreater;

	case iblob::IBlobFilterParams::FC_GREATER_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isGreaterEqual : !isGreaterEqual;

	case iblob::IBlobFilterParams::FC_LESS:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isLess : !isLess;

	case iblob::IBlobFilterParams::FC_LESS_EQUAL:
		return (filter.operation == iblob::IBlobFilterParams::FO_INCLUDE) ? isLessEqual : !isLessEqual;

	default:
		return true;
	}
}


const iprm::IOptionsList* CBlobFilterParams::GetSupportedProperties() const
{
	return m_supportedFeaturesListPtr;
}


bool CBlobFilterParams::IsFiltersEnabled() const
{
	return m_filtersEnabled;
}


void CBlobFilterParams::SetFiltersEnabled(bool enabled)
{
	if (m_filtersEnabled != enabled){
		istd::CChangeNotifier changeNotifier(this);
	
		m_filtersEnabled = enabled;
	}
}


int CBlobFilterParams::GetFiltersCount() const
{
	return m_filters.count();
}


CBlobFilterParams::Filter& CBlobFilterParams::GetFilterAt(int index)
{
	Q_ASSERT(index >= 0 && index < m_filters.count());

	return m_filters[index];
}


const CBlobFilterParams::Filter& CBlobFilterParams::GetFilterAt(int index) const
{
	Q_ASSERT(index >= 0 && index < m_filters.count());

	return m_filters.at(index);
}


void CBlobFilterParams::RemoveFilterAt(int index)
{
	Q_ASSERT(index >= 0 && index < m_filters.count());

	istd::CChangeNotifier changeNotifier(this);

	m_filters.removeAt(index);
}


void CBlobFilterParams::ResetFilters()
{
	istd::CChangeNotifier changeNotifier(this);

	m_filters.clear();
}


void CBlobFilterParams::AddFilter(const Filter& filterInfo)
{
	istd::CChangeNotifier changeNotifier(this);

	m_filters.append(filterInfo);
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CBlobFilterParams::GetCalibration() const
{
	return NULL;
}


// helper

bool SerializeEnableFlag(iser::IArchive& archive, bool& filtersEnabled)
{
	static iser::CArchiveTag filterEnabledTag("BlobFilteringEnabled", "Blob filter enabled", iser::CArchiveTag::TT_LEAF);

	if (archive.IsStoring()) {
		archive.BeginTag(filterEnabledTag);
		archive.Process(filtersEnabled);
		archive.EndTag(filterEnabledTag);
		return true;
	}

	// read & skip if needed
	bool ok1 = archive.BeginTag(filterEnabledTag);
	bool ok2 = archive.Process(filtersEnabled);

	if (!ok1 || !ok2) {
		filtersEnabled = true;	// enabled by default
	}

	return archive.EndTag(filterEnabledTag);
}


// reimplemented (iser::ISerializable)

bool CBlobFilterParams::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag filtersTag("Filters", "Blob Filters", iser::CArchiveTag::TT_MULTIPLE, NULL);
	static iser::CArchiveTag filterTag("Filter", "Single Blob Filter", iser::CArchiveTag::TT_GROUP, &filtersTag);
	static iser::CArchiveTag featureIdTag("FeatureId", "ID of filtered feature", iser::CArchiveTag::TT_LEAF, &filterTag);
	static iser::CArchiveTag filterConditionTag("FilterCondition", "Filter condition", iser::CArchiveTag::TT_LEAF, &filterTag);
	static iser::CArchiveTag filterOperationTag("FilterOperation", "Filter operation", iser::CArchiveTag::TT_LEAF, &filterTag);
	static iser::CArchiveTag valueRangeTag("ValueRange", "Filter's value range", iser::CArchiveTag::TT_GROUP, &filterTag);

	istd::CChangeNotifier notifier(archive.IsStoring() ? NULL : this);

	bool retVal = SerializeEnableFlag(archive, m_filtersEnabled);

	int filterCount = m_filters.count();
	retVal = retVal && archive.BeginMultiTag(filtersTag, filterTag, filterCount);

	bool isStoring = archive.IsStoring();
	if (!isStoring){
		m_filters.clear();
	}

	for (int i = 0; i < filterCount; i++){
		Filter filterInfo;
		if (isStoring){
			filterInfo = m_filters.at(i);
		}

		retVal = retVal && archive.BeginTag(filterTag);

		retVal = retVal && archive.BeginTag(featureIdTag);
		retVal = retVal && archive.Process(filterInfo.featureId);
		retVal = retVal && archive.EndTag(featureIdTag);

		retVal = retVal && archive.BeginTag(filterConditionTag);
		retVal = retVal && I_SERIALIZE_ENUM(FilterCondition, archive, filterInfo.condition);
		retVal = retVal && archive.EndTag(filterConditionTag);

		retVal = retVal && archive.BeginTag(filterOperationTag);
		retVal = retVal && I_SERIALIZE_ENUM(FilterOperation, archive, filterInfo.operation);
		retVal = retVal && archive.EndTag(filterOperationTag);

		retVal = retVal && archive.BeginTag(valueRangeTag);
		retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeRange(archive, filterInfo.valueRange);
		retVal = retVal && archive.EndTag(valueRangeTag);

		retVal = retVal && archive.EndTag(filterTag);

		if (!isStoring){
			m_filters.append(filterInfo);
		}
	}

	retVal = retVal && archive.EndTag(filtersTag);

	return retVal;
}


} // namespace iblob
