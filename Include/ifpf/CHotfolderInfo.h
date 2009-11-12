#ifndef ifpf_CHotfolderInfo_included
#define ifpf_CHotfolderInfo_included


// ACF includes
#include "ibase/TNamedWrap.h"


// AcfSln includes
#include "ifpf/IHotfolderInfo.h"


namespace ifpf
{


/**
	Implementation of the hotfolder info
*/
class CHotfolderInfo: public ibase::TNamedWrap<ifpf::IHotfolderInfo>
{
public:
	typedef ibase::TNamedWrap<ifpf::IHotfolderInfo> BaseClass;

	// reimplemented (ifpf::IHotfolderInfo)
	virtual int GetFileState(const istd::CString& fileName) const;

	// reimplemented (ibase::IFileListProvider)
	virtual istd::CStringList GetFileList() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	virtual istd::CStringList GetFilesForState(int state) const;

private:
	istd::CStringList m_processedFiles;
	istd::CStringList m_abortedFiles;
	istd::CStringList m_waitingFiles;

};


} // namespace ifpf


#endif // !ifpf_CHotfolderInfo_included

