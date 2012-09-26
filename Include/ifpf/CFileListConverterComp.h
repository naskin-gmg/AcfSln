#ifndef ifpf_CFileListConverterComp_included
#define ifpf_CFileListConverterComp_included


// ACF includes
#include "istd/TPointerVector.h"
#include "iproc/TSyncProcessorCompBase.h"
#include "ibase/IFileListProvider.h"
#include "ibase/IFileConvertCopy.h"

// ACF-Solutions includes
#include "ifpf/IFileNaming.h"


namespace ifpf
{


class CFileListConverterComp: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;

	I_BEGIN_COMPONENT(CFileListConverterComp);
		I_ASSIGN(m_fileConvertCompPtr, "FileCopy", "File copy component used for execution", true, "FileCopy");
		I_ASSIGN(m_outputFileNameCompPtr, "OutputFileNaming", "File naming component for the processing output", true, "OutputFileNaming");
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

private:
	I_REF(ibase::IFileConvertCopy, m_fileConvertCompPtr);
	I_REF(IFileNaming, m_outputFileNameCompPtr);
};


} // namespace ifpf


#endif // !ifpf_CFileListConverterComp_included

