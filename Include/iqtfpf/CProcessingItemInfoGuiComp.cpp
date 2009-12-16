#include "iqtfpf/CProcessingItemInfoGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

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
	}
}


} // namespace iqtfpf


