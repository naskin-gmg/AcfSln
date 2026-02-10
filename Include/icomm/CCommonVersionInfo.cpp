// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomm/CCommonVersionInfo.h>


namespace icomm
{


CCommonVersionInfo::CCommonVersionInfo()
:	m_masterVersionInfoPtr(NULL)
{
}


void CCommonVersionInfo::SetMasterVersionInfo(const iser::IVersionInfo* versionInfoPtr)
{
	m_masterVersionInfoPtr = versionInfoPtr;

	CCommonVersionInfo::Reset();
}


void CCommonVersionInfo::Reset()
{
	m_versionIdList.clear();

	if (m_masterVersionInfoPtr != NULL){
		CCommonVersionInfo::MergeVersions(*m_masterVersionInfoPtr);
	}
}


void CCommonVersionInfo::MergeVersions(const iser::IVersionInfo& versionInfo)
{
	iser::IVersionInfo::VersionIds ids = versionInfo.GetVersionIds();
	for (		iser::IVersionInfo::VersionIds::ConstIterator iter = ids.begin();
				iter != ids.end();
				++iter){
		int versionId = *iter;

		quint32 versionNumber;
		if (!versionInfo.GetVersionNumber(versionId, versionNumber)){
			continue;
		}

		VersionElements::Iterator findIter = m_versionIdList.find(versionId);
		if (findIter != m_versionIdList.end()){
			VersionIdElement& element = *findIter;
			element.versionNumber = qMin(element.versionNumber, versionNumber);
			if (element.description.isEmpty()){
				element.description = versionInfo.GetVersionIdDescription(versionId);
			}
		}
		else{
			VersionIdElement& element = m_versionIdList[versionId];
			element.versionNumber = versionNumber;
			element.description = versionInfo.GetVersionIdDescription(versionId);
		}
	}
}


// reimplemented (iser::IVersionInfo)

CCommonVersionInfo::VersionIds CCommonVersionInfo::GetVersionIds() const
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	QList<int> keys = m_versionIdList.keys();

	return QSet<int>(keys.begin(), keys.end());
#else
	return m_versionIdList.keys().toSet();
#endif
}


bool CCommonVersionInfo::GetVersionNumber(int versionId, quint32& result) const
{
	VersionElements::ConstIterator findIter = m_versionIdList.constFind(versionId);
	if (findIter != m_versionIdList.constEnd()){
		result = findIter.value().versionNumber;

		return true;
	}

	result = 0xffffffff;

	return false;
}


QString CCommonVersionInfo::GetVersionIdDescription(int versionId) const
{
	VersionElements::ConstIterator findIter = m_versionIdList.constFind(versionId);
	if (findIter != m_versionIdList.constEnd()){
		return findIter.value().description;
	}

	return QString();
}


QString CCommonVersionInfo::GetEncodedVersionName(int versionId, quint32 versionNumber) const
{
	if (m_masterVersionInfoPtr != NULL){
		return m_masterVersionInfoPtr->GetEncodedVersionName(versionId, versionNumber);
	}

	return QString();
}


} // namespace icomm


