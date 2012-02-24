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
	virtual int GetItemsCount(const QString& directoryPath = QString()) const;
	virtual int GetSuccessCount(const QString& directoryPath = QString()) const;
	virtual int GetErrorsCount(const QString& directoryPath = QString()) const;
	virtual int GetAbortedCount(const QString& directoryPath = QString()) const;
	virtual double GetProcessingTime(const QString& directoryPath = QString()) const;

	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	void ResetStatistics();
	void RebuildStatistics();
	void UpdateStateMaps(int itemState, const QString& directoryPath);
	static QString GetDirectoryPath(const ifpf::IHotfolderProcessingItem& item);

private:
	typedef std::map<QString, int> CounterMap;
	CounterMap m_itemsCount;
	CounterMap m_successCount;
	CounterMap m_errorsCount;
	CounterMap m_abortedCount;

	typedef std::map<QString, double> ProcessingTimeMap;
	ProcessingTimeMap m_processingTimeMap;
};


} // namespace ifpf


#endif // !ifpf_CHotfolderStatistics_included


