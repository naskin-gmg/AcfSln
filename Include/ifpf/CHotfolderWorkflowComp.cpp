#include "ifpf/CHotfolderWorkflowComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/CSectionBlocker.h"

#include "iser/IArchive.h"
#include "iser/CArchivetag.h"

#include "iproc/IProcessor.h"


namespace ifpf
{


// public methods

CHotfolderWorkflowComp::CHotfolderWorkflowComp()
{
}


// reimplemented (ifpf::IHotfolderWorkflow)

iprm::IParamsSet* CHotfolderWorkflowComp::AddHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId)
{
	iprm::IParamsSet* hotfolderInfoPtr = CreateHotfolder(hotfolderName, hotfolderId);
	if (hotfolderInfoPtr != NULL){
		istd::CChangeNotifier changePtr(this, CF_HOTFOLDER_ADDED);

		HotfolderInfo newHotfolder;

		newHotfolder.hotfolderId = hotfolderId;
		newHotfolder.hotfolderPtr.SetPtr(hotfolderInfoPtr);

		m_hotfolders.push_back(newHotfolder);

		return hotfolderInfoPtr;
	}

	return NULL;
}


bool CHotfolderWorkflowComp::RemoveHotfolder(const istd::CString& hotfolderName)
{
	return true;
}


ifpf::IHotfolderProcessingInfo* CHotfolderWorkflowComp::GetHotfolderProcessingInfo(const istd::CString& hotfolderName) const
{
	return NULL;
}


istd::CStringList CHotfolderWorkflowComp::GetHotfolderList() const
{
	return istd::CStringList();
}


int CHotfolderWorkflowComp::GetWorkingState(const istd::CString& hotfolderName) const
{
	return 0;
}


istd::CStringList CHotfolderWorkflowComp::GetHotfolderIds() const
{
	istd::CStringList hotfolderIds;

	if (m_hotoflderFactoryIdsAttrPtr.IsValid()){
		for (int index = 0; index < m_hotoflderFactoryIdsAttrPtr.GetCount(); index++){
			hotfolderIds.push_back(m_hotoflderFactoryIdsAttrPtr[index]);
		}
	}

	return hotfolderIds;
}


// reimplemented (iser::ISerializable)

bool CHotfolderWorkflowComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	return retVal;
}


// protected methods
	
iprm::IParamsSet* CHotfolderWorkflowComp::CreateHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId) const
{
	if (!m_hotoflderFactoriesCompPtr.IsValid()){
		return NULL;
	}

	if (!m_hotoflderFactoryIdsAttrPtr.IsValid()){
		return NULL;
	}

	for (int factoryIndex = 0; factoryIndex < m_hotoflderFactoryIdsAttrPtr.GetCount(); factoryIndex++){
		if (hotfolderId == m_hotoflderFactoryIdsAttrPtr[factoryIndex]){
			if (factoryIndex < m_hotoflderFactoriesCompPtr.GetCount()){
				return m_hotoflderFactoriesCompPtr.CreateInstance(factoryIndex);
			}
		}
	}

	return NULL;
}


} // namespace ifpf


