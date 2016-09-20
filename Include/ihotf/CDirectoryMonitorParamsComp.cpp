#include <ihotf/CDirectoryMonitorParamsComp.h>


namespace ihotf
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
		QStringList acceptPatterns;
		for (int index = 0; index < m_acceptPatternsAttrPtr.GetCount(); index++){
			acceptPatterns.push_back(m_acceptPatternsAttrPtr[index]);
		}

		SetAcceptPatterns(acceptPatterns);
	}

	if (m_ignorePatternsAttrPtr.IsValid()){
		QStringList ignorePatterns;
		for (int index = 0; index < m_ignorePatternsAttrPtr.GetCount(); index++){
			ignorePatterns.push_back(m_ignorePatternsAttrPtr[index]);
		}

		SetIgnorePatterns(ignorePatterns);
	}

	if (m_minLastModificationTimeDifferenceAttrPtr.IsValid()){
		SetMinLastModificationTimeDifference(*m_minLastModificationTimeDifferenceAttrPtr);
	}

	if (m_folderDepthAttrPtr.IsValid()){
		SetFolderDepth(*m_folderDepthAttrPtr);
	}
}


} // namespace ihotf


