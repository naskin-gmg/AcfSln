#ifndef ifpf_CFileNamingParamsComp_included
#define ifpf_CFileNamingParamsComp_included


// ACF includes
#include "icomp/CComponentBase.h"


// AcfSln includes
#include "ifpf/IFileNamingParams.h"


namespace ifpf
{


class CFileNamingParamsComp: public icomp::CComponentBase, virtual public ifpf::IFileNamingParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	CFileNamingParamsComp();

	I_BEGIN_COMPONENT(ifpf::CFileNamingParamsComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ifpf::IFileNamingParams);
	I_END_COMPONENT();

	// reimplemented (ifpf::IFileNamingParams)
	virtual int GetRenamingMode() const;
	virtual void SetRenamingMode(int renamingMode);
	virtual istd::CString GetPrefix() const;
	virtual void SetPrefix(const istd::CString& prefix);
	virtual istd::CString GetSuffix() const;
	virtual void SetSuffix(const istd::CString& suffix);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	int m_renamingMode;
	istd::CString m_suffix;
	istd::CString m_prefix;
};


} // namespace ifpf


#endif // !ifpf_CFileNamingParamsComp_included


