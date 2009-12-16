#include "iqtfpf/CProcessingItemInfoGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iproc/IProcessor.h"

#include "iqt/CSignalBlocker.h"


namespace iqtfpf
{


// public methods

// reimplemented (imod::IModelEditor)

void CProcessingItemInfoGuiComp::UpdateModel() const
{
}


void CProcessingItemInfoGuiComp::UpdateEditor(int /*updateFlags*/)
{
	ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		InputFilePathLabel->setText(iqt::GetQString(objectPtr->GetInputFile()));
		OutputFilePathLabel->setText(iqt::GetQString(objectPtr->GetOutputFile()));
		ItemProgress->setValue(objectPtr->GetProgress() * 100);

		int itemState = objectPtr->GetProcessingState();
	
		ItemProgress->setVisible(itemState == iproc::IProcessor::TS_WAIT || itemState == iproc::IProcessor::TS_NONE);
		ProgressLabel->setVisible(itemState == iproc::IProcessor::TS_WAIT || itemState == iproc::IProcessor::TS_NONE);

		switch (itemState){
			case iproc::IProcessor::TS_OK:
				StateLabel->setText(tr("Processing finished successfully"));
				break;
			case iproc::IProcessor::TS_INVALID:
				StateLabel->setText(tr("Processing finished with any error(s)"));
				break;
			case iproc::IProcessor::TS_NONE:
				StateLabel->setText(tr("Waiting for processing..."));
				break;
			case iproc::IProcessor::TS_CANCELED:
				StateLabel->setText(tr("Processing was canceled"));
				break;
		}
	}
}


} // namespace iqtfpf


