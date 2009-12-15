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

void CHotfolderStatistics::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	if ((updateFlags & ifpf::IHotfolder::CF_FILE_REMOVED) != 0 || (updateFlags & ifpf::IHotfolder::CF_CREATE) != 0 || (updateFlags & istd::CChangeDelegator::CF_DELEGATED) != 0){
		ResetStatistics();

		istd::CChangeNotifier changePtr(this);
		for (int itemIndex = 0; itemIndex < objectPtr->GetProcessingItemsCount(); itemIndex++){
			ifpf::IHotfolderProcessingItem* itemPtr = objectPtr->GetProcessingItem(itemIndex);
			I_ASSERT(itemPtr != NULL);

			istd::CString directoryPath = GetDirectoryPath(*itemPtr);

			++m_itemsCount[directoryPath];

			switch (itemPtr->GetProcessingState()){
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
	}

	if ((updateFlags & ifpf::IHotfolder::CF_FILE_ADDED) != 0){
		int lastItemIndex = objectPtr->GetProcessingItemsCount() - 1;
		I_ASSERT(lastItemIndex >= 0);

		ifpf::IHotfolderProcessingItem* itemPtr = objectPtr->GetProcessingItem(lastItemIndex);
			I_ASSERT(itemPtr != NULL);
				
			istd::CString directoryPath = GetDirectoryPath(*itemPtr);

			++m_itemsCount[directoryPath];

			switch (itemPtr->GetProcessingState()){
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


