#include "iqtfpf/CProcessingItemPreviewGeneratorComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iimg/CGeneralBitmap.h"


namespace iqtfpf
{


// protected methods

// reimplemented (imod::CSingleModelObserverBase)
	
void CProcessingItemPreviewGeneratorComp::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if ((updateFlags & ifpf::IHotfolderProcessingInfo::CF_FILE_ADDED) != 0 && m_inputFilePreviewGeneratorCompPtr.IsValid()){
		ifpf::IHotfolderProcessingItem* itemPtr = dynamic_cast<ifpf::IHotfolderProcessingItem*>(updateParamsPtr);
		if (itemPtr != NULL && m_fileNameCompPtr.IsValid()){
			iimg::CGeneralBitmap previewBitmap;

			m_fileNameCompPtr->SetPath(itemPtr->GetInputFile());

			int retVal = m_inputFilePreviewGeneratorCompPtr->DoProcessing(NULL, m_fileNameCompPtr.GetPtr(), &previewBitmap);
			if (retVal == iproc::IProcessor::TS_OK){
				itemPtr->SetInputPreview(previewBitmap);
			}
		}
	}

	if ((updateFlags & ifpf::IHotfolderProcessingItem::CF_STATE_CHANGED) != 0 && m_outputFilePreviewGeneratorCompPtr.IsValid()){
		ifpf::IHotfolderProcessingItem* itemPtr = dynamic_cast<ifpf::IHotfolderProcessingItem*>(updateParamsPtr);
		if (itemPtr != NULL && m_fileNameCompPtr.IsValid() && itemPtr->GetProcessingState() == iproc::IProcessor::TS_OK){
			iimg::CGeneralBitmap previewBitmap;

			m_fileNameCompPtr->SetPath(itemPtr->GetOutputFile());

			int retVal = m_outputFilePreviewGeneratorCompPtr->DoProcessing(NULL, m_fileNameCompPtr.GetPtr(), &previewBitmap);
			if (retVal == iproc::IProcessor::TS_OK){
				itemPtr->SetOutputPreview(previewBitmap);
			}
		}
	}
}


} // namespace iqtfpf


