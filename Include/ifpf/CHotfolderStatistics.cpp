#include "ifpf/CHotfolderStatistics.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchivetag.h"

#include "iproc/IProcessor.h"


namespace ifpf
{


// public methods
	
CHotfolderStatistics::CHotfolderStatistics()
	:m_waitingCount(0),
	m_errorsCount(0),
	m_abortedCount(0),
	m_processedCount(0)
{
}


// reimplemented (ifpf::IHotfolderStatistics)

int CHotfolderStatistics::GetWaitingCount(const istd::CString& directoryPath) const
{
	return m_waitingCount;
}


int CHotfolderStatistics::GetProcessedCount(const istd::CString& directoryPath) const
{
	return m_processedCount;

}


int CHotfolderStatistics::GetErrorsCount(const istd::CString& directoryPath) const
{
	return m_errorsCount;
}


int CHotfolderStatistics::GetAbortedCount(const istd::CString& directoryPath) const
{
	return m_abortedCount;
}


// reimplemented (imod::TSingleModelObserverBase)

void CHotfolderStatistics::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	ResetStatistics();

	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		istd::CChangeNotifier changePtr(this);
		for (int itemIndex = 0; itemIndex < objectPtr->GetProcessingItemsCount(); itemIndex++){
			ifpf::IHotfolderProcessingItem* itemPtr = objectPtr->GetProcessingItem(itemIndex);
			switch (itemPtr->GetProcessingState()){
				case iproc::IProcessor::TS_NONE:
					++m_waitingCount;
					break;
				case iproc::IProcessor::TS_INVALID:
					++m_errorsCount;
					break;
				case iproc::IProcessor::TS_CANCELED:
					++m_abortedCount;
					break;

			}
		}

		m_processedCount = objectPtr->GetProcessingItemsCount() - m_abortedCount - m_waitingCount;
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
	m_waitingCount = 0;
	m_processedCount = 0;
	m_errorsCount = 0;
	m_abortedCount = 0;
}


} // namespace ifpf


