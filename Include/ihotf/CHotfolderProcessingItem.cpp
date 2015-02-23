#include "ihotf/CHotfolderProcessingItem.h"


// Qt includes
#include <QtCore/QUuid>

// ACF includes
#include "istd/CChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iser/CPrimitiveTypesSerializer.h"
#include "iproc/IProcessor.h"


namespace ihotf
{


CHotfolderProcessingItem::CHotfolderProcessingItem()
	:m_processingState(iproc::IProcessor::TS_NONE),
	m_processingTime(0.0)
{
	GetItemUuid();
}


// reimplemented ()ihotf::CHotfolderProcessingItem

QByteArray CHotfolderProcessingItem::GetItemUuid() const
{
	if (m_itemId.isEmpty()){
		m_itemId = QUuid::createUuid().toString().toLocal8Bit();
	}

	return m_itemId;
}


int CHotfolderProcessingItem::GetProcessingState() const
{
	return m_processingState;
}


void CHotfolderProcessingItem::SetProcessingState(int processingState)
{
	if (m_processingState != processingState){
		ChangeSet changeSet(CF_STATE_CHANGED);
		istd::CChangeNotifier changePtr(this, changeSet);

		m_processingState = processingState;
	}
}


QString CHotfolderProcessingItem::GetInputFile() const
{
	return m_inputFile;
}


void CHotfolderProcessingItem::SetInputFile(const QString& inputFile)
{
	if (m_inputFile != inputFile){
		istd::CChangeNotifier changePtr(this);

		m_inputFile = inputFile;
	}
}


QString CHotfolderProcessingItem::GetOutputFile() const
{
	return m_outputFile;
}


void CHotfolderProcessingItem::SetOutputFile(const QString& outputFile)
{
	if (m_outputFile != outputFile){
		istd::CChangeNotifier changePtr(this);

		m_outputFile = outputFile;
	}
}


double CHotfolderProcessingItem::GetProcessingTime() const
{
	return m_processingTime;
}


void CHotfolderProcessingItem::SetProcessingTime(double processingTime)
{
	if (m_processingTime != processingTime){
		istd::CChangeNotifier changePtr(this);

		m_processingTime = processingTime;
	}
}


const QDateTime& CHotfolderProcessingItem::GetStartTime() const
{
	return m_startTime;
}


void CHotfolderProcessingItem::SetStartTime(const QDateTime& startTime)
{
	if (m_startTime != startTime){
		istd::CChangeNotifier changePtr(this);

		m_startTime = startTime;
	}
}


// reimplemented (iser::ISerializable)

bool CHotfolderProcessingItem::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag itemIdTag("Uuid", "Processing item identifier", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag processingStateTag("ProcessingState", "Current processing state", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag inputFileTag("InputFilePath", "Input file path", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag outputFileTag("OutputFilePath", "Output file path", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag processingTimeTag("ProcessingTime", "Processing time", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag startTimeTag("StartTime", "Start time of the processing", iser::CArchiveTag::TT_LEAF);

	bool retVal = true;

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

	retVal = retVal && archive.BeginTag(itemIdTag);
	retVal = retVal && archive.Process(m_itemId);
	retVal = retVal && archive.EndTag(itemIdTag);

	retVal = retVal && archive.BeginTag(processingStateTag);
	retVal = retVal && archive.Process(m_processingState);
	retVal = retVal && archive.EndTag(processingStateTag);

	retVal = retVal && archive.BeginTag(inputFileTag);
	retVal = retVal && archive.Process(m_inputFile);
	retVal = retVal && archive.EndTag(inputFileTag);

	retVal = retVal && archive.BeginTag(outputFileTag);
	retVal = retVal && archive.Process(m_outputFile);
	retVal = retVal && archive.EndTag(outputFileTag);

	retVal = retVal && archive.BeginTag(processingTimeTag);
	retVal = retVal && archive.Process(m_processingTime);
	retVal = retVal && archive.EndTag(processingTimeTag);

	retVal = retVal && archive.BeginTag(startTimeTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeDateTime(archive, m_startTime);
	retVal = retVal && archive.EndTag(startTimeTag);

	return retVal;
}


} // namespace ihotf


