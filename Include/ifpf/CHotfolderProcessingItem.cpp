#include "ifpf/CHotfolderProcessingItem.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iproc/IProcessor.h"


namespace ifpf
{


CHotfolderProcessingItem::CHotfolderProcessingItem()
	:m_processingState(iproc::IProcessor::TS_NONE),
	m_progress(0.0)
{
}


void CHotfolderProcessingItem::SetProcessingState(int processingState)
{
	if (m_processingState != processingState){
		istd::CChangeNotifier changePtr(this);

		m_processingState = processingState;
	}
}


void CHotfolderProcessingItem::SetProgress(double progress)
{
}


void CHotfolderProcessingItem::SetInputFile(const istd::CString& inputFile)
{
	if (m_inputFile != inputFile){
		istd::CChangeNotifier changePtr(this);

		m_inputFile = inputFile;
	}

}


void CHotfolderProcessingItem::SetOutputFile(const istd::CString& outputFile)
{
	if (m_outputFile != outputFile){
		istd::CChangeNotifier changePtr(this);

		m_outputFile = outputFile;
	}
}


// reimplemented ()ifpf::CHotfolderProcessingItem

int CHotfolderProcessingItem::GetProcessingState() const
{
	return m_processingState;
}


double CHotfolderProcessingItem::GetProgress() const
{
	return m_progress;
}


istd::CString CHotfolderProcessingItem::GetInputFile() const
{
	return m_inputFile;
}


istd::CString CHotfolderProcessingItem::GetOutputFile() const
{
	return m_outputFile;
}


} // namespace ifpf


