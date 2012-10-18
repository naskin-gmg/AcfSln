#include "imeas/CNumericValuesJoinerComp.h"

#include "imeas/CSimpleNumericValue.h"


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
	static imeas::CSimpleNumericValue dummy;

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

	return dummy;
}


} // namespace imeas


