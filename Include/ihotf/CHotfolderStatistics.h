#ifndef ihotf_CHotfolderStatistics_included
#define ihotf_CHotfolderStatistics_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "imod/TSingleModelObserverBase.h"

// AcfSln includes
#include "ihotf/IHotfolderStatistics.h"
#include "ihotf/IHotfolderProcessingInfo.h"


namespace ihotf
{


/**
	Standard implementation if the IHotfolderStatistics interface.
	Statistics data is calculated and updated by observing the changes in the hotfolder state model.
*/
class CHotfolderStatistics: 
			virtual public ihotf::IHotfolderStatistics,
			public imod::TSingleModelObserverBase<ihotf::IHotfolderProcessingInfo>
{
public:
	typedef imod::TSingleModelObserverBase<ihotf::IHotfolderProcessingInfo> BaseClass;

	CHotfolderStatistics();

	// reimplemented (ihotf::IHotfolderStatistics)
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
	static QString GetDirectoryPath(const ihotf::IHotfolderProcessingItem& item);

private:
	typedef QMap<QString, int> CounterMap;
	CounterMap m_itemsCount;
	CounterMap m_successCount;
	CounterMap m_errorsCount;
	CounterMap m_abortedCount;

	typedef QMap<QString, double> ProcessingTimeMap;
	ProcessingTimeMap m_processingTimeMap;
};


} // namespace ihotf


#endif // !ihotf_CHotfolderStatistics_included


