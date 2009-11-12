#include "ifpf/CHotfolderInfo.h"


namespace ifpf
{


// public methods

// reimplemented (ifpf::IHotfolderInfo)

int CHotfolderInfo::GetFileState(const istd::CString& fileName) const
{
	return 0;
}


// reimplemented (ibase::IFileListProvider)

istd::CStringList CHotfolderInfo::GetFileList() const
{
	istd::CStringList files;

	return files;
}


// reimplemented (iser::ISerializable)

bool CHotfolderInfo::Serialize(iser::IArchive& archive)
{
	return true;
}


istd::CStringList CHotfolderInfo::GetFilesForState(int state) const
{
	istd::CStringList files;

	return files;
}


} // namespace ifpf

