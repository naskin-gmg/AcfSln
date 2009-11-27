#include "ifpf/CHotfolder.h"


namespace ifpf
{


// public methods

// reimplemented (ifpf::IHotfolderInfo)

int CHotfolder::GetFileState(const istd::CString& fileName) const
{
	return 0;
}


int CHotfolder::GetHotfolderState() const
{
	return m_hotfolderState;
}


// reimplemented (ibase::IFileListProvider)

istd::CStringList CHotfolder::GetFileList() const
{
	istd::CStringList files;

	return files;
}


// reimplemented (iser::ISerializable)

bool CHotfolder::Serialize(iser::IArchive& archive)
{
	return true;
}


istd::CStringList CHotfolder::GetFilesForState(int state) const
{
	istd::CStringList files;

	return files;
}


} // namespace ifpf

