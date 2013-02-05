#include "ihotf/CHotfolderStatistics.h"


// Qt includes
#include <QtCore/QFileInfo>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CChangeDelegator.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iproc/IProcessor.h"


namespace ihotf
{


// public methods
	
CHotfolderStatistics::CHotfolderStatistics()
{
}


// reimplemented (ihotf::IHotfolderStatistics)

int CHotfolderStatistics::GetItemsCount(const QString& directoryPath) const
{
	CounterMap::ConstIterator iter = m_itemsCount.constFind(directoryPath);
	if (iter != m_itemsCount.constEnd()){
		return iter.value();
	}

	return 0;
}


int CHotfolderStatistics::GetSuccessCount(const QString& directoryPath) const
{
	CounterMap::ConstIterator iter = m_successCount.constFind(directoryPath);
	if (iter != m_successCount.constEnd()){
		return iter.value();
	}

	return 0;
}


int CHotfolderStatistics::GetErrorsCount(const QString& directoryPath) const
{
	CounterMap::ConstIterator iter = m_errorsCount.constFind(directoryPath);
	if (iter != m_errorsCount.constEnd()){
		return iter.value();
	}

	return 0;
}


int CHotfolderStatistics::GetAbortedCount(const QString& directoryPath) const
{
	CounterMap::ConstIterator iter = m_abortedCount.constFind(directoryPath);
	if (iter != m_abortedCount.constEnd()){
		return iter.value();
	}

	return 0;
}


double CHotfolderStatistics::GetProcessingTime(const QString& directoryPath) const
{
	if (directoryPath.isEmpty()){
		double processingTime = 0;
		for (ProcessingTimeMap::const_iterator index = m_processingTimeMap.begin(); index != m_processingTimeMap.end(); index++){
			processingTime += index.value();
		}
		
		return processingTime;
	}

	ProcessingTimeMap::ConstIterator foundIter = m_processingTimeMap.constFind(directoryPath);
	if (foundIter != m_processingTimeMap.constEnd()){
		return foundIter.value();
	}

	return 0.0;
}


// reimplemented (imod::TSingleModelObserverBase)

void CHotfolderStatistics::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	RebuildStatistics();
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

bool CHotfolderStatistics::Serialize(iser::IArchive& /*archive*/)
{
	bool retVal = true;

	return retVal;
}


// protected methods

void CHotfolderStatistics::ResetStatistics()
{
	m_processingTimeMap.clear();
	m_itemsCount.clear();
	m_successCount.clear();
	m_errorsCount.clear();
	m_abortedCount.clear();
}


void CHotfolderStatistics::RebuildStatistics()
{
	ihotf::IHotfolderProcessingInfo* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	ResetStatistics();

	istd::CChangeNotifier changePtr(this);
	int itemsCount = objectPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ihotf::IHotfolderProcessingItem* itemPtr = objectPtr->GetProcessingItem(itemIndex);
		Q_ASSERT(itemPtr != NULL);

		int itemState = itemPtr->GetProcessingState();
		QString directoryPath = GetDirectoryPath(*itemPtr);

		++m_itemsCount[directoryPath];
		UpdateStateMaps(itemState, directoryPath);

		m_processingTimeMap[directoryPath] += itemPtr->GetProcessingTime();
	}
}


void CHotfolderStatistics::UpdateStateMaps(int itemState, const QString& directoryPath)
{
	switch (itemState){
		case iproc::IProcessor::TS_OK:
			++m_successCount[directoryPath];
			break;
		case iproc::IProcessor::TS_INVALID:
			++m_errorsCount[directoryPath];
			break;
		case iproc::IProcessor::TS_CANCELED:
			++m_abortedCount[directoryPath];
			break;
	}
}



// protected static methods

QString CHotfolderStatistics::GetDirectoryPath(const ihotf::IHotfolderProcessingItem& item)
{
	QString filePath = item.GetInputFile();

	return QFileInfo(filePath).absolutePath();
}


} // namespace ihotf


