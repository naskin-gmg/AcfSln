#include <imeas/CSimpleNumericValue.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>

// ACF-Solutions includes
#include <imeas/INumericConstraints.h>


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
	Q_ASSERT(valueTypeId == VTI_AUTO);

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
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_values = values;

		const INumericConstraints* constraintsPtr = GetNumericConstraints();
		if (constraintsPtr != NULL){
			m_values.SetElementsCount(constraintsPtr->GetNumericValuesCount(), 0);
		}
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool CSimpleNumericValue::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag valuesTag("Values", "List of numeric values", iser::CArchiveTag::TT_WEAK);

	bool retVal = true;

	if (archive.IsStoring()){
		retVal = retVal && archive.BeginTag(valuesTag);
		retVal = retVal && m_values.Serialize(archive);
		retVal = retVal && archive.EndTag(valuesTag);
	}
	else{
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		retVal = retVal && archive.BeginTag(valuesTag);
		retVal = retVal && m_values.Serialize(archive);
		retVal = retVal && archive.EndTag(valuesTag);

		const INumericConstraints* constraintsPtr = GetNumericConstraints();
		if (constraintsPtr != NULL){
			m_values.SetElementsCount(constraintsPtr->GetNumericValuesCount(), 0);
		}
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CSimpleNumericValue::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const INumericValue* sourcePtr = dynamic_cast<const INumericValue*>(&object);
	if (sourcePtr != NULL){
		imath::CVarVector values = sourcePtr->GetValues();

		const INumericConstraints* constraintsPtr = GetNumericConstraints();
		if (constraintsPtr != NULL){
			int expectedElemetsCount = constraintsPtr->GetNumericValuesCount();
			if ((mode == CM_STRICT) && (values.GetElementsCount() != expectedElemetsCount)){
				return false;
			}

			values.SetElementsCount(expectedElemetsCount, 0);
		}

		if (values != m_values){
			istd::CChangeNotifier notifier(this);
			Q_UNUSED(notifier);

			m_values = values;
		}

		return true;
	}

	return false;
}


} // namespace imeas


