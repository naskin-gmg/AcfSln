#pragma once


// ACF includes
#include <istd/TPointerVector.h>
#include <imod/IModel.h>
#include <imod/TSingleModelObserverBase.h>
#include <idoc/IMultiPageDocument.h>
#include <iimg/IBitmap.h>
#include <iimg/IMultiBitmapProvider.h>
#include <icomp/CComponentBase.h>
#include <iproc/IProcessor.h>


namespace idocproc
{


class CRenderedDocumentPreviewGeneratorComp:
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<idoc::IMultiPageDocument>,
			virtual public iimg::IMultiBitmapProvider
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<idoc::IMultiPageDocument> BaseClass2;

	I_BEGIN_COMPONENT(CRenderedDocumentPreviewGeneratorComp);
		I_REGISTER_INTERFACE(iimg::IMultiBitmapProvider);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_bitmapFactoryCompPtr, "BitmapFactory", "Factory for the preview bitmap creation", false, "BitmapFactory");
		I_ASSIGN(m_renderingProcessorCompPtr, "RenderingProcessor", "Processor used for page rendering into a bitmap", true, "RenderingProcessor");
		I_ASSIGN(m_imageSizeAttrPtr, "ImageSize", "Image size in pixel using for rendering of the document page", true, 4096);
		I_ASSIGN(m_pageSelectionAttrPtr, "PageSelectionParamId", "ID of the page selection in the parameter set used by the rendering processor", true, "PageSelection");
	I_END_COMPONENT;

	// reimplemented (iimg::IMultiBitmapProvider)
	virtual const iprm::IOptionsList* GetBitmapListInfo() const;
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;

protected:
	// reimplemented (imod::IObserver)
	virtual bool OnModelDetached(imod::IModel* modelPtr);

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

protected:
	virtual void EnsurePreviewGenerated();

private:
	I_FACT(iimg::IBitmap, m_bitmapFactoryCompPtr);
	I_REF(iproc::IProcessor, m_renderingProcessorCompPtr);
	I_ATTR(int, m_imageSizeAttrPtr);
	I_ATTR(QByteArray, m_pageSelectionAttrPtr);

	typedef istd::TPointerVector<iimg::IBitmap> PreviewBitmaps;

	PreviewBitmaps m_previewBitmaps;
};


} // namespace idocproc


