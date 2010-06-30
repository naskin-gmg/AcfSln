#include "ifpf/CHotfolderProcessingItem.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/ISystemEnvironment.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iproc/IProcessor.h"


namespace ifpf
{


CHotfolderProcessingItem::CHotfolderProcessingItem()
	:m_processingState(iproc::IProcessor::TS_NONE)
{
	GetItemUuid();
}


// reimplemented ()ifpf::CHotfolderProcessingItem

std::string CHotfolderProcessingItem::GetItemUuid() const
{
	if (m_itemId.empty()){
		isys::ISystemEnvironment* systemEnvironmentPtr = istd::GetService<isys::ISystemEnvironment>();
		I_ASSERT(systemEnvironmentPtr != NULL);
		if (systemEnvironmentPtr != NULL){
			m_itemId = systemEnvironmentPtr->GetUniqueIdentifier().ToString();
		}
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
		istd::CChangeNotifier changePtr(this, CF_STATE_CHANGED, this);

		m_processingState = processingState;
	}
}


istd::CString CHotfolderProcessingItem::GetInputFile() const
{
	return m_inputFile;
}


void CHotfolderProcessingItem::SetInputFile(const istd::CString& inputFile)
{
	if (m_inputFile != inputFile){
		istd::CChangeNotifier changePtr(this);

		m_inputFile = inputFile;
	}
}


istd::CString CHotfolderProcessingItem::GetOutputFile() const
{
	return m_outputFile;
}


void CHotfolderProcessingItem::SetOutputFile(const istd::CString& outputFile)
{
	if (m_outputFile != outputFile){
		istd::CChangeNotifier changePtr(this);

		m_outputFile = outputFile;
	}
}


// reimplemented (iser::ISerializable)

bool CHotfolderProcessingItem::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag itemIdTag("Uuid", "Processing item identifier");
	retVal = retVal && archive.BeginTag(itemIdTag);
	retVal = retVal && archive.Process(m_itemId);
	retVal = retVal && archive.EndTag(itemIdTag);

	static iser::CArchiveTag processingStateTag("ProcessingState", "Current processing state");
	retVal = retVal && archive.BeginTag(processingStateTag);
	retVal = retVal && archive.Process(m_processingState);
	retVal = retVal && archive.EndTag(processingStateTag);

	static iser::CArchiveTag inputFileTag("InputFilePath", "Input file path");
	retVal = retVal && archive.BeginTag(inputFileTag);
	retVal = retVal && archive.Process(m_inputFile);
	retVal = retVal && archive.EndTag(inputFileTag);

	static iser::CArchiveTag outputFileTag("OutputFilePath", "Output file path");
	retVal = retVal && archive.BeginTag(outputFileTag);
	retVal = retVal && archive.Process(m_outputFile);
	retVal = retVal && archive.EndTag(outputFileTag);

	return retVal;
}


} // namespace ifpf


