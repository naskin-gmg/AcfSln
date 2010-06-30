#include "iqtfpf/CProcessingItemPreviewGeneratorComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtfpf
{


// public methods

// reimplemented (ifpf::IProcessingItemPreviewProvider)

const iimg::IBitmap* CProcessingItemPreviewGeneratorComp::GetInputFilePreview(const ifpf::IHotfolderProcessingItem& item, bool ensureCreated) const
{
	std::string itemUuid = item.GetItemUuid();
	I_ASSERT(!itemUuid.empty());
	if (itemUuid.empty()){
		return NULL;
	}

	I_ASSERT(m_inputFilePreviewGeneratorCompPtr.IsValid());
	if (!m_inputFilePreviewGeneratorCompPtr.IsValid()){
		return NULL;
	}

	return GetFilePreview(itemUuid, item.GetInputFile(), *m_inputFilePreviewGeneratorCompPtr.GetPtr(), m_inputFilePreviewMap, ensureCreated);
}


const iimg::IBitmap* CProcessingItemPreviewGeneratorComp::GetOutputFilePreview(const ifpf::IHotfolderProcessingItem& item, bool ensureCreated) const
{
	std::string itemUuid = item.GetItemUuid();
	I_ASSERT(!itemUuid.empty());
	if (itemUuid.empty()){
		return NULL;
	}

	I_ASSERT(m_outputFilePreviewGeneratorCompPtr.IsValid());
	if (!m_outputFilePreviewGeneratorCompPtr.IsValid()){
		return NULL;
	}

	return GetFilePreview(itemUuid, item.GetOutputFile(), *m_outputFilePreviewGeneratorCompPtr.GetPtr(), m_outputFilePreviewMap, ensureCreated);
}


// reimplemented (iser::ISerializable)

bool CProcessingItemPreviewGeneratorComp::Serialize(iser::IArchive& /*archive*/)
{
	bool retVal = true;

	return retVal;
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)
	
void CProcessingItemPreviewGeneratorComp::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	I_ASSERT(m_inputFilePreviewGeneratorCompPtr.IsValid());
	I_ASSERT(m_outputFilePreviewGeneratorCompPtr.IsValid());

	if ((updateFlags & ifpf::IHotfolderProcessingItem::CF_STATE_CHANGED) != 0 && m_outputFilePreviewGeneratorCompPtr.IsValid()){
		ifpf::IHotfolderProcessingItem* itemPtr = dynamic_cast<ifpf::IHotfolderProcessingItem*>(updateParamsPtr);
		if (itemPtr != NULL){
			GetOutputFilePreview(*itemPtr, true);
		}
	}
}


const iimg::IBitmap* CProcessingItemPreviewGeneratorComp::GetFilePreview(
			const std::string& processingItemUuid,
			const istd::CString& filePath,
			iproc::IProcessor& bitmapAcquisition,
			PreviewMap& previewMap,
			bool ensureCreated) const
{
	I_ASSERT(m_fileNameCompPtr.IsValid());
	if (!m_fileNameCompPtr.IsValid()){
		return NULL;
	}

	BitmapPtr bitmapPtr(new BitmapImpl);
	PreviewMap::iterator findIter = previewMap.find(processingItemUuid);
	if (findIter == previewMap.end()){
		m_fileNameCompPtr->SetPath(filePath);

		bitmapAcquisition.DoProcessing(NULL, m_fileNameCompPtr.GetPtr(), bitmapPtr.GetPtr());

		previewMap[processingItemUuid] = bitmapPtr;

		return bitmapPtr.GetPtr();
	}
	else if (ensureCreated){
		m_fileNameCompPtr->SetPath(filePath);

		bitmapAcquisition.DoProcessing(NULL, m_fileNameCompPtr.GetPtr(), findIter->second.GetPtr());
	}

	return findIter->second.GetPtr();
}


} // namespace iqtfpf


