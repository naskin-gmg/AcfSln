#ifndef ifileproc_CCopyAppComp_included
#define ifileproc_CCopyAppComp_included


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include "istd/IChangeable.h"
#include "ibase/IApplication.h"
#include "ifile/IFilePersistence.h"
#include "ilog/TLoggerCompWrap.h"

// ACF-Solutions include
#include "ifileproc/IFileConversion.h"


namespace ifileproc
{


/**
	General file transformation application.
	This implementation uses some slave ifileproc::IFileConversion implementation to provide copy operation.
	It implements ibase::IApplication and can be used as standalone application.
*/
class CCopyAppComp:
			public ilog::CLoggerComponentBase,
			virtual public ibase::IApplication
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCopyAppComp);
		I_REGISTER_INTERFACE(ibase::IApplication);

		I_ASSIGN(m_fileCopyCompPtr, "FileCopy", "Provide copy of single file", true, "FileCopy");
		I_ASSIGN(m_needExplicitInputAttrPtr, "NeedExplicitInput", "Indicate if explicit input file with -input is needed, otherwise first argument will be taken", true, false);
	I_END_COMPONENT;

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual QString GetHelpText() const;
	virtual QStringList GetApplicationArguments() const;

private:
	I_REF(IFileConversion, m_fileCopyCompPtr);
	I_ATTR(bool, m_needExplicitInputAttrPtr);

	QStringList m_applicationArguments;
};


} // namespace ifileproc


#endif // !ifileproc_CCopyAppComp_included


