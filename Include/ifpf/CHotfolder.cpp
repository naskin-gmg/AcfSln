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


void CHotfolder::RemoveFile(ifpf::IHotfolderProcessingItem* fileItemPtr)
{
	if (m_fileItems.Remove(fileItemPtr)){
		istd::CChangeNotifier changePtr(this, CF_FILE_REMOVED);
	}
}


void CHotfolder::AddFile(ifpf::IHotfolderProcessingItem* fileItemPtr, bool releaseFlag)
{
	istd::CChangeNotifier changePtr(this, CF_FILE_ADDED);

	m_fileItems.PushBack(fileItemPtr, releaseFlag);
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
	int itemsCount = m_fileItems.GetCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = m_fileItems.GetAt(itemIndex);

		if (processingItemPtr->GetProcessingState() == iproc::IProcessor::TS_NONE){
			return processingItemPtr;
		}
	}

	return NULL;
}


// reimplemented (ifpf::IHotfolder)

int CHotfolder::GetProcessingItemsCount() const
{
	return m_fileItems.GetCount();
}


IHotfolderProcessingItem* CHotfolder::GetProcessingItem(int processingItemIndex) const
{
	return m_fileItems.GetAt(processingItemIndex);
}


bool CHotfolder::IsWorking() const
{
	return m_isWorking;
}

void CHotfolder::SetWorking(bool working)
{
	if (working != m_isWorking){
		istd::CChangeNotifier changePtr(this);
	
		m_isWorking = m_isWorking;
	}
}


iprm::IParamsSet* CHotfolder::GetHotfolderParams() const
{
	return m_paramsSetPtr;
}


// reimplemented (iser::ISerializable)

bool CHotfolder::Serialize(iser::IArchive& archive)
{
	return true;
}


} // namespace ifpf

