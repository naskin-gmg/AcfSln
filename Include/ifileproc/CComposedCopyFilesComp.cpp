#include "ifileproc/CComposedCopyFilesComp.h"


// Qt includes
#include <QtCore/QObject>


namespace ifileproc
{


// reimplemented (ifileproc::IFileConversion)

int CComposedCopyFilesComp::ConvertFiles(
			const QString& inputPath,
			QString& outputPath,
			const iprm::IParamsSet* paramsPtr,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	int retVal = iproc::IProcessor::TS_NONE;

	int copiersCount = m_fileCopiersCompPtr.GetCount();
	for (int i = 0; i < copiersCount; ++i){
		ifileproc::IFileConversion* copierPtr = m_fileCopiersCompPtr[i];
		if (copierPtr != NULL){
			retVal = qMax(retVal, copierPtr->ConvertFiles(inputPath, outputPath, paramsPtr));
		}
		else{
			SendErrorMessage(MI_CRITICAL, QObject::tr("File copy provider is not present"));

			return iproc::IProcessor::TS_INVALID;
		}
	}

	return retVal;
}


} // namespace ifileproc


