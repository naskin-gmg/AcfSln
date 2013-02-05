#include "ihotf/CHotfolderWorkflowComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QMutexLocker>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iproc/IProcessor.h"


namespace ihotf
{


// public methods

CHotfolderWorkflowComp::CHotfolderWorkflowComp()
{
}


// reimplemented (ihotf::IHotfolderInfoManager)

ihotf::IHotfolderProcessingInfo* CHotfolderWorkflowComp::GetProcessingInfo(const QString& /*hotfolderName*/) const
{
	return NULL;
}


// reimplemented (ihotf::IHotfolderWorkflow)

ihotf::IHotfolderWorkflowItem* CHotfolderWorkflowComp::AddHotfolder(const QString& hotfolderName, const QString& hotfolderId)
{
	HotfolderItem* newHotfolderInfoPtr = CreateHotfolder(hotfolderName, hotfolderId);
	if (newHotfolderInfoPtr != NULL){
		istd::CChangeNotifier changePtr(this, CF_MODEL | CF_HOTFOLDER_ADDED);

		m_hotfolders.PushBack(newHotfolderInfoPtr);

		return newHotfolderInfoPtr->elementPtr.GetPtr();
	}

	return NULL;
}


bool CHotfolderWorkflowComp::RemoveHotfolder(const QString& /*hotfolderName*/)
{
	return true;
}


QStringList CHotfolderWorkflowComp::GetHotfolderIds() const
{
	QStringList hotfolderIds;

	if (m_hotoflderFactoryIdsAttrPtr.IsValid()){
		for (int index = 0; index < m_hotoflderFactoryIdsAttrPtr.GetCount(); index++){
			hotfolderIds.push_back(m_hotoflderFactoryIdsAttrPtr[index]);
		}
	}

	return hotfolderIds;
}


QStringList CHotfolderWorkflowComp::GetHotfolderList() const
{
	QStringList hotfolderList;

	for (int index = 0; index < m_hotfolders.GetCount(); index++){
		hotfolderList.push_back(m_hotfolders.GetAt(index)->elementPtr->GetName());
	}

	return hotfolderList;
}


ihotf::IHotfolderWorkflowItem* CHotfolderWorkflowComp::GetHotfolder(const QString& hotfolderName) const
{
	for (int index = 0; index < m_hotfolders.GetCount(); index++){
		HotfolderItem* infoPtr = m_hotfolders.GetAt(index);
		Q_ASSERT(infoPtr != NULL);
		Q_ASSERT(infoPtr->elementPtr != NULL);

		if (infoPtr->elementPtr->GetName() == hotfolderName){
			return infoPtr->elementPtr.GetPtr();
		}
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CHotfolderWorkflowComp::Serialize(iser::IArchive& /*archive*/)
{
	bool retVal = true;

	return retVal;
}


// protected methods
	
CHotfolderWorkflowComp::HotfolderItem* CHotfolderWorkflowComp::CreateHotfolder(const QString& hotfolderName, const QString& hotfolderId) const
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


ihotf::IHotfolderWorkflowItem* CHotfolderWorkflowComp::CreateWorkflowItem(const QString& hotfolderId) const
{
	ihotf::CHotfolderWorkflowItem* itemPtr = new ihotf::CHotfolderWorkflowItem();

	itemPtr->Initialize(hotfolderId, this);

	return itemPtr;
}


} // namespace ihotf


