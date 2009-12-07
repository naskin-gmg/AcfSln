#ifndef iqtfpf_CFileNamingComp_included
#define iqtfpf_CFileNamingComp_included


// ACF includes
#include "ibase/TLoggerCompWrap.h"


// AcfSln includes
#include "ifpf/IFileNaming.h"


namespace iqtfpf
{


/**
	Component for renaming strategy for the otuput
*/
class CFileNamingComp:
			public ibase::CLoggerComponentBase,
			virtual public ifpf::IFileNaming
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileNamingComp);
		I_REGISTER_INTERFACE(ifpf::IFileNaming);
	I_END_COMPONENT();

	// reimplemented (ifpf::IFileNaming)
	virtual istd::CString GetFilePath(const istd::CString& fileName, const istd::CString& directoryPath) const;
};


} // namespace iqtfpf


#endif // !iqtfpf_CFileNamingComp_included

