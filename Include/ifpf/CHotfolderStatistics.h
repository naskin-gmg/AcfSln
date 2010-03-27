#ifndef ifpf_CHotfolderStatistics_included
#define ifpf_CHotfolderStatistics_included


// STL includes
#include <map>


// ACF includes
#include "imod/TSingleModelObserverBase.h"


// AcfSln includes
#include "ifpf/IHotfolderStatistics.h"
#include "ifpf/IHotfolderProcessingInfo.h"


namespace ifpf
{


/**
	Standard implementation if the IHotfolderStatistics interface.
	Statistics data is calculated and updated by observing the changes in the hotfolder state model.
*/
class CHotfolderStatistics: 
			virtual public ifpf::IHotfolderStatistics,
			public imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingInfo>
{
public:
	typedef imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingInfo> BaseClass;

	CHotfolderStatistics();

	// reimplemented (ifpf::IHotfolderStatistics)
	virtual int GetItemsCount(const istd::CString& directoryPath = istd::CString()) const;
	virtual int GetProcessedCount(const istd::CString& directoryPath = istd::CString()) const;
	virtual int GetErrorsCount(const istd::CString& directoryPath = istd::CString()) const;
	virtual int GetAbortedCount(const istd::CString& directoryPath = istd::CString()) const;

	// reimplemented (imod::TSingleModelObserverBase)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	void ResetStatistics();
	void RebuildStatistics();
	void UpdateStateMaps(int itemState, const istd::CString& directoryPath);
	static istd::CString GetDirectoryPath(const ifpf::IHotfolderProcessingItem& item);

private:
	typedef std::map<istd::CString, int> CounterMap;
	CounterMap m_itemsCount;
	CounterMap m_processedCount;
	CounterMap m_errorsCount;
	CounterMap m_abortedCount;

	typedef std::pair<ifpf::IHotfolderProcessingItem*, int> ProcessingItemState;

	ProcessingItemState m_previousItemState;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderStatistics_included


