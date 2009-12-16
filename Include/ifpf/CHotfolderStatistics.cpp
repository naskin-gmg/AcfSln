#include "ifpf/CHotfolderStatistics.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"
#include "istd/CChangeDelegator.h"

#include "isys/IFileSystem.h"

#include "iser/IArchive.h"
#include "iser/CArchivetag.h"

#include "iproc/IProcessor.h"


namespace ifpf
{


// public methods
	
CHotfolderStatistics::CHotfolderStatistics()
{
}


// reimplemented (ifpf::IHotfolderStatistics)

int CHotfolderStatistics::GetItemsCount(const istd::CString& directoryPath) const
{
	CounterMap::const_iterator iter = m_itemsCount.find(directoryPath);
	if (iter != m_itemsCount.end()){
		return iter->second;
	}

	return 0;
}


int CHotfolderStatistics::GetProcessedCount(const istd::CString& directoryPath) const
{
	CounterMap::const_iterator iter = m_processedCount.find(directoryPath);
	if (iter != m_processedCount.end()){
		return iter->second;
	}

	return 0;
}


int CHotfolderStatistics::GetErrorsCount(const istd::CString& directoryPath) const
{
	CounterMap::const_iterator iter = m_errorsCount.find(directoryPath);
	if (iter != m_errorsCount.end()){
		return iter->second;
	}

	return 0;
}


int CHotfolderStatistics::GetAbortedCount(const istd::CString& directoryPath) const
{
	CounterMap::const_iterator iter = m_abortedCount.find(directoryPath);
	if (iter != m_abortedCount.end()){
		return iter->second;
	}

	return 0;
}


// reimplemented (imod::TSingleModelObserverBase)

void CHotfolderStatistics::BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	// save previous state of the current changing item:
	ifpf::IHotfolderProcessingItem* itemPtr = dynamic_cast<ifpf::IHotfolderProcessingItem*>(updateParamsPtr);
	if (itemPtr != NULL && (updateFlags & ifpf::IHotfolderProcessingItem::CF_STATE_CHANGED) != 0){
		m_previousItemState = std::make_pair(itemPtr, itemPtr->GetProcessingState());
	}
}


void CHotfolderStatistics::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	if ((updateFlags & ifpf::IHotfolder::CF_FILE_REMOVED) != 0 || (updateFlags & ifpf::IHotfolder::CF_CREATE) != 0){
		RebuildStatistics();
	}

	if ((updateFlags & ifpf::IHotfolder::CF_FILE_ADDED) != 0){
		istd::CChangeNotifier changePtr(this);

		int lastItemIndex = objectPtr->GetProcessingItemsCount() - 1;
		I_ASSERT(lastItemIndex >= 0);

		ifpf::IHotfolderProcessingItem* itemPtr = objectPtr->GetProcessingItem(lastItemIndex);
		I_ASSERT(itemPtr != NULL);
				
		int itemState = itemPtr->GetProcessingState();
		istd::CString directoryPath = GetDirectoryPath(*itemPtr);

		++m_itemsCount[directoryPath];
		UpdateStateMaps(itemState, directoryPath);
	}

	// increment new item state and decrement the old one:
	ifpf::IHotfolderProcessingItem* itemPtr = dynamic_cast<ifpf::IHotfolderProcessingItem*>(updateParamsPtr);
	if (itemPtr != NULL && (updateFlags & ifpf::IHotfolderProcessingItem::CF_STATE_CHANGED) != 0){
		istd::CChangeNotifier changePtr(this);

		int itemState = itemPtr->GetProcessingState();
		istd::CString directoryPath = GetDirectoryPath(*itemPtr);

		UpdateStateMaps(itemState, directoryPath);

		if (m_previousItemState.first == itemPtr){
			switch (m_previousItemState.second){
				case iproc::IProcessor::TS_OK:
					--m_processedCount[directoryPath];
					break;
				case iproc::IProcessor::TS_INVALID:
					--m_errorsCount[directoryPath];
					--m_processedCount[directoryPath];
					break;
				case iproc::IProcessor::TS_CANCELED:
					--m_abortedCount[directoryPath];
					break;
			}
		}

		m_previousItemState = std::make_pair((ifpf::IHotfolderProcessingItem*)NULL, -1);
	}
}


// reimplemented (imod::IObserver)

bool CHotfolderStatistics::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass::OnAttached(modelPtr)){
		RebuildStatistics();

		return true;
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CHotfolderStatistics::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	return retVal;
}


// protected methods

void CHotfolderStatistics::ResetStatistics()
{
	m_itemsCount.clear();
	m_processedCount.clear();
	m_errorsCount.clear();
	m_abortedCount.clear();
}


void CHotfolderStatistics::RebuildStatistics()
{
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	ResetStatistics();

	istd::CChangeNotifier changePtr(this);
	int itemsCount = objectPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ifpf::IHotfolderProcessingItem* itemPtr = objectPtr->GetProcessingItem(itemIndex);
		I_ASSERT(itemPtr != NULL);

		int itemState = itemPtr->GetProcessingState();
		istd::CString directoryPath = GetDirectoryPath(*itemPtr);

		++m_itemsCount[directoryPath];
		UpdateStateMaps(itemState, directoryPath);
	}
}


void CHotfolderStatistics::UpdateStateMaps(int itemState, const istd::CString& directoryPath)
{
	switch (itemState){
		case iproc::IProcessor::TS_OK:
			++m_processedCount[directoryPath];
			break;
		case iproc::IProcessor::TS_INVALID:
			++m_errorsCount[directoryPath];
			++m_processedCount[directoryPath];
			break;
		case iproc::IProcessor::TS_CANCELED:
			++m_abortedCount[directoryPath];
			break;
	}
}



// protected static methods

istd::CString CHotfolderStatistics::GetDirectoryPath(const ifpf::IHotfolderProcessingItem& item)
{
	istd::CString filePath = item.GetInputFile();

	isys::IFileSystem* systemPtr = istd::GetService<isys::IFileSystem>();
	if (systemPtr != NULL){
		return systemPtr->GetDirPath(filePath);
	}

	return istd::CString();
}


} // namespace ifpf


