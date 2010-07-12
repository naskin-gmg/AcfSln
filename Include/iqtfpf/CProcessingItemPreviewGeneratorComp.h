#ifndef iqtfpf_CProcessingItemPreviewGeneratorComp_included
#define iqtfpf_CProcessingItemPreviewGeneratorComp_included


// ACF includes
#include "istd/TSmartPtr.h"

#include "imod/TSingleModelObserverBase.h"
#include "imod/TModelWrap.h"

#include "icomp/CComponentBase.h"

#include "iproc/IBitmapAcquisition.h"

#include "iprm/IFileNameParam.h"

#include "iqt/CBitmap.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingInfo.h"
#include "ifpf/IHotfolderProcessingItem.h"
#include "ifpf/IProcessingItemPreviewProvider.h"


namespace iqtfpf
{


class CProcessingItemPreviewGeneratorComp:
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingItem>,
			virtual public ifpf::IProcessingItemPreviewProvider,
			virtual public iser::ISerializable
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingItem> BaseClass2;

	I_BEGIN_COMPONENT(CProcessingItemPreviewGeneratorComp)
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(ifpf::IProcessingItemPreviewProvider);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_inputFilePreviewGeneratorCompPtr, "InputFilePreviewGenerator", "Input file preview generator", true, "InputFilePreviewGenerator");
		I_ASSIGN(m_outputFilePreviewGeneratorCompPtr, "OutputFilePreviewGenerator", "Output file preview generator", true, "OutputFilePreviewGenerator");
		I_ASSIGN(m_fileNameCompPtr, "FilePath", "File path definition component", true, "FilePath");
	I_END_COMPONENT;

	// reimplemented (ifpf::IProcessingItemPreviewProvider)
	virtual const iimg::IBitmap* GetInputFilePreview(const ifpf::IHotfolderProcessingItem& item, bool ensureCreated = false) const;
	virtual const iimg::IBitmap* GetOutputFilePreview(const ifpf::IHotfolderProcessingItem& item, bool ensureCreated = false) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	typedef imod::TModelWrap<iqt::CBitmap> BitmapImpl;
	typedef istd::TSmartPtr<BitmapImpl> BitmapPtr;
	typedef std::map<std::string, BitmapPtr> PreviewMap;

	const iimg::IBitmap* GetFilePreview(
				const std::string& processingItemUuid,
				const istd::CString& filePath,
				iproc::IProcessor& bitmapAcquisition,
				PreviewMap& previewMap,
				bool ensureCreated = false) const;

private:
	I_REF(iproc::IBitmapAcquisition, m_inputFilePreviewGeneratorCompPtr);
	I_REF(iproc::IBitmapAcquisition, m_outputFilePreviewGeneratorCompPtr);
	I_REF(iprm::IFileNameParam, m_fileNameCompPtr);

	mutable PreviewMap m_inputFilePreviewMap;
	mutable PreviewMap m_outputFilePreviewMap;
};


} // namespace iqtfpf


#endif // !iqtfpf_CProcessingItemPreviewGeneratorComp_included


