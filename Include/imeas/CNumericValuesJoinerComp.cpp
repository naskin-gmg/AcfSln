#include <imeas/CNumericValuesJoinerComp.h>

#include <imeas/CSimpleNumericValue.h>


namespace imeas
{


// reimplemented (imeas::INumericValueProvider)

int CNumericValuesJoinerComp::GetValuesCount() const
{
	if (m_valueProvidersCompPtr.IsValid()){
		return m_valueProvidersCompPtr.GetCount();
	}

	return 0;
}


const imeas::INumericValue& CNumericValuesJoinerComp::GetNumericValue(int index) const
{
	if (m_valueProvidersCompPtr.IsValid()){
		int providerCount = m_valueProvidersCompPtr.GetCount();
		if (providerCount > 0 && index >= 0){
			int providerIndex = index % providerCount;
			int valueIndex = index / providerCount;
			if (m_valueProvidersCompPtr[providerIndex]->GetValuesCount() > valueIndex){
				return m_valueProvidersCompPtr[providerIndex]->GetNumericValue(valueIndex);
			}
		}
	}

	return *this;
}


// protected methods

// reimplemented (imeas::CSimpleNumericValue)

bool CNumericValuesJoinerComp::IsValueTypeSupported(imeas::INumericValue::ValueTypeId /*valueTypeId*/) const
{
	return false;
}


imath::CVarVector CNumericValuesJoinerComp::GetComponentValue(imeas::INumericValue::ValueTypeId /*valueTypeId*/) const
{
	return imath::CVarVector();
}


} // namespace imeas


