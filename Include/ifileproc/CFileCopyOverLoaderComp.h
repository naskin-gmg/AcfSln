#ifndef ifileproc_CFileCopyOverLoaderComp_included
#define ifileproc_CFileCopyOverLoaderComp_included


#include "istd/IChangeable.h"

#include "ifile/IFilePersistence.h"

#include "ifileproc/IFileConversion.h"
#include "ilog/TLoggerCompWrap.h"


namespace ifileproc
{


/**
	General file copy or transformation.
	This implementation uses general file loader, saver and any object instance.
*/
class CFileCopyOverLoaderComp:
			public ilog::CLoggerComponentBase,
			virtual public IFileConversion
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileCopyOverLoaderComp);
		I_REGISTER_INTERFACE(IFileConversion);

		I_ASSIGN(m_objectCompPtr, "Object", "Object used as representation of copied data", true, "Object");
		I_ASSIGN(m_inputLoaderCompPtr, "InputLoader", "input file loader", true, "InputLoader");
		I_ASSIGN(m_outputLoaderCompPtr, "OutputLoader", "output file loader", true, "OutputLoader");
	I_END_COMPONENT;

	// reimplemented (ifileproc::IFileConversion)
	virtual bool ConvertFiles(
				const QString& inputPath,
				const QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL,
				ibase::IProgressManager* progressManagerPtr = NULL) const;

private:
	I_REF(istd::IChangeable, m_objectCompPtr);
	I_REF(ifile::IFilePersistence, m_inputLoaderCompPtr);
	I_REF(ifile::IFilePersistence, m_outputLoaderCompPtr);
};


} // namespace ifileproc


#endif // !ifileproc_CFileCopyOverLoaderComp_included


