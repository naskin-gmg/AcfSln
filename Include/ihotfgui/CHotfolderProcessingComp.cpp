#include "ihotfgui/CHotfolderProcessingComp.h"


// Qt includes
#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtCore/QElapsedTimer>
#include <QtCore/QMutexLocker>

// ACF includes
#include "istd/TChangeDelegator.h"
#include "istd/TChangeNotifier.h"
#include "ifile/IFileNameParam.h"
#include "iproc/IProcessor.h"


namespace ihotfgui
{


// public methods

CHotfolderProcessingComp::CHotfolderProcessingComp()
	:m_stateObserver(*this),
	m_isInitialized(false)
{
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CHotfolderProcessingComp::OnComponentCreated()
{
	connect(&m_processingTimer, SIGNAL(timeout()), this, SLOT(OnProcessingTimer()));

	m_processingTimer.start(500);

	Q_ASSERT(m_processingTimer.isActive());
	
	BaseClass::OnComponentCreated();

	m_isInitialized = true;
}


void CHotfolderProcessingComp::OnComponentDestroyed()
{
	m_processingTimer.stop();

	CancelAllProcessingItems();

	BaseClass::OnComponentDestroyed();
}


// protected slots

void CHotfolderProcessingComp::OnProcessingTimer()
{
	bool updateProcessedItemsDone = false;
	while (!updateProcessedItemsDone){
		updateProcessedItemsDone = true;
		for (int processingFutureIndex = 0; processingFutureIndex < m_pendingProcessors.GetCount(); processingFutureIndex++){
			ItemProcessor* processorPtr = m_pendingProcessors.GetAt(processingFutureIndex);
			if (processorPtr->isFinished()){
				OnProcessingItemFinished(*processorPtr);

				m_pendingProcessors.RemoveAt(processingFutureIndex);
				updateProcessedItemsDone = false;
				break;
			}
		}
	}

	if (!m_hotfolderProcessingInfoCompPtr->IsWorking()){
		return;
	}

	// get available file to process:
	if (m_taskManagerCompPtr.IsValid()){
		ihotf::IHotfolderProcessingItem* processingItemPtr = m_taskManagerCompPtr->GetNextProcessingTask();
		if (processingItemPtr != NULL){
			int runningThreadCount = 0;

			for (int processingFutureIndex = 0; processingFutureIndex < m_pendingProcessors.GetCount(); processingFutureIndex++){
				if (m_pendingProcessors.GetAt(processingFutureIndex)->isRunning()){
					runningThreadCount++;
				}
			}

			if (runningThreadCount < QThread::idealThreadCount()){
				istd::CChangeNotifier changePtr(processingItemPtr);

				processingItemPtr->SetProcessingState(iproc::IProcessor::TS_WAIT);
			

				changePtr.Reset();

				QString inputFilePath = processingItemPtr->GetInputFile();
				QString outputFilePath = processingItemPtr->GetOutputFile();
				QByteArray itemUuid = processingItemPtr->GetItemUuid();

				m_pendingProcessors.PushBack(new ItemProcessor(*this, inputFilePath, outputFilePath, itemUuid));
			}
		}
	}
}


void CHotfolderProcessingComp::OnProcessingItemFinished(const ItemProcessor& processor)
{
	ihotf::IHotfolderProcessingItem* itemPtr = GetItemFromId(processor.GetItemUuid());
	Q_ASSERT(itemPtr != NULL);
	
	if (itemPtr != NULL && itemPtr->GetProcessingState() != iproc::IProcessor::TS_CANCELED){
		istd::CChangeNotifier changePtr(itemPtr);

		itemPtr->SetProcessingState(processor.GetProcessingState());
		itemPtr->SetStartTime(processor.GetStartTime());
		itemPtr->SetProcessingTime(processor.GetProcessingTime());
	}
}


void CHotfolderProcessingComp::OnCancelProcessingItem(const ihotf::IHotfolderProcessingItem* processingItemPtr)
{
	for (int pendingProcessorIndex = 0; pendingProcessorIndex < m_pendingProcessors.GetCount(); pendingProcessorIndex++){
		ItemProcessor* processorPtr = m_pendingProcessors.GetAt(pendingProcessorIndex);
		Q_ASSERT(processorPtr != NULL);

		if (processorPtr->GetItemUuid() == processingItemPtr->GetItemUuid()){
			processorPtr->Cancel();

			m_pendingProcessors.RemoveAt(pendingProcessorIndex);

			break;
		}
	}
}


void CHotfolderProcessingComp::CancelAllProcessingItems()
{
	for (int pendingProcessorIndex = 0; pendingProcessorIndex < m_pendingProcessors.GetCount(); pendingProcessorIndex++){
		m_pendingProcessors.GetAt(pendingProcessorIndex)->Cancel();		
	}

	m_pendingProcessors.Reset();

	Q_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return;
	}

	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ihotf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);

