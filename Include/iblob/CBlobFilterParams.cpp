#include <iblob/CBlobFilterParams.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace iblob
{


static BlobDescriptorInfoList s_defaultDescriptorsList = BlobDescriptorInfoList()
			<< CBlobDescriptorInfo(CBlobDescriptorInfo::BDT_AREA, "Area", "Area", "Blob area")
			<< CBlobDescriptorInfo(CBlobDescriptorInfo::BDT_CIRCULARITY, "Circularity", "Circularity", "Circularity factor of the blob")
			<< CBlobDescriptorInfo(CBlobDescriptorInfo::BDT_PERIMETER, "Perimeter", "Perimeter", "Total length of edges in a blob (including the edges of any holes)");


CBlobFilterParams::CBlobFilterParams()
	:m_filtersEnabled(false)
{
}


// reimplemented (IBlobFilterParams)

const BlobDescriptorInfoList* CBlobFilterParams::GetSupportedDescriptorsList() const
{
	return &s_defaultDescriptorsList;
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


// reimplemented (iser::ISerializable)

bool CBlobFilterParams::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag filtersTag("Filters", "Blob Filters", iser::CArchiveTag::TT_MULTIPLE, NULL);
	static iser::CArchiveTag filterTag("Filter", "Single Blob Filter", iser::CArchiveTag::TT_GROUP, &filtersTag);

	bool retVal = true;

	istd::CChangeNotifier notifier(archive.IsStoring() ? NULL : this);


	retVal = retVal && archive.Process(m_filtersEnabled);

	
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

		static iser::CArchiveTag filterConditionTag("FilterCondition", "Filter condition", iser::CArchiveTag::TT_LEAF, &filterTag);
		retVal = retVal && archive.BeginTag(filterConditionTag);
		retVal = retVal && I_SERIALIZE_ENUM(FilterCondition, archive, filterInfo.condition);
		retVal = retVal && archive.EndTag(filterConditionTag);

		static iser::CArchiveTag filterOperationTag("FilterOperation", "Filter operation", iser::CArchiveTag::TT_LEAF, &filterTag);
		retVal = retVal && archive.BeginTag(filterOperationTag);
		retVal = retVal && I_SERIALIZE_ENUM(FilterOperation, archive, filterInfo.operation);
		retVal = retVal && archive.EndTag(filterOperationTag);

		static iser::CArchiveTag valueRangeTag("ValueRange", "Filter's value range", iser::CArchiveTag::TT_LEAF, &filterTag);
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
