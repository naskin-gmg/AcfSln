#include "iqtfpf/CDirectoryItemGuiComp.h"


namespace iqtfpf
{


// public methods

void CDirectoryItemGuiComp::SetDirectoryPath(const QString& directoryPath)
{
	m_directoryPath = directoryPath;
}


// reimplemented (imod::IModelEditor)

void CDirectoryItemGuiComp::UpdateModel() const
{
}


void CDirectoryItemGuiComp::UpdateEditor(int /*updateFlags*/)
{
	ifpf::IHotfolderStatistics* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		DirectoryLabel->setVisible(!m_directoryPath.isEmpty());
		DirectoryLabel->setText(m_directoryPath);

		UpdateProgressBar(*objectPtr);
	}
}


// private methods

void CDirectoryItemGuiComp::UpdateProgressBar(const ifpf::IHotfolderStatistics& statistics)
{
	int itemsCount = statistics.GetItemsCount(iqt::GetCString(m_directoryPath));

	double successed = itemsCount == 0 ? 0 : statistics.GetSuccessCount(iqt::GetCString(m_directoryPath)) / double(itemsCount);
	double errors = itemsCount == 0 ? 0 : statistics.GetErrorsCount(iqt::GetCString(m_directoryPath)) / double(itemsCount);
	double aborted = itemsCount == 0 ? 0 : statistics.GetAbortedCount(iqt::GetCString(m_directoryPath)) / double(itemsCount);

	int processedCountPercent = (successed + errors + aborted) * 100;

	ProcessedProgressBar->setValue(processedCountPercent);
	ProcessedProgressBar->setVisible(processedCountPercent != 0);

	QString styleSheet = GenerateStyleSheet(successed, errors, aborted);

	ProcessedProgressBar->setStyleSheet(styleSheet);
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


