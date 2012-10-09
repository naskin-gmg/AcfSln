#ifndef ifileproc_CComposedCopyFilesComp_included
#define ifileproc_CComposedCopyFilesComp_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QDir>

// ACF includes
#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"


namespace ifileproc
{


/**
	Composed files copy.
*/
class CComposedCopyFilesComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			virtual public ibase::IFileConvertCopy
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_CRITICAL = 0xa8760
	};

	I_BEGIN_COMPONENT(CComposedCopyFilesComp);
		I_REGISTER_INTERFACE(ibase::IFileConvertCopy);
		I_ASSIGN_MULTI_0(m_fileCopiersCompPtr, "FileCopiers", "Set of copiers will be used for input and output path", true);
	I_END_COMPONENT;

	// reimplemented (ibase::IFileConvertCopy)
	virtual bool ConvertFiles(
				const QString& inputPath,
				const QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

private:
	I_MULTIREF(ibase::IFileConvertCopy, m_fileCopiersCompPtr);
};


} // namespace ifileproc


#endif // !ifileproc_CComposedCopyFilesComp_included


