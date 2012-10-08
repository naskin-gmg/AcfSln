#ifndef ihotf_CHotfolderProcessingInfo_included
#define ihotf_CHotfolderProcessingInfo_included


// Qt includes
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
#include "ihotf/IHotfolderProcessingInfo.h"
#include "ihotf/CHotfolderProcessingItem.h"


namespace ihotf
{


/**
	Implementation of the processing data model of the hotfolder.
*/
class CHotfolderProcessingInfo: virtual public ihotf::IHotfolderProcessingInfo
{
public:
	CHotfolderProcessingInfo();

	/**
		Return \c true if the item for the given input and output path already exists.
		If foundItemPtr not equals \c NULL and an item was found, \c foundItemPtr will be set to the found item.
	*/
	virtual bool ItemExists(const QString& inputFilePath, ihotf::IHotfolderProcessingItem** foundItemPtr = NULL) const;
	virtual bool ItemExists(const ihotf::IHotfolderProcessingItem& processingItem) const;

	// reimplemented (ihotf::IHotfolderProcessingInfo)
	virtual const ihotf::IHotfolderProcessingItem* AddProcessingItem(
				const QString& inputFilePath,
				const QString& outputFilePath = QString());
	virtual void RemoveProcessingItem(ihotf::IHotfolderProcessingItem* fileItemPtr);
	virtual int GetProcessingItemsCount() const;
	virtual ihotf::IHotfolderProcessingItem* GetProcessingItem(int processingItemIndex) const;
	virtual bool IsWorking() const;
	virtual void SetWorking(bool working = true);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	typedef imod::TModelWrap<
				istd::TChangeDelegator<
						ihotf::CHotfolderProcessingItem> > ProcessingItem;
	typedef istd::TPointerVector<ihotf::IHotfolderProcessingItem> FileItems;

	FileItems m_processingItems;

	bool m_isWorking;

	mutable QMutex m_mutex;
};


} // namespace ihotf


#endif // !ihotf_CHotfolderProcessingInfo_included

