#ifndef ifpf_CHotfolder_included
#define ifpf_CHotfolder_included


// STL includes
#include <map>


// ACF includes
#include "istd/TOptPointerVector.h"
#include "istd/TDelPtr.h"

#include "iser/ISerializable.h"


// AcfSln includes
#include "ifpf/IHotfolder.h"
#include "ifpf/IHotfolderProcessingItem.h"
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

	virtual void RemoveFile(ifpf::IHotfolderProcessingItem* fileItemPtr);
	virtual void AddFile(ifpf::IHotfolderProcessingItem* fileItemPtr, bool releaseFlag = true);
	virtual void SetParams(iprm::IParamsSet* paramsSet);
	virtual const ifpf::IHotfolderProcessingItem* GetNextProcessingFile() const;

	// reimplemented (ifpf::IHotfolder)
	virtual int GetProcessingItemsCount() const;
	virtual IHotfolderProcessingItem* GetProcessingItem(int processingItemIndex) const;
	virtual bool IsWorking() const;
	virtual void SetWorking(bool working = true);
	virtual iprm::IParamsSet* GetHotfolderParams() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	typedef istd::TOptPointerVector<ifpf::IHotfolderProcessingItem> FileItems;

	FileItems m_fileItems;

	bool m_isWorking;

	typedef std::map<istd::CString, istd::TDelPtr<ifpf::CMonitoringSession> > MonitoringSessionsMap;
	MonitoringSessionsMap m_monitoringSessionsMap;

	iprm::IParamsSet* m_paramsSetPtr;
};


} // namespace ifpf


#endif // !ifpf_CHotfolder_included

