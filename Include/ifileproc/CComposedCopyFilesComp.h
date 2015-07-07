#ifndef ifileproc_CComposedCopyFilesComp_included
#define ifileproc_CComposedCopyFilesComp_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QDir>

// ACF includes
#include "ifileproc/IFileConversion.h"
#include "ilog/TLoggerCompWrap.h"


namespace ifileproc
{


/**
	Composed files copy.
*/
class CComposedCopyFilesComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			virtual public ifileproc::IFileConversion
{
	Q_OBJECT

public:
	typedef ilog::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_CRITICAL = 0xa8760
	};

	I_BEGIN_COMPONENT(CComposedCopyFilesComp);
		I_REGISTER_INTERFACE(ifileproc::IFileConversion);
		I_ASSIGN_MULTI_0(m_fileCopiersCompPtr, "FileCopiers", "Set of copiers will be used for input and output path", true);
	I_END_COMPONENT;

	// reimplemented (ifileproc::IFileConversion)
	virtual int ConvertFiles(
				const QString& inputPath,
				const QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL,
				ibase::IProgressManager* progressManagerPtr = NULL) const;

private:
	I_MULTIREF(ifileproc::IFileConversion, m_fileCopiersCompPtr);
};


} // namespace ifileproc


#endif // !ifileproc_CComposedCopyFilesComp_included


