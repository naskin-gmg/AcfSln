#include "ifpf/CHotfolderProcessingItem.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iproc/IProcessor.h"


namespace ifpf
{


CHotfolderProcessingItem::CHotfolderProcessingItem()
	:m_processingState(iproc::IProcessor::TS_NONE),
	m_progress(0.0)
{
}


// reimplemented ()ifpf::CHotfolderProcessingItem

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


double CHotfolderProcessingItem::GetProgress() const
{
	return m_progress;
}


void CHotfolderProcessingItem::SetProgress(double progress)
{
	if (m_progress != progress){
		istd::CChangeNotifier changePtr(this, CF_PROGRESS_CHANGED);

		m_progress = progress;
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


const iimg::IBitmap& CHotfolderProcessingItem::GetInputPreview() const
{
	return m_inputPreview;
}


void CHotfolderProcessingItem::SetInputPreview(const iimg::IBitmap& inputPreview)
{
	istd::CChangeNotifier changePtr(this, CF_INPUT_PREVIEW_CHANGED);

	m_inputPreview.CopyFrom(inputPreview);
}


const iimg::IBitmap& CHotfolderProcessingItem::GetOutputPreview() const
{
	return m_outputPreview;
}


void CHotfolderProcessingItem::SetOutputPreview(const iimg::IBitmap& outputPreview)
{
	istd::CChangeNotifier changePtr(this, CF_OUTPUT_PREVIEW_CHANGED);

	m_outputPreview.CopyFrom(outputPreview);
}


// reimplemented (iser::ISerializable)

bool CHotfolderProcessingItem::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag processingStateTag("ProcessingState", "Current processing state");
	retVal = retVal && archive.BeginTag(processingStateTag);
	retVal = retVal && archive.Process(m_processingState);
	retVal = retVal && archive.EndTag(processingStateTag);

	static iser::CArchiveTag progressTag("Progress", "Current processing progress");
	retVal = retVal && archive.BeginTag(progressTag);
	retVal = retVal && archive.Process(m_progress);
	retVal = retVal && archive.EndTag(progressTag);

	static iser::CArchiveTag inputFileTag("InputFilePath", "Input file path");
	retVal = retVal && archive.BeginTag(inputFileTag);
	retVal = retVal && archive.Process(m_inputFile);
	retVal = retVal && archive.EndTag(inputFileTag);

	static iser::CArchiveTag outputFileTag("OutputFilePath", "Output file path");
	retVal = retVal && archive.BeginTag(outputFileTag);
	retVal = retVal && archive.Process(m_outputFile);
	retVal = retVal && archive.EndTag(outputFileTag);

	static iser::CArchiveTag inputFilePreviewTag("InputFilePreview", "Preview of the input file");
	retVal = retVal && archive.BeginTag(inputFilePreviewTag);
	retVal = retVal && m_inputPreview.Serialize(archive);
	retVal = retVal && archive.EndTag(inputFilePreviewTag);
	
	static iser::CArchiveTag outputFilePreviewTag("OutputFilePreview", "Preview of the output file");
	retVal = retVal && archive.BeginTag(outputFilePreviewTag);
	retVal = retVal && m_outputPreview.Serialize(archive);
	retVal = retVal && archive.EndTag(outputFilePreviewTag);


	return retVal;
}


} // namespace ifpf


