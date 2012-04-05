#ifndef ifpf_CHotfolderProcessingInfo_included
#define ifpf_CHotfolderProcessingInfo_included


// STL includes
#include <QtCore/QMap>

// Qt includes
#include <QtCore/QMutex>

// ACF includes
#include "istd/TOptPointerVector.h"
#include "istd/TDelPtr.h"
#include "istd/TSmartPtr.h"
#include "istd/TChangeDelegator.h"
#include "imod/TModelWrap.h"

// AcfSln includes
#include "ifpf/IHotfolderProcessingInfo.h"
#include "ifpf/CHotfolderProcessingItem.h"


namespace ifpf
{


/**
	Implementation of the processing data model of the hotfolder.
*/
class CHotfolderProcessingInfo: virtual public ifpf::IHotfolderProcessingInfo
{
public:
	CHotfolderProcessingInfo();

	/**
		Return \c true if the item for the given input and output path already exists.
		If foundItemPtr not equals \c NULL and an item was found, \c foundItemPtr will be set to the found item.
	*/
	virtual bool ItemExists(const QString& inputFilePath, ifpf::IHotfolderProcessingItem** foundItemPtr = NULL) const;
	virtual bool ItemExists(const ifpf::IHotfolderProcessingItem& processingItem) const;

	// reimplemented (ifpf::IHotfolderProcessingInfo)
	virtual const ifpf::IHotfolderProcessingItem* AddProcessingItem(
				const QString& inputFilePath,
				const QString& outputFilePath = QString());
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
	typedef istd::TPointerVector<ifpf::IHotfolderProcessingItem> FileItems;

	FileItems m_processingItems;

	bool m_isWorking;

	mutable QMutex m_mutex;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderProcessingInfo_included

