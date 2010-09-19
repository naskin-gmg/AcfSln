#include "iqtfpf/CProcessingItemPreviewGuiComp.h"


// Qt includes
#include <QFileInfo>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iproc/IProcessor.h"

#include "iqt/CSignalBlocker.h"


namespace iqtfpf
{


// public methods

// reimplemented (imod::IModelEditor)

void CProcessingItemPreviewGuiComp::UpdateModel() const
{
}


void CProcessingItemPreviewGuiComp::UpdateEditor(int /*updateFlags*/)
{
	ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int processingState = objectPtr->GetProcessingState();
		switch (processingState){
			case iproc::IProcessor::TS_NONE:
				ProcessingStateLabel->setText(tr("Waiting for processing..."));
				break;
			case iproc::IProcessor::TS_OK:
				ProcessingStateLabel->setText(tr("OK"));
				break;
			case iproc::IProcessor::TS_INVALID:
				ProcessingStateLabel->setText(tr("Error"));
				break;
			case iproc::IProcessor::TS_WAIT:
				ProcessingStateLabel->setText(tr("Processing..."));
				break;
			case iproc::IProcessor::TS_CANCELED:
				ProcessingStateLabel->setText(tr("Canceled"));
				break;
		}

		QString inputFile = iqt::GetQString(objectPtr->GetInputFile());
		QString outputFile = iqt::GetQString(objectPtr->GetOutputFile());

		double inputFileSize = 0.0;
		double outputFileSize = 0.0;

		QFileInfo inputFileInfo(inputFile);
		if (inputFileInfo.exists()){
			inputFileSize = inputFileInfo.size() / double(1 << 10);
		}
		
		QFileInfo outputFileInfo(outputFile);
		if (outputFileInfo.exists()){
			outputFileSize = outputFileInfo.size() / double(1 << 10);
		}
		
		QString inputFileNameLink = inputFileInfo.exists() ? QString("<a href=\"%1\">%2</a>").arg(inputFile).arg(inputFileInfo.completeBaseName()) : inputFileInfo.completeBaseName();
		InputFileNameLabel->setText(inputFileNameLink);
		InputFileNameLabel->setToolTip(inputFile);

		QString outputFileNameLink = outputFileInfo.exists() ? QString("<a href=\"%1\">%2</a>").arg(outputFile).arg(outputFileInfo.completeBaseName()) : outputFileInfo.completeBaseName();
		OutputFileNameLabel->setText(outputFileNameLink);
		OutputFileNameLabel->setToolTip(outputFile);

		InputFileSizeLabel->setText(QString("%1 KBytes").arg(inputFileSize, 1, 'f', 1));
		OutputFileSizeLabel->setText(QString("%1 KBytes").arg(outputFileSize, 1, 'f', 1));
	}
}


// reimplemented TGuiObserverWrap

void CProcessingItemPreviewGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	if (m_inputPreviewObserverCompPtr.IsValid() && m_processingItemPreviewProviderCompPtr.IsValid()){
		ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(m_processingItemPreviewProviderCompPtr->GetInputFilePreview(*objectPtr));
			if (modelPtr != NULL){
				(const_cast<imod::IModel*>(modelPtr))->AttachObserver(m_inputPreviewObserverCompPtr.GetPtr());
			}
		}
	}

	if (m_outputPreviewObserverCompPtr.IsValid() && m_processingItemPreviewProviderCompPtr.IsValid()){
		ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(m_processingItemPreviewProviderCompPtr->GetOutputFilePreview(*objectPtr));
			if (modelPtr != NULL){
				(const_cast<imod::IModel*>(modelPtr))->AttachObserver(m_outputPreviewObserverCompPtr.GetPtr());
			}
		}
	}
}


void CProcessingItemPreviewGuiComp::OnGuiModelDetached()
{
	if (m_inputPreviewObserverCompPtr.IsValid() && m_processingItemPreviewProviderCompPtr.IsValid()){
		ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(m_processingItemPreviewProviderCompPtr->GetInputFilePreview(*objectPtr));
			if (modelPtr != NULL && modelPtr->IsAttached(m_inputPreviewObserverCompPtr.GetPtr())){
				(const_cast<imod::IModel*>(modelPtr))->DetachObserver(m_inputPreviewObserverCompPtr.GetPtr());
			}
		}
	}

	if (m_outputPreviewObserverCompPtr.IsValid() && m_processingItemPreviewProviderCompPtr.IsValid()){
		ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(m_processingItemPreviewProviderCompPtr->GetOutputFilePreview(*objectPtr));
			if (modelPtr != NULL && modelPtr->IsAttached(m_outputPreviewObserverCompPtr.GetPtr())){
				(const_cast<imod::IModel*>(modelPtr))->DetachObserver(m_outputPreviewObserverCompPtr.GetPtr());
			}
		}
	}

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CProcessingItemPreviewGuiComp::OnGuiCreated()
{
	if (m_inputPreviewObserverCompPtr.IsValid() && m_inputPreviewGuiCompPtr.IsValid()){
		m_inputPreviewGuiCompPtr->CreateGui(InputPreviewFrame);
	}

	if (m_outputPreviewObserverCompPtr.IsValid() && m_outputPreviewGuiCompPtr.IsValid()){
		m_outputPreviewGuiCompPtr->CreateGui(OutputPreviewFrame);
	}

	BaseClass::OnGuiCreated();
}


void CProcessingItemPreviewGuiComp::OnGuiDestroyed()
{
	if (m_inputPreviewGuiCompPtr.IsValid()){
		m_inputPreviewGuiCompPtr->DestroyGui();
	}

	if (m_outputPreviewGuiCompPtr.IsValid()){
		m_outputPreviewGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtfpf


