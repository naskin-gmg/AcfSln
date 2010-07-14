#include "iqtfpf/CProcessingItemPreviewGeneratorComp.h"


// Qt includes
#include <QFileInfo>


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CCrcCalculator.h"


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
	
void CProcessingItemPreviewGeneratorComp::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	I_ASSERT(m_inputFilePreviewGeneratorCompPtr.IsValid());
	I_ASSERT(m_outputFilePreviewGeneratorCompPtr.IsValid());

	ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
	if (m_outputFilePreviewGeneratorCompPtr.IsValid() && objectPtr != NULL){
		if ((updateFlags & ifpf::IHotfolderProcessingItem::CF_STATE_CHANGED) != 0){

			switch (objectPtr->GetProcessingState()){
				case iproc::IProcessor::TS_OK:
					GetOutputFilePreview(*objectPtr, true);
					break;

				case iproc::IProcessor::TS_NONE:
				case iproc::IProcessor::TS_INVALID:
					ResetOutputPreview(*objectPtr);
					break;
			}
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

	PreviewMap::iterator findIter = previewMap.find(processingItemUuid);

	QFileInfo fileInfo(iqt::GetQString(filePath));

	QDateTime fileTimeStamp = fileInfo.lastModified();

	bool foundInCache = (findIter != previewMap.end());
	if (foundInCache){
		foundInCache = (findIter->second.fileTimeStamp == fileTimeStamp);
	}

	if (!foundInCache && !ensureCreated){
		BitmapPtr bitmapPtr(new BitmapImpl);
		m_fileNameCompPtr->SetPath(filePath);

		bitmapAcquisition.DoProcessing(NULL, m_fileNameCompPtr.GetPtr(), bitmapPtr.GetPtr());

		previewMap[processingItemUuid].bitmapPtr = bitmapPtr;
		previewMap[processingItemUuid].fileTimeStamp = fileTimeStamp;

		return previewMap[processingItemUuid].bitmapPtr.GetPtr();
	}
	else if (ensureCreated){
		m_fileNameCompPtr->SetPath(filePath);

		bitmapAcquisition.DoProcessing(NULL, m_fileNameCompPtr.GetPtr(), findIter->second.bitmapPtr.GetPtr());
	
		findIter->second.fileTimeStamp = fileTimeStamp;
	}

	return findIter->second.bitmapPtr.GetPtr();
}


void CProcessingItemPreviewGeneratorComp::ResetOutputPreview(const ifpf::IHotfolderProcessingItem& processingItem)
{
	PreviewMap::iterator findIter = m_outputFilePreviewMap.find(processingItem.GetItemUuid());
	if (findIter != m_outputFilePreviewMap.end()){
		findIter->second.bitmapPtr->ResetImage();
	}
}


} // namespace iqtfpf


