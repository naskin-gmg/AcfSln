#pragma once


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include <istd/TRange.h>
#include <iser/ISerializable.h>

// ACF-Solutions includes
#include <iblob/CBlobDescriptorInfo.h>


namespace iblob
{


/**
	Interface for parameters of the blob filtering.
*/
class IBlobFilterParams: virtual public iser::ISerializable
{
public:
	enum FilterOperation
	{
		/**
			If the extracted blob feature matches specified conditions, it will be passed through.
		*/
		FO_INCLUDE,

		/**
			If the extracted blob feature matches specified conditions, it will be exculded from the result list.
		*/
		FO_EXCLUDE
	};

	I_DECLARE_ENUM(FilterOperation, FO_INCLUDE, FO_EXCLUDE)

	enum FilterCondition
	{
		FC_BETWEEN,
		FC_OUTSIDE,
		FC_LESS,
		FC_EQUAL,
		FC_GREATER,
		FC_LESS_EQUAL,
		FC_GREATER_EQUAL,
		FC_NOT_EQUAL
	};

	I_DECLARE_ENUM(
		FilterCondition,
		FC_BETWEEN,
		FC_OUTSIDE,
		FC_LESS,
		FC_EQUAL,
		FC_GREATER,
		FC_LESS_EQUAL,
		FC_GREATER_EQUAL,
		FC_NOT_EQUAL)

	struct Filter
	{
		Filter()
			:blobDescriptorType(CBlobDescriptorInfo::BDT_USER),
			operation(FO_INCLUDE),
			condition(FC_BETWEEN)
		{
		}

		/**
			\sa CBlobDescriptorInfo::BlobDescriptorType
		*/
		int blobDescriptorType;
		FilterOperation operation;
		FilterCondition condition;
		istd::CRange valueRange;
	};

	virtual const BlobDescriptorInfoList* GetSupportedDescriptorsList() const = 0;
	virtual bool IsFiltersEnabled() const = 0;
	virtual void SetFiltersEnabled(bool enabled = true) = 0;
	virtual int GetFiltersCount() const = 0;
	virtual Filter& GetFilterAt(int index) = 0;
	virtual const Filter& GetFilterAt(int index) const = 0;
	virtual void RemoveFilterAt(int index) = 0;
	virtual void ResetFilters() = 0;
	virtual void AddFilter(const Filter& filterInfo) = 0;
};


} // namespace iblob


