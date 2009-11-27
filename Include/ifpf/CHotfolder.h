#ifndef ifpf_CHotfolder_included
#define ifpf_CHotfolder_included


// ACF includes
#include "iser/ISerializable.h"


// AcfSln includes
#include "ifpf/IHotfolder.h"


namespace ifpf
{


/**
	Implementation of the dynamic data model of the hotfolder.
*/
class CHotfolder: public ifpf::IHotfolder, virtual public iser::ISerializable
{
public:
	// reimplemented (ifpf::IHotfolder)
	virtual int GetFileState(const istd::CString& fileName) const;
	virtual int GetHotfolderState() const;

	// reimplemented (ibase::IFileListProvider)
	virtual istd::CStringList GetFileList() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	virtual istd::CStringList GetFilesForState(int state) const;

protected:
	istd::CStringList m_processedFiles;
	istd::CStringList m_abortedFiles;
	istd::CStringList m_waitingFiles;

	int m_hotfolderState;
};


} // namespace ifpf


#endif // !ifpf_CHotfolder_included

