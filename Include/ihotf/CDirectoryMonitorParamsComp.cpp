// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ihotf/CDirectoryMonitorParamsComp.h>


namespace ihotf
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CDirectoryMonitorParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_pollingIntervalAttrPtr.IsValid()){
		SetPollingInterval(*m_pollingIntervalAttrPtr);
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


