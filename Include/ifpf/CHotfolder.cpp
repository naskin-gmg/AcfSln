#include "ifpf/CHotfolder.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchivetag.h"

#include "iproc/IProcessor.h"


namespace ifpf
{


// public methods

CHotfolder::CHotfolder()
	:m_isWorking(false),
	m_paramsSetPtr(NULL)
{
}


void CHotfolder::RemoveProcessingItem(ProcessingItem* fileItemPtr)
{
	if (m_processingItems.Remove(fileItemPtr)){
		istd::CChangeNotifier changePtr(this, CF_FILE_REMOVED);
	}
}


void CHotfolder::AddProcessingItem(ProcessingItem* fileItemPtr, bool releaseFlag)
{
	istd::CChangeNotifier changePtr(this, CF_FILE_ADDED);

	m_processingItems.PushBack(fileItemPtr, releaseFlag);
}


void CHotfolder::SetParams(iprm::IParamsSet* paramsSet)
{
	// Set params set only by initialization:
	I_ASSERT(m_paramsSetPtr == NULL);
	// Parameter validation:
	I_ASSERT(paramsSet != NULL);

	m_paramsSetPtr = paramsSet;
}


const ifpf::IHotfolderProcessingItem* CHotfolder::GetNextProcessingFile() const
{
	int itemsCount = m_processingItems.GetCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = m_processingItems.GetAt(itemIndex);

		if (processingItemPtr->GetProcessingState() == iproc::IProcessor::TS_NONE){
			return processingItemPtr;
		}
	}

	return NULL;
}


void CHotfolder::UpdateProcessingState(const ifpf::IHotfolderProcessingItem* processingItemPtr, int processingState)
{
	int itemsCount = m_processingItems.GetCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ifpf::CHotfolderProcessingItem* itemPtr = m_processingItems.GetAt(itemIndex);

		if (itemPtr == processingItemPtr){
			itemPtr->SetProcessingState(processingState);

			break;
		}
	}
}


// reimplemented (ifpf::IHotfolder)

int CHotfolder::GetProcessingItemsCount() const
{
	return m_processingItems.GetCount();
}


IHotfolderProcessingItem* CHotfolder::GetProcessingItem(int processingItemIndex) const
{
	return m_processingItems.GetAt(processingItemIndex);
}


bool CHotfolder::IsWorking() const
{
	return m_isWorking;
}


void CHotfolder::SetWorking(bool working)
{
	if (working != m_isWorking){
		istd::CChangeNotifier changePtr(this, CF_WORKING_STATE_CHANGED);
	
		m_isWorking = working;
	}
}


iprm::IParamsSet* CHotfolder::GetHotfolderParams() const
{
	return m_paramsSetPtr;
}


// reimplemented (iser::ISerializable)

bool CHotfolder::Serialize(iser::IArchive& archive)
{
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
			ifpf::CHotfolderProcessingItem* itemPtr = m_processingItems.GetAt(itemIndex);

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
				m_processingItems.PushBack(itemPtr.PopPtr());
			}
		}	
	}

	retVal = retVal && archive.EndTag(processingItemsTag);

	if (retVal && !archive.IsStoring()){
		changePtr.SetPtr(this);
	}

	return retVal;
}


} // namespace ifpf

