#include "ifpf/CHotfolderWorkflowComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/CSectionBlocker.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iproc/IProcessor.h"


namespace ifpf
{


// public methods

CHotfolderWorkflowComp::CHotfolderWorkflowComp()
{
}


// reimplemented (ifpf::IHotfolderInfoManager)

ifpf::IHotfolderProcessingInfo* CHotfolderWorkflowComp::GetProcessingInfo(const istd::CString& hotfolderName) const
{
	return NULL;
}


// reimplemented (ifpf::IHotfolderWorkflow)

ifpf::IHotfolderWorkflowItem* CHotfolderWorkflowComp::AddHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId)
{
	HotfolderItem* newHotfolderInfoPtr = CreateHotfolder(hotfolderName, hotfolderId);
	if (newHotfolderInfoPtr != NULL){
		istd::CChangeNotifier changePtr(this, CF_HOTFOLDER_ADDED);

		m_hotfolders.PushBack(newHotfolderInfoPtr);

		return newHotfolderInfoPtr->elementPtr.GetPtr();
	}

	return NULL;
}


bool CHotfolderWorkflowComp::RemoveHotfolder(const istd::CString& hotfolderName)
{
	return true;
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


istd::CStringList CHotfolderWorkflowComp::GetHotfolderList() const
{
	istd::CStringList hotfolderList;

	for (int index = 0; index < m_hotfolders.GetCount(); index++){
		hotfolderList.push_back(m_hotfolders.GetAt(index)->elementPtr->GetName());
	}

	return hotfolderList;
}


ifpf::IHotfolderWorkflowItem* CHotfolderWorkflowComp::GetHotfolder(const istd::CString& hotfolderName) const
{
	for (int index = 0; index < m_hotfolders.GetCount(); index++){
		HotfolderItem* infoPtr = m_hotfolders.GetAt(index);
		I_ASSERT(infoPtr != NULL);
		I_ASSERT(infoPtr->elementPtr != NULL);

		if (infoPtr->elementPtr->GetName() == hotfolderName){
			return infoPtr->elementPtr.GetPtr();
		}
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CHotfolderWorkflowComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	return retVal;
}


// protected methods
	
CHotfolderWorkflowComp::HotfolderItem* CHotfolderWorkflowComp::CreateHotfolder(const istd::CString& hotfolderName, const istd::CString& hotfolderId) const
{
	if (!m_hotoflderFactoriesCompPtr.IsValid()){
		return NULL;
	}

	if (!m_hotoflderFactoryIdsAttrPtr.IsValid()){
		return NULL;
	}

	istd::TDelPtr<HotfolderItem> newHotfolderItemPtr(new HotfolderItem());

	for (int factoryIndex = 0; factoryIndex < m_hotoflderFactoryIdsAttrPtr.GetCount(); factoryIndex++){
		if (hotfolderId == m_hotoflderFactoryIdsAttrPtr[factoryIndex]){
			if (factoryIndex < m_hotoflderFactoriesCompPtr.GetCount()){
				iprm::IParamsSet* hotfolderParamsPtr = m_hotoflderFactoriesCompPtr.CreateInstance(factoryIndex);
				if (hotfolderParamsPtr != NULL){
					newHotfolderItemPtr->hotfolderParamsPtr.SetPtr(hotfolderParamsPtr);

					newHotfolderItemPtr->elementPtr.SetPtr(CreateWorkflowItem(hotfolderId));

					newHotfolderItemPtr->elementPtr->SetName(hotfolderName);

					return newHotfolderItemPtr.PopPtr();
				}
			}
		}
	}

	return NULL;
}


ifpf::IHotfolderWorkflowItem* CHotfolderWorkflowComp::CreateWorkflowItem(const istd::CString& hotfolderId) const
{
	ifpf::CHotfolderWorkflowItem* itemPtr = new ifpf::CHotfolderWorkflowItem();

	itemPtr->Initialize(hotfolderId, this);

	return itemPtr;
}


} // namespace ifpf


