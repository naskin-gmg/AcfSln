#ifndef ifpf_CHotfolder_included
#define ifpf_CHotfolder_included


// STL includes
#include <map>


// ACF includes
#include "istd/TOptPointerVector.h"
#include "istd/TDelPtr.h"
#include "istd/TSmartPtr.h"
#include "istd/TChangeDelegator.h"

#include "isys/ICriticalSection.h"

#include "imod/TModelWrap.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingInfo.h"
#include "ifpf/CHotfolderProcessingItem.h"
#include "ifpf/CMonitoringSession.h"


namespace ifpf
{


/**
	Implementation of the processing data model of the hotfolder.
*/
class CHotfolder: virtual public ifpf::IHotfolderProcessingInfo
{
public:
	CHotfolder();

	/**
		Return \c true if the item for the given input and output path already exists.
		If foundItemPtr not equals \c NULL and an item was found, \c foundItemPtr will be set to the found item.
	*/
	virtual bool ItemExists(const istd::CString& inputFilePath, const istd::CString& outputFilePath, ifpf::IHotfolderProcessingItem** foundItemPtr = NULL) const;
	virtual bool ItemExists(const ifpf::IHotfolderProcessingItem& processingItem) const;

	// reimplemented (ifpf::IHotfolderProcessingInfo)
	virtual const ifpf::IHotfolderProcessingItem* AddProcessingItem(const istd::CString& inputFilePath, const istd::CString& outputFilePath);
	virtual void RemoveProcessingItem(ifpf::IHotfolderProcessingItem* fileItemPtr);
	virtual int GetProcessingItemsCount() const;
	virtual ifpf::IHotfolderProcessingItem* GetProcessingItem(int processingItemIndex) const;
	virtual bool IsWorking() const;
	virtual void SetWorking(bool working = true);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	typedef imod::TModelWrap<
				istd::TChangeDelegator<
						ifpf::CHotfolderProcessingItem> > ProcessingItem;
	typedef istd::TOptPointerVector<ifpf::IHotfolderProcessingItem> FileItems;

	FileItems m_processingItems;

	bool m_isWorking;

	istd::TSmartPtr<isys::ICriticalSection> m_lockPtr;
};


} // namespace ifpf


#endif // !ifpf_CHotfolder_included