		if (processingItemPtr->GetProcessingState() == iproc::IProcessor::TS_WAIT){
			processingItemPtr->SetProcessingState(iproc::IProcessor::TS_NONE);
		}
	}
}


ihotf::IHotfolderProcessingItem* CHotfolderProcessingComp::GetItemFromId(const QByteArray& itemUuid) const
{
	Q_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return NULL;
	}
		
	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ihotf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);

		if (processingItemPtr->GetItemUuid() == itemUuid){
			return processingItemPtr;
		}
	}

	return NULL;
}


// public methods of embedded class StateObserver

CHotfolderProcessingComp::StateObserver::StateObserver(CHotfolderProcessingComp& parent)
	:m_parent(parent)
{
}


void CHotfolderProcessingComp::StateObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if ((updateFlags & ihotf::IHotfolderProcessingItem::CF_STATE_CHANGED) != 0){
		ihotf::IHotfolderProcessingItem* processingItemPtr = dynamic_cast<ihotf::IHotfolderProcessingItem*>(updateParamsPtr);
		if (processingItemPtr != NULL && processingItemPtr->GetProcessingState() == iproc::IProcessor::TS_CANCELED){
			m_parent.OnCancelProcessingItem(processingItemPtr);
		}
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// public methods of embedded class ItemProcessor

CHotfolderProcessingComp::ItemProcessor::ItemProcessor(
			CHotfolderProcessingComp& parent,
			const QString& inputFilePath,
			const QString& outputFilePath,
			const QByteArray& itemUuid)
	:m_parent(parent),
	m_inputFilePath(inputFilePath),
	m_outputFilePath(outputFilePath),
	m_itemUuid(itemUuid),
	m_processingState(iproc::IProcessor::TS_INVALID)
{
	start(QThread::LowPriority);
}


int CHotfolderProcessingComp::ItemProcessor::GetProcessingState() const
{
	return m_processingState;
}


QByteArray CHotfolderProcessingComp::ItemProcessor::GetItemUuid() const
{
	return m_itemUuid;
}


QDateTime CHotfolderProcessingComp::ItemProcessor::GetStartTime() const
{
	return m_startTime;
}

double CHotfolderProcessingComp::ItemProcessor::GetProcessingTime() const
{
	return m_processingTime;
}


void CHotfolderProcessingComp::ItemProcessor::Cancel()
{
	if (BaseClass::isFinished()){
		return;
	}

	// wait for 5 seconds for finishing of thread:
	QElapsedTimer timer;
	while (isRunning() && !timer.hasExpired(5000)){
		qApp->processEvents();
	}

	if (isRunning()){
		BaseClass::terminate();

		BaseClass::wait();
	}
}


// protected methods

// reimplemented (QThread)

void CHotfolderProcessingComp::ItemProcessor::run()
{
	m_startTime = QDateTime::currentDateTime(); 
	
	if (!m_parent.m_fileConvertCompPtr.IsValid()){
		return;
	}

	QElapsedTimer timer;
	
	if (!m_parent.m_fileConvertCompPtr->ConvertFiles(m_inputFilePath, m_outputFilePath)){
		m_parent.SendErrorMessage(0, QObject::tr("Processing of  %1 failed").arg(m_inputFilePath), "Hotfolder");

		m_processingState = iproc::IProcessor::TS_INVALID;
	}
	else{
		m_processingState = iproc::IProcessor::TS_OK;
	}

	m_processingTime = timer.elapsed() * 0.001;
}


} // namespace ihotfgui


