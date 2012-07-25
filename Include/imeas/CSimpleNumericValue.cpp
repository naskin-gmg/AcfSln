#include "imeas/CSimpleNumericValue.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

// ACF-Solutions includes
#include "imeas/INumericConstraints.h"


namespace imeas
{


// public methods

// reimplemented (imeas::INumericValue)

bool CSimpleNumericValue::IsValueTypeSupported(ValueTypeId valueTypeId) const
{
	if (valueTypeId == VTI_AUTO){
		return true;
	}

	return false;
}


const INumericConstraints* CSimpleNumericValue::GetNumericConstraints() const
{
	return NULL;
}


imath::CVarVector CSimpleNumericValue::GetComponentValue(ValueTypeId valueTypeId) const
{
	I_ASSERT(valueTypeId == VTI_AUTO);

	if (valueTypeId != VTI_AUTO){
		return imath::CVarVector();
	}

	return m_values;
}


imath::CVarVector CSimpleNumericValue::GetValues() const
{
	return m_values;
}


bool CSimpleNumericValue::SetValues(const imath::CVarVector& values)
{
	if (m_values != values){
		const INumericConstraints* constraintsPtr = GetNumericConstraints();
		if ((constraintsPtr != NULL) && (values.GetElementsCount() != constraintsPtr->GetNumericValuesCount())){
			return false;
		}

		istd::CChangeNotifier notifier(this);

		m_values = values;
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool CSimpleNumericValue::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag valuesTag("Values", "List of numeric values");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool retVal = archive.BeginTag(valuesTag);
	retVal = retVal && m_values.Serialize(archive);
	retVal = retVal && archive.EndTag(valuesTag);

	const INumericConstraints* constraintsPtr = GetNumericConstraints();
	if (constraintsPtr != NULL){
		m_values.SetElementsCount(constraintsPtr->GetNumericValuesCount());
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CSimpleNumericValue::CopyFrom(const IChangeable& object)
{
	const INumericValue* sourcePtr = dynamic_cast<const INumericValue*>(&object);
	if (sourcePtr != NULL){
		imath::CVarVector values = sourcePtr->GetValues();

		const INumericConstraints* constraintsPtr = GetNumericConstraints();
		if (constraintsPtr != NULL){
			values.SetElementsCount(constraintsPtr->GetNumericValuesCount(), 0);
		}

		if (values != m_values){
			istd::CChangeNotifier notifier(this);

			m_values = sourcePtr->GetValues();
		}

		return true;
	}

	return false;
}


} // namespace imeas


