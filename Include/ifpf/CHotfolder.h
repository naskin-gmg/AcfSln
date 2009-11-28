#ifndef ifpf_CHotfolder_included
#define ifpf_CHotfolder_included


// STL includes
#include <map>


// ACF includes
#include "istd/TDelPtr.h"

#include "iser/ISerializable.h"


// AcfSln includes
#include "ifpf/IHotfolder.h"
#include "ifpf/CMonitoringSession.h"


namespace ifpf
{


/**
	Implementation of the dynamic data model of the hotfolder.
*/
class CHotfolder: public ifpf::IHotfolder, virtual public iser::ISerializable
{
public:
	CHotfolder();

	virtual void SetFileState(const istd::CString& fileName, int state);
	virtual void RemoveFile(const istd::CString& fileName);
	virtual void AddFile(const istd::CString& fileName, int state);
	virtual void SetParams(iprm::IParamsSet* paramsSet);

	// reimplemented (ifpf::IHotfolder)
	virtual int GetFileState(const istd::CString& fileName) const;
	virtual bool GetWorking() const;
	virtual void SetWorking(bool working = true);
	virtual iprm::IParamsSet* GetHotfolderParams() const;

	// reimplemented (ibase::IFileListProvider)
	virtual istd::CStringList GetFileList() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	virtual istd::CStringList GetFilesForState(int state) const;

protected:
	struct FileItem
	{
		istd::CString filePath;
		int fileState;
	};

	typedef std::vector<FileItem> FileItems;

	FileItems m_fileItems;

	bool m_isWorking;

	typedef std::map<istd::CString, istd::TDelPtr<ifpf::CMonitoringSession> > MonitoringSessionsMap;
	MonitoringSessionsMap m_monitoringSessionsMap;

	iprm::IParamsSet* m_paramsSetPtr;
};


} // namespace ifpf


#endif // !ifpf_CHotfolder_included

