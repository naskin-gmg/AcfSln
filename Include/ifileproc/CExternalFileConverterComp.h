#ifndef ifileproc_CExternalFileConverterComp_included
#define ifileproc_CExternalFileConverterComp_included


// Qt includes
#include <QtCore/QProcess>

// ACF includes
#include "ifileproc/IFileConvertCopy.h"
#include "ilog/TLoggerCompWrap.h"

#include "iprm/INameParam.h"
#include "ifile/IFileNameParam.h"


namespace ifileproc
{


/**
	File converter, which uses an external programm to perform the convert action.
	There are several ways to define how the command line parameter can be passed to the underlaying executable process.
	The simplest way is to define the complete command line in text attribute \c DefaultProcessArguments.
	It works only for use cases, where the command line can be statically defined and is known at the component definition time.
	For the situation, where the process arguments can be changed at runtime,
	you can use \c m_additionalArgumentsCompPtr. This is a list of iprm::INameParam objects.
	For integration into the command line, you can use placeholder (variable) in the form $(An),
	where c\ n is the index in the object list of \c additionalArgumentsCompPtr.
	For example, in the command line <MyProcess.exe $(A1)>,
	the $(A1) argument will be replaced by the additionalArgumentsCompPtr[0]->GetName().
	For defining of the command line you can also use some predefined variables. Currently, following variables are supported:
	$(Input) - \c inputPath argument of ConvertFiles method is used.
	$(Output) - \c outputPath argument of ConvertFiles method is used.
	$(OutputDir) - Output directory extracted from \c outputPath argument of ConvertFiles method is used.
*/
class CExternalFileConverterComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			virtual public ifileproc::IFileConvertCopy
{
	Q_OBJECT

public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CExternalFileConverterComp);
		I_REGISTER_INTERFACE(ifileproc::IFileConvertCopy);
		I_ASSIGN(m_executablePathCompPtr, "ExecutablePath", "Path to the application's binary", true, "ExecutablePath");
		I_ASSIGN(m_defaultProcessArgumentsAttrPtr, "DefaultProcessArguments", "Application conversion arguments.\nUse $(Input) to specify the input and $(Output) for output file name", false, "$(Input) $(Output)");
		I_ASSIGN(m_processArgumentsParamsIdAttrPtr, "ProcessArgumentsParamsId", "ID of the command line parameter (given as a INameParam object) in the parameter set", true, "ProcessArgumentsParamsId");
		I_ASSIGN_MULTI_0(m_additionalArgumentsCompPtr, "AdditionalArguments", "Additional command line arguments", false);
	I_END_COMPONENT;

	// reimplemented (ifileproc::IFileConvertCopy)
	virtual bool ConvertFiles(
				const QString& inputPath,
				const QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private Q_SLOTS:
	void OnReadyReadStandardError();
	void OnReadyReadStandardOutput();

private:
	I_REF(ifile::IFileNameParam, m_executablePathCompPtr);
	I_ATTR(QString, m_defaultProcessArgumentsAttrPtr);
	I_ATTR(QByteArray, m_processArgumentsParamsIdAttrPtr);
	I_MULTIREF(iprm::INameParam, m_additionalArgumentsCompPtr);

	mutable QProcess m_conversionProcess;
};


} // namespace ifileproc


#endif // !ifileproc_CExternalFileConverterComp_included


