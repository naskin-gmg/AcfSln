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
	CHotfolder();

	// reimplemented (ifpf::IHotfolder)
	virtual const ifpf::IHotfolderProcessingItem* AddProcessingItem(const istd::CString& inputFilePath, const istd::CString& outputFilePath);
	virtual void RemoveProcessingItem(ifpf::IHotfolderProcessingItem* fileItemPtr);
	virtual ifpf::IHotfolderProcessingItem* GetNextProcessingFile() const;
	virtual int GetProcessingItemsCount() const;
	virtual ifpf::IHotfolderProcessingItem* GetProcessingItem(int processingItemIndex) const;
	virtual bool IsWorking() const;
	virtual void SetWorking(bool working = true);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	typedef imod::TModelWrap<ifpf::CHotfolderProcessingItem> ProcessingItem;
	typedef istd::TOptPointerVector<ifpf::IHotfolderProcessingItem> FileItems;

	FileItems m_processingItems;

	bool m_isWorking;

	iprm::IParamsSet* m_paramsSetPtr;
};


} // namespace ifpf


#endif // !ifpf_CHotfolder_included

