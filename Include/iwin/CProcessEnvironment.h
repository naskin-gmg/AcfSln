#ifndef iwin_CProcessEnvironment_included
#define iwin_CProcessEnvironment_included


#include "isys/IProcessEnvironment.h"


namespace iwin
{


/**
	Windows based implementation of interface \c isys::IProcessEnvironment.

	\ingroup Windows
*/
class CProcessEnvironment: virtual public isys::IProcessEnvironment
{
public:
	// reimplemented (isys::IProcessEnvironment)
	virtual int GetMainThreadId() const;
	virtual void Sleep(double seconds);
	virtual istd::CString GetTempDirPath() const;
	virtual istd::CString GetWorkingDirectory() const;
	virtual istd::CStringList GetApplicationArguments() const;
	virtual istd::CString GetModulePath(bool useApplicationModule = false, bool onlyDirectory = false) const;
	virtual EnvironmentVariables GetEnvironmentVariables() const;
	virtual void SetEnvironmentVariableValue(const istd::CString& variableName, const istd::CString& value);
};


} // namespace iwin


#endif // !iwin_CProcessEnvironment_included
