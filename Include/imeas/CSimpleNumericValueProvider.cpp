#include "imeas/CSimpleNumericValueProvider.h"


// ACF includes
#include "istd/CChangeNotifier.h"


namespace imeas
{


// reimplemented (imeas::INumericValueProvider)

int CSimpleNumericValueProvider::GetValuesCount() const
{
	return m_values.count();
}


const imeas::INumericValue& CSimpleNumericValueProvider::GetNumericValue(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < m_values.count());

	return m_values[index];
}


// reimplemented (iser::ISerializable)

bool CSimpleNumericValueProvider::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring() ? NULL : this);

	static iser::CArchiveTag valuesTag("Values", "List of numeric values");
	static iser::CArchiveTag valueTag("Value", "Single value");

	int valuesCount = m_values.count();

	bool retVal = archive.BeginMultiTag(valuesTag, valueTag, valuesCount);

	if (!archive.IsStoring()){
		m_values.resize(valuesCount);
	}

	for (int valueIndex = 0; valueIndex < valuesCount; valueIndex++){
		retVal = retVal && m_values[valueIndex].Serialize(archive);
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CSimpleNumericValueProvider::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const imeas::INumericValueProvider* sourcePtr = dynamic_cast<const imeas::INumericValueProvider*>(&object);
	if (sourcePtr != NULL){
		istd::CChangeNotifier notifier(this);
		int valuesCount = sourcePtr->GetValuesCount();

		m_values.resize(valuesCount);

		for (int valueIndex = 0; valueIndex < valuesCount; valueIndex++){
			const imeas::INumericValue& sourceValue = sourcePtr->GetNumericValue(valueIndex);

			m_values[valueIndex].SetValues(sourceValue.GetValues());
		}

		return true;
	}

	return false;
}


} // namespace imeas


