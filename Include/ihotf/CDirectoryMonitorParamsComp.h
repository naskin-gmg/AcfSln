// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef ihotf_CDirectoryMonitorParamsComp_included
#define ihotf_CDirectoryMonitorParamsComp_included


// ACF includes
#include <icomp/CComponentBase.h>


// AcfSln includes
#include <ihotf/CDirectoryMonitorParams.h>


namespace ihotf
{


/**
	Implementation of directory parameters component.
	This implementation makes possible the definition of the default parameters at the component level.
*/
class CDirectoryMonitorParamsComp:
			public icomp::CComponentBase,
			public ihotf::CDirectoryMonitorParams
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef ihotf::CDirectoryMonitorParams BaseClass2;

	I_BEGIN_COMPONENT(ihotf::CDirectoryMonitorParamsComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ihotf::IDirectoryMonitorParams);
		I_ASSIGN(m_pollingIntervalAttrPtr, "PollingInterval", "Interval for checking of directory changes in seconds", false, 5);
		I_ASSIGN(m_observedItemTypesAttrPtr, "ObservedItems", "Items to be observed.\n1 - Directories\n2 - Files\n4 - Drives", false, OI_FILES);
		I_ASSIGN(m_observedChangesAttrPtr, "ObservedChanges", "Item changes to be observed.\n1 - Add\n2 - Remove\n4 - Modified\n8 - Attributes changed", false, OC_ADD);
		I_ASSIGN(m_minLastModificationTimeDifferenceAttrPtr, "MinLastModificationTimeDifference", "Second since last modification time of the file before operating on it", false, 30);
		I_ASSIGN_MULTI_0(m_acceptPatternsAttrPtr, "AcceptPatterns", "File name patterns for accepting of the file items", false);
		I_ASSIGN_MULTI_0(m_ignorePatternsAttrPtr, "IgnorePatterns", "File name patterns for ignoring of the file items", false);
		I_ASSIGN(m_folderDepthAttrPtr, "FolderDepth", "Depth of the folder tree to be monitored.", false, -1);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_ATTR(double, m_pollingIntervalAttrPtr);
	I_ATTR(int, m_observedItemTypesAttrPtr);
	I_ATTR(int, m_observedChangesAttrPtr);
	I_ATTR(int, m_minLastModificationTimeDifferenceAttrPtr);
	I_MULTIATTR(QString, m_acceptPatternsAttrPtr);
	I_MULTIATTR(QString, m_ignorePatternsAttrPtr);
	I_ATTR(int, m_folderDepthAttrPtr);
};


} // namespace ihotf


#endif // !ihotf_CDirectoryMonitorParamsComp_included


