#include "ifpf/CHotfolderProcessingInfo.h"


// Qt includes
#include <QtCore/QMutexLocker>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iproc/IProcessor.h"


namespace ifpf
{


// public methods

CHotfolderProcessingInfo::CHotfolderProcessingInfo()
:	m_isWorking(false)
{
}


bool CHotfolderProcessingInfo::ItemExists(const QString& inputFilePath, ifpf::IHotfolderProcessingItem** foundItemPtr) const
{
	QMutexLocker locker(&m_mutex);

	for(int itemIndex = 0; itemIndex < m_processingItems.GetCount(); itemIndex++){
		ifpf::IHotfolderProcessingItem* itemPtr = m_processingItems.GetAt(itemIndex);
		I_ASSERT(itemPtr != NULL);

		if (itemPtr->GetInputFile() == inputFilePath){
			if (foundItemPtr != NULL){
				*foundItemPtr = itemPtr;
			}

			return true;
		}
	}

	return false;
}


bool CHotfolderProcessingInfo::ItemExists(const ifpf::IHotfolderProcessingItem& processingItem) const
{
	return ItemExists(processingItem.GetInputFile());
}


// reimplemented (ifpf::IHotfolderProcessingInfo)

const ifpf::IHotfolderProcessingItem* CHotfolderProcessingInfo::AddProcessingItem(const QString& inputFilePath, const QString& outputFilePath)
{
	ifpf::IHotfolderProcessingItem* foundItemPtr = NULL;
	if (ItemExists(inputFilePath, &foundItemPtr)){
		return foundItemPtr;
	}

	ProcessingItem* itemPtr = new ProcessingItem;

	istd::CChangeNotifier changePtr(this, CF_FILE_ADDED, itemPtr);

	itemPtr->SetInputFile(inputFilePath);
	itemPtr->SetOutputFile(outputFilePath);
	itemPtr->SetProcessingState(iproc::IProcessor::TS_NONE);
	itemPtr->SetSlavePtr(this);

	QMutexLocker locker(&m_mutex);

	m_processingItems.PushBack(itemPtr);

	return itemPtr;
}


void CHotfolderProcessingInfo::RemoveProcessingItem(ifpf::IHotfolderProcessingItem* fileItemPtr)
{
	QMutexLocker locker(&m_mutex);

	// if file is in processing, abort
	if (fileItemPtr->GetProcessingState() == iproc::IProcessor::TS_WAIT){
		return;
	}

	istd::CChangeNotifier changePtr(this, CF_FILE_REMOVED, fileItemPtr);

	if (!m_processingItems.Remove(fileItemPtr)){
		changePtr.Abort();
	}
}


int CHotfolderProcessingInfo::GetProcessingItemsCount() const
{
	QMutexLocker locker(&m_mutex);

	return m_processingItems.GetCount();
}


ifpf::IHotfolderProcessingItem* CHotfolderProcessingInfo::GetProcessingItem(int processingItemIndex) const
{
	QMutexLocker locker(&m_mutex);

	return m_processingItems.GetAt(processingItemIndex);
}


bool CHotfolderProcessingInfo::IsWorking() const
{
	return m_isWorking;
}


void CHotfolderProcessingInfo::SetWorking(bool working)
{
	if (working != m_isWorking){
		istd::CChangeNotifier changePtr(this, CF_WORKING_STATE_CHANGED);
	
		m_isWorking = working;
	}
}


// reimplemented (iser::ISerializable)

bool CHotfolderProcessingInfo::Serialize(iser::IArchive& archive)
{
	QMutexLocker locker(&m_mutex);

	bool retVal = true;

	// before restoring stop the hotfolder:
	if (!archive.IsStoring() && m_isWorking){
		SetWorking(false);
	}

	istd::CChangeNotifier changePtr(NULL, CF_CREATE);

	static iser::CArchiveTag isWorkingTag("Working", "Hotfolder is in running state");
	retVal = retVal && archive.BeginTag(isWorkingTag);
	retVal = retVal && archive.Process(m_isWorking);
	retVal = retVal && archive.EndTag(isWorkingTag);
	
	static iser::CArchiveTag processingItemsTag("ProcessingItems", "List of processing items");
	static iser::CArchiveTag processingItemTag("ProcessingItem", "Single processing item");

	int processingItemsCount = m_processingItems.GetCount();
	retVal = retVal && archive.BeginMultiTag(processingItemsTag, processingItemTag, processingItemsCount);
	if (archive.IsStoring()){
		for (int itemIndex = 0; itemIndex < processingItemsCount; itemIndex++){
			ifpf::IHotfolderProcessingItem* itemPtr = m_processingItems.GetAt(itemIndex);

			retVal = retVal && archive.BeginTag(processingItemTag);
			retVal = retVal && itemPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(processingItemTag);
		}
	}
	else{
		m_processingItems.Reset();

		for (int itemIndex = 0; itemIndex < processingItemsCount; itemIndex++){
			istd::TDelPtr<ProcessingItem> itemPtr(new ProcessingItem);
			retVal = retVal && archive.BeginTag(processingItemTag);
			retVal = retVal && itemPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(processingItemTag);

			if (retVal){
				itemPtr->SetSlavePtr(this);

				m_processingItems.PushBack(itemPtr.PopPtr());
			}
		}	
	}

	retVal = retVal && archive.EndTag(processingItemsTag);

	if (retVal && !archive.IsStoring()){
		locker.unlock();

		changePtr.SetPtr(this);
	}

	return retVal;
}


} // namespace ifpf


