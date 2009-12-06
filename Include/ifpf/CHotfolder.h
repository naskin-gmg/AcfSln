#ifndef ifpf_CHotfolder_included
#define ifpf_CHotfolder_included


// STL includes
#include <map>


// ACF includes
#include "istd/TOptPointerVector.h"
#include "istd/TDelPtr.h"
#include "imod/TModelWrap.h"


// AcfSln includes
#include "ifpf/IHotfolder.h"
#include "ifpf/CHotfolderProcessingItem.h"
#include "ifpf/CMonitoringSession.h"


namespace ifpf
{


/**
	Implementation of the dynamic data model of the hotfolder.
*/
class CHotfolder: public ifpf::IHotfolder
{
public:
	typedef imod::TModelWrap<ifpf::CHotfolderProcessingItem> ProcessingItem;

	CHotfolder();

	virtual void RemoveProcessingItem(ProcessingItem* fileItemPtr);
	virtual void AddProcessingItem(ProcessingItem* fileItemPtr, bool releaseFlag = true);
	virtual void SetParams(iprm::IParamsSet* paramsSet);
	virtual const ifpf::IHotfolderProcessingItem* GetNextProcessingFile() const;
	virtual void UpdateProcessingState(const ifpf::IHotfolderProcessingItem* processingItemPtr, int processingState);

	// reimplemented (ifpf::IHotfolder)
	virtual int GetProcessingItemsCount() const;
	virtual IHotfolderProcessingItem* GetProcessingItem(int processingItemIndex) const;
	virtual bool IsWorking() const;
	virtual void SetWorking(bool working = true);
	virtual iprm::IParamsSet* GetHotfolderParams() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	typedef istd::TOptPointerVector<ProcessingItem> FileItems;

	FileItems m_processingItems;

	bool m_isWorking;

	iprm::IParamsSet* m_paramsSetPtr;
};


} // namespace ifpf


#endif // !ifpf_CHotfolder_included

