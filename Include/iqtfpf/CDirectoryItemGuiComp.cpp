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

		int itemsCount = objectPtr->GetItemsCount(iqt::GetCString(m_directoryPath));
		
		int abortedCount = objectPtr->GetAbortedCount(iqt::GetCString(m_directoryPath));
		int abortedCountPercent = itemsCount == 0 ? 0: 100 * abortedCount / double(itemsCount);

		int errorsCount = objectPtr->GetErrorsCount(iqt::GetCString(m_directoryPath));
		int errorsCountPercent = itemsCount == 0 ? 0: 100 * errorsCount / double(itemsCount);

		int processedCount = objectPtr->GetProcessedCount(iqt::GetCString(m_directoryPath));
		int processedCountPercent = itemsCount == 0 ? 0: 100 * processedCount / double(itemsCount);

		ErrorsProgressBar->setVisible(false);
		AbortedProgressBar->setVisible(false);

		ProcessedProgressBar->setValue(processedCountPercent);
		ProcessedProgressBar->setVisible(processedCountPercent != 0);
		ErrorsProgressBar->setValue(errorsCountPercent);
		ErrorsProgressBar->setVisible(errorsCountPercent != 0);
		AbortedProgressBar->setValue(abortedCountPercent);
		AbortedProgressBar->setVisible(abortedCountPercent != 0);
	}
}


} // namespace iqtfpf


