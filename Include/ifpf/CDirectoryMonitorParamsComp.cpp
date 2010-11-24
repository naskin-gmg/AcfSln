#include "ifpf/CDirectoryMonitorParamsComp.h"


namespace ifpf
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CDirectoryMonitorParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_poolingIntervallAttrPtr.IsValid()){
		SetPoolingIntervall(*m_poolingIntervallAttrPtr);
	}

	if (m_observedItemTypesAttrPtr.IsValid()){
		SetObservedItemTypes(*m_observedItemTypesAttrPtr);
	}

	if (m_observedChangesAttrPtr.IsValid()){
		SetObservedChanges(*m_observedChangesAttrPtr);
	}

	if (m_acceptPatternsAttrPtr.IsValid()){
		istd::CStringList acceptPatterns;
		for (int index = 0; index < m_acceptPatternsAttrPtr.GetCount(); index++){
			acceptPatterns.push_back(m_acceptPatternsAttrPtr[index]);
		}

		SetAcceptPatterns(acceptPatterns);
	}

	if (m_ignorePatternsAttrPtr.IsValid()){
		istd::CStringList ignorePatterns;
		for (int index = 0; index < m_ignorePatternsAttrPtr.GetCount(); index++){
			ignorePatterns.push_back(m_ignorePatternsAttrPtr[index]);
		}

		SetIgnorePatterns(ignorePatterns);
	}
}


} // namespace ifpf


