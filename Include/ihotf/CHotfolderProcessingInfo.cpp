#include "ihotf/CHotfolderProcessingInfo.h"


// Qt includes
#include <QtCore/QMutexLocker>

// ACF includes
#include "istd/CChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iproc/IProcessor.h"


namespace ihotf
{


const istd::IChangeable::ChangeSet s_addFileChangeSet(IHotfolderProcessingInfo::CF_FILE_ADDED);
const istd::IChangeable::ChangeSet s_removeItemChangeSet(IHotfolderProcessingInfo::CF_FILE_REMOVED);
const istd::IChangeable::ChangeSet s_itemStateChangeSet(IHotfolderProcessingInfo::CF_WORKING_STATE_CHANGED);



CHotfolderProcessingInfo::CHotfolderProcessingInfo()
:	m_isWorking(false)
{
}


bool CHotfolderProcessingInfo::ItemExists(const QString& inputFilePath, ihotf::IHotfolderProcessingItem** foundItemPtr) const
{
	QMutexLocker locker(&m_mutex);

	for (int itemIndex = 0; itemIndex < m_processingItems.GetCount(); itemIndex++){
		ihotf::IHotfolderProcessingItem* itemPtr = m_processingItems.GetAt(itemIndex);
		Q_ASSERT(itemPtr != NULL);

		if (itemPtr->GetInputFile() == inputFilePath){
			if (foundItemPtr != NULL){
				*foundItemPtr = itemPtr;
			}

			return true;
		}
	}

	return false;
}


bool CHotfolderProcessingInfo::ItemExists(const ihotf::IHotfolderProcessingItem& processingItem) const
{
	return ItemExists(processingItem.GetInputFile());
}


// reimplemented (ihotf::IHotfolderProcessingInfo)

const ihotf::IHotfolderProcessingItem* CHotfolderProcessingInfo::AddProcessingItem(const QString& inputFilePath, const QString& outputFilePath)
{
	ihotf::IHotfolderProcessingItem* foundItemPtr = NULL;
	if (ItemExists(inputFilePath, &foundItemPtr)){
		return foundItemPtr;
	}

	istd::CChangeNotifier notifier(this, &s_addFileChangeSet);
	Q_UNUSED(notifier);

	ProcessingItem* itemPtr = new ProcessingItem;
	itemPtr->SetInputFile(inputFilePath);
	itemPtr->SetOutputFile(outputFilePath);
	itemPtr->SetProcessingState(iproc::IProcessor::TS_NONE);
	itemPtr->SetSlavePtr(this);

	QMutexLocker locker(&m_mutex);

	m_processingItems.PushBack(itemPtr);

	return itemPtr;
}


void CHotfolderProcessingInfo::RemoveProcessingItem(ihotf::IHotfolderProcessingItem* fileItemPtr)
{
	QMutexLocker locker(&m_mutex);

	// if file is in processing, abort
	if (fileItemPtr->GetProcessingState() == iproc::IProcessor::TS_WAIT){
		return;
	}

	istd::CChangeNotifier notifier(this, &s_removeItemChangeSet);

	if (!m_processingItems.Remove(fileItemPtr)){
		notifier.Abort();
	}
}


int CHotfolderProcessingInfo::GetProcessingItemsCount() const
{
	QMutexLocker locker(&m_mutex);

	return m_processingItems.GetCount();
}


ihotf::IHotfolderProcessingItem* CHotfolderProcessingInfo::GetProcessingItem(int processingItemIndex) const
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
		istd::CChangeNotifier notifier(this, &s_itemStateChangeSet);
		Q_UNUSED(notifier);

		m_isWorking = working;
	}
}


// reimplemented (iser::ISerializable)

bool CHotfolderProcessingInfo::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	QMutexLocker locker(&m_mutex);

	bool retVal = true;

	// before restoring stop the hotfolder:
	if (!archive.IsStoring() && m_isWorking){
		SetWorking(false);
	}

	static iser::CArchiveTag isWorkingTag("Working", "Hotfolder is in running state", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag processingItemsTag("ProcessingItems", "List of processing items", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag processingItemTag("ProcessingItem", "Single processing item", iser::CArchiveTag::TT_GROUP, &processingItemsTag);

	retVal = retVal && archive.BeginTag(isWorkingTag);
	retVal = retVal && archive.Process(m_isWorking);
	retVal = retVal && archive.EndTag(isWorkingTag);

	int processingItemsCount = m_processingItems.GetCount();
	retVal = retVal && archive.BeginMultiTag(processingItemsTag, processingItemTag, processingItemsCount);
	if (archive.IsStoring()){
		for (int itemIndex = 0; itemIndex < processingItemsCount; itemIndex++){
			ihotf::IHotfolderProcessingItem* itemPtr = m_processingItems.GetAt(itemIndex);

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

	return retVal;
}


} // namespace ihotf


