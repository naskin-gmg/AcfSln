#include "iqtfpf/CDirectoryItemGuiComp.h"


#include "iprm/IParamsSet.h"


namespace iqtfpf
{


// public methods

CDirectoryItemGuiComp::CDirectoryItemGuiComp()
:	m_setIndex(-1)
{
}


// reimplemented (iprm::ISelectionParam)

const iprm::ISelectionConstraints* CDirectoryItemGuiComp::GetSelectionConstraints() const
{
	return NULL;
}


int CDirectoryItemGuiComp::GetSelectedOptionIndex() const
{
	return m_setIndex;
}


bool CDirectoryItemGuiComp::SetSelectedOptionIndex(int index)
{
	m_setIndex = index;

	if (m_inputDirectoriesParamsManagerCompPtr.IsValid()){
		int setsCount = m_inputDirectoriesParamsManagerCompPtr->GetParamsSetsCount();
		if (m_setIndex < setsCount && m_setIndex >= 0){
			QString setName = m_inputDirectoriesParamsManagerCompPtr->GetParamsSetName(m_setIndex);

			iprm::IParamsSet* paramSetPtr = m_inputDirectoriesParamsManagerCompPtr->GetParamsSet(m_setIndex);

			std::string fileNameParamId;
			if (m_directoryPathIdAttrPtr.IsValid()){
				fileNameParamId = (*m_directoryPathIdAttrPtr).toStdString();
			}

			m_hotfolderInputName = setName;

			const iprm::IFileNameParam* fileNameParamPtr = dynamic_cast<const iprm::IFileNameParam*>(paramSetPtr->GetParameter(fileNameParamId));
			if (fileNameParamPtr != NULL){
				m_directoryPath = fileNameParamPtr->GetPath();
			}
		}
	}

	return true;
}


iprm::ISelectionParam* CDirectoryItemGuiComp::GetActiveSubselection() const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CDirectoryItemGuiComp::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


// reimplemenented (iqtgui::TGuiObserverWrap)

void CDirectoryItemGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	ifpf::IHotfolderStatistics* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		DirectoryLabel->setVisible(!m_directoryPath.isEmpty());
		DirectoryLabel->setText(m_directoryPath);
		PathNameLabel->setText(m_hotfolderInputName);

		UpdateProgressBar(*objectPtr);
	}
}


// private methods

void CDirectoryItemGuiComp::UpdateProgressBar(const ifpf::IHotfolderStatistics& statistics)
{
	int itemsCount = statistics.GetItemsCount(m_directoryPath);
	int successCount = statistics.GetSuccessCount(m_directoryPath);
	int errorCount = statistics.GetErrorsCount(m_directoryPath);
	int abortedCount = statistics.GetAbortedCount(m_directoryPath);

	I_ASSERT(itemsCount >= (successCount + errorCount + abortedCount));

	double successed = itemsCount == 0 ? 0 : successCount / double(itemsCount);
	double errors = itemsCount == 0 ? 0 : errorCount / double(itemsCount);
	double aborted = itemsCount == 0 ? 0 : abortedCount / double(itemsCount);

	int processedCountPercent = (successed + errors + aborted) * 100;

	ProcessedProgressBar->setValue(processedCountPercent);

	QString styleSheet = GenerateStyleSheet(successed, errors, aborted);

	ProcessedProgressBar->setStyleSheet(styleSheet);

	FilesCountLabel->setText(QString("%1").arg(itemsCount));
	ProcessedFileCountLabel->setText(QString("%1 (%2%)").arg(successCount + errorCount + abortedCount).arg(processedCountPercent));
	ErrorsCountLabel->setText(QString("%1 (%2%)").arg(errorCount).arg(errors * 100, 1, 'f', 1));
}


QString CDirectoryItemGuiComp::GenerateStyleSheet(double successed, double errors, double aborted) const
{
	double delta = 0.00001;

	istd::CRange successRange(0, successed);
	double boundary = successRange.GetMaxValue();
	if (boundary > 0){
		boundary += delta;
	}

	boundary = istd::Min(1.0, boundary);

	istd::CRange errorRange(boundary, errors + boundary);
	boundary = errorRange.GetMaxValue();
	if (boundary > 0){
		boundary += delta;
	}

	boundary = istd::Min(1.0, boundary);

	istd::CRange abortedRange(boundary, boundary + aborted);
	boundary = abortedRange.GetMaxValue();
	if (boundary > 0){
		boundary += delta;
	}

	boundary = istd::Min(1.0, boundary);

	QString successColor = "rgba(50, 180, 60, 255)";
	QString errorColor = "rgba(255, 0, 0, 192)";
	QString abortedColor = "rgba(165, 201, 239, 255)";

	QString styleSheet = QString("QProgressBar::chunk{background: transparent;} QProgressBar{background:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0");
	
	if (!successRange.IsEmpty()){
		styleSheet += QString(", stop:%1 %2, stop:%3 %4").
					arg(successRange.GetMinValue(), 5, 'f', 5).
					arg(successColor).
					arg(successRange.GetMaxValue(), 5, 'f', 5).
					arg(successColor);
	}

	if (!errorRange.IsEmpty()){
		styleSheet += QString(" ,stop:%1 %2, stop:%3 %4").
					arg(errorRange.GetMinValue(), 5, 'f', 5).
					arg(errorColor).
					arg(errorRange.GetMaxValue(), 5, 'f', 5).
					arg(errorColor);
	}

	if (!abortedRange.IsEmpty()){
		styleSheet += QString(" ,stop:%1 %2, stop:%3 %4").
					arg(abortedRange.GetMinValue(), 5, 'f', 5).
					arg(abortedColor).
					arg(abortedRange.GetMaxValue(), 5, 'f', 5).
					arg(abortedColor);
	}

	if (boundary < 1.0){
		styleSheet += QString(" ,stop:%1 rgba(255,255,255,255), stop:1.0 rgba(255,255,255,255)").
					arg(boundary, 5, 'f', 5);
	}
	
	styleSheet += ");}";

	return styleSheet;
}


} // namespace iqtfpf


