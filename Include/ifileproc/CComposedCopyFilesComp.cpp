#include "ifileproc/CComposedCopyFilesComp.h"


// Qt includes
#include <QtCore/QRegExp>


// ACF includes
#include "istd/CSystem.h"


namespace ifileproc
{


// reimplemented (ibase::IFileConvertCopy)

bool CComposedCopyFilesComp::ConvertFiles(
			const QString& inputPath,
			const QString& outputPath,
			const iprm::IParamsSet* paramsPtr) const
{
	bool retVal = true;

	int copiersCount = m_fileCopiersCompPtr.GetCount();
	for (int i = 0; i < copiersCount; ++i){
		ibase::IFileConvertCopy* copierPtr = m_fileCopiersCompPtr[i];
		if (copierPtr != NULL){
			retVal = copierPtr->ConvertFiles(inputPath, outputPath, paramsPtr) && retVal;
		}
		else{
			SendErrorMessage(MI_CRITICAL, tr("File copy provider is not present"));

			return false;
		}
	}

	return retVal;
}


} // namespace ifileproc


