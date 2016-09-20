#include <idocproc/CRenderedDocumentPreviewGeneratorComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iimg/CBitmap.h>
#include <iprm/CParamsSet.h>
#include <iprm/CSelectionParam.h>
#include <imod/TModelWrap.h>


namespace idocproc
{


// public methods

// reimplemented (iimg::IMultiBitmapProvider)

const iprm::IOptionsList* CRenderedDocumentPreviewGeneratorComp::GetBitmapListInfo() const
{
	return NULL;
}


int CRenderedDocumentPreviewGeneratorComp::GetBitmapsCount() const
{
	return m_previewBitmaps.GetCount();
}


const iimg::IBitmap* CRenderedDocumentPreviewGeneratorComp::GetBitmap(int bitmapIndex) const
{
	Q_ASSERT(bitmapIndex >= 0);
	Q_ASSERT(bitmapIndex < m_previewBitmaps.GetCount());

	return m_previewBitmaps.GetAt(bitmapIndex);
}


// protected methods

// reimplemented (imod::IObserver)

bool CRenderedDocumentPreviewGeneratorComp::OnModelDetached(imod::IModel* modelPtr)
{
	if (BaseClass2::OnModelDetached(modelPtr)){

		istd::CChangeNotifier changePtr(this);

		m_previewBitmaps.Reset();

		return true;
	}

	return false;
}


// reimplemented (imod::CSingleModelObserverBase)

void CRenderedDocumentPreviewGeneratorComp::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	EnsurePreviewGenerated();
}


// private methods

void CRenderedDocumentPreviewGeneratorComp::EnsurePreviewGenerated()
{
	istd::CChangeNotifier changePtr(this);

	m_previewBitmaps.Reset();

	idoc::IMultiPageDocument* documentPtr = GetObservedObject();
	if ((documentPtr != NULL) && m_renderingProcessorCompPtr.IsValid()){
		int pageCount = documentPtr->GetPagesCount();

		for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex){
			iprm::CParamsSet processingParams;
			iprm::CSelectionParam pageSelection;
			pageSelection.SetSelectedOptionIndex(pageIndex);
			processingParams.SetEditableParameter(*m_pageSelectionAttrPtr, &pageSelection);

			istd::TDelPtr<iimg::IBitmap> previewBitmapPtr;

			if (m_bitmapFactoryCompPtr.IsValid()){
				previewBitmapPtr.SetPtr(m_bitmapFactoryCompPtr.CreateInstance());
			}
			else{
				previewBitmapPtr.SetPtr(new imod::TModelWrap<iimg::CBitmap>);
			}
	
			if (previewBitmapPtr.IsValid()){
				int state = m_renderingProcessorCompPtr->DoProcessing(&processingParams, documentPtr, previewBitmapPtr.GetPtr());
				if (state == iproc::IProcessor::TS_OK){
					m_previewBitmaps.PushBack(previewBitmapPtr.PopPtr());
				}
			}
		}
	}
}


} // namespace idocproc


