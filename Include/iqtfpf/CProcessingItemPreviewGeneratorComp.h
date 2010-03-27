#ifndef iqtfpf_CProcessingItemPreviewGeneratorComp_included
#define iqtfpf_CProcessingItemPreviewGeneratorComp_included


// ACF includes
#include "imod/TSingleModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "iproc/IBitmapAcquisition.h"

#include "iprm/IFileNameParam.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingInfo.h"
#include "ifpf/IHotfolderProcessingItem.h"


namespace iqtfpf
{


class CProcessingItemPreviewGeneratorComp: public icomp::CComponentBase, public imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingInfo>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingInfo> BaseClass2;

	I_BEGIN_COMPONENT(CProcessingItemPreviewGeneratorComp)
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_inputFilePreviewGeneratorCompPtr, "InputFilePreviewGenerator", "Input file preview generator", true, "InputFilePreviewGenerator");
		I_ASSIGN(m_outputFilePreviewGeneratorCompPtr, "OutputFilePreviewGenerator", "Output file preview generator", true, "OutputFilePreviewGenerator");
		I_ASSIGN(m_fileNameCompPtr, "FilePath", "File path definition component", true, "FilePath");
	I_END_COMPONENT;

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	I_REF(iproc::IBitmapAcquisition, m_inputFilePreviewGeneratorCompPtr);
	I_REF(iproc::IBitmapAcquisition, m_outputFilePreviewGeneratorCompPtr);
	I_REF(iprm::IFileNameParam, m_fileNameCompPtr);
};


} // namespace iqtfpf


#endif // !iqtfpf_CProcessingItemPreviewGeneratorComp_included


