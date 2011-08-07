#include "ifpf/CHotfolderProcessingInfo.h"


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

CHotfolderProcessingInfo::CHotfolderProcessingInfo()
	:m_isWorking(false)
{
	m_lockPtr = istd::CreateService<isys::ICriticalSection>();

	I_ASSERT(m_lockPtr.IsValid());
}


bool CHotfolderProcessingInfo::ItemExists(const istd::CString& inputFilePath, ifpf::IHotfolderProcessingItem** foundItemPtr) const
{
	isys::CSectionBlocker lock(const_cast<isys::ICriticalSection*>(m_lockPtr.GetPtr()));

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

const ifpf::IHotfolderProcessingItem* CHotfolderProcessingInfo::AddProcessingItem(const istd::CString& inputFilePath, const istd::CString& outputFilePath)
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

	m_lockPtr->Enter();

	m_processingItems.PushBack(itemPtr);

	m_lockPtr->Leave();

	return itemPtr;
}


void CHotfolderProcessingInfo::RemoveProcessingItem(ifpf::IHotfolderProcessingItem* fileItemPtr)
{
	isys::CSectionBlocker lock(const_cast<isys::ICriticalSection*>(m_lockPtr.GetPtr()));

	// if file is in processing, abort
	if (fileItemPtr->GetProcessingState() == iproc::IProcessor::TS_WAIT){
		return;
	}

	istd::CChangeNotifier changePtr(this, CF_FILE_REMOVED, fileItemPtr);

	if (!m_processingItems.Remove(fileItemPtr)){
		changePtr.Abort();
	}

	lock.Reset();
}


int CHotfolderProcessingInfo::GetProcessingItemsCount() const
{
	isys::CSectionBlocker lock(const_cast<isys::ICriticalSection*>(m_lockPtr.GetPtr()));

	return m_processingItems.GetCount();
}


ifpf::IHotfolderProcessingItem* CHotfolderProcessingInfo::GetProcessingItem(int processingItemIndex) const
{
	isys::CSectionBlocker lock(const_cast<isys::ICriticalSection*>(m_lockPtr.GetPtr()));

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
	isys::CSectionBlocker lock(const_cast<isys::ICriticalSection*>(m_lockPtr.GetPtr()));

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
		lock.Reset();

		changePtr.SetPtr(this);
	}

	return retVal;
}


} // namespace ifpf


