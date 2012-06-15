#ifndef iqtfpf_CHotfolderProcessingComp_included
#define iqtfpf_CHotfolderProcessingComp_included


// Qt includes
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>

// ACF includes
#include "istd/TPointerVector.h"
#include "imod/IModel.h"
#include "imod/CSingleModelObserverBase.h"
#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

// AcfSln includes
#include "ifpf/IHotfolderProcessingInfo.h"
#include "ifpf/IHotfolderTaskManager.h"


namespace iqtfpf
{


/**
	Hotfolder representation.
*/
class CHotfolderProcessingComp: public QObject, public ibase::CLoggerComponentBase
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CHotfolderProcessingComp);
		I_ASSIGN(m_fileConvertCompPtr, "FileConverter", "File converter", true, "FileConverter");
		I_ASSIGN(m_hotfolderProcessingInfoCompPtr, "HotfolderProcessingInfo", "State data model of the hotfolder", true, "HotfolderProcessingInfo");
		I_ASSIGN_TO(m_hotfolderProcessingModelCompPtr, m_hotfolderProcessingInfoCompPtr, true);
		I_ASSIGN(m_taskManagerCompPtr, "TaskManager", "Hotfolder task manager", true, "TaskManager");
	I_END_COMPONENT;

	CHotfolderProcessingComp();

protected:
	/**
		Implementation of processing thread.
	*/
	class ItemProcessor: public QThread
	{
	public:
		typedef QThread BaseClass;

		ItemProcessor(
					CHotfolderProcessingComp& parent,
					const QString& inputFilePath,
					const QString& outputFilePath,
					const QByteArray& itemUuid);

		int GetProcessingState() const;
		QByteArray GetItemUuid() const;
		QDateTime GetStartTime() const;
		double GetProcessingTime() const;
		void Cancel();

	protected:
		// reimplemented (QThread)
		virtual void run();

	private:
		CHotfolderProcessingComp& m_parent;
		QString m_inputFilePath;
		QString m_outputFilePath;
		QByteArray m_itemUuid;
		int m_processingState;
		double m_processingTime;
		QDateTime m_startTime;
	};

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected Q_SLOTS:
	void OnProcessingTimer();
	void OnProcessingItemFinished(const ItemProcessor& processor);

private:
	/**
		Cancel processing item
	*/
	void OnCancelProcessingItem(const ifpf::IHotfolderProcessingItem* processingItemPtr);

	/**
		Cancel all items.
	*/
	void CancelAllProcessingItems();

	/**
		Get procesing item from its UUID.
	*/
	ifpf::IHotfolderProcessingItem* GetItemFromId(const QByteArray& itemUuid) const;

private:
	/**
		Class for observation of the changes in the hotfolder state model
	*/
	class StateObserver: public imod::CSingleModelObserverBase
	{
	public:
		typedef imod::CSingleModelObserverBase BaseClass;

		StateObserver(CHotfolderProcessingComp& parent);

		// reimplemented (imod::IObserver)
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	private:
		CHotfolderProcessingComp& m_parent;
	};

private:
	I_REF(ibase::IFileConvertCopy, m_fileConvertCompPtr);
	I_REF(ifpf::IHotfolderProcessingInfo, m_hotfolderProcessingInfoCompPtr);
	I_REF(imod::IModel, m_hotfolderProcessingModelCompPtr);
	I_REF(ifpf::IHotfolderTaskManager, m_taskManagerCompPtr);

	StateObserver m_stateObserver;

	QTimer m_processingTimer;

	typedef istd::TPointerVector<ItemProcessor> PendingProcessors;
	PendingProcessors m_pendingProcessors;

	bool m_isInitialized;
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderProcessingComp_included

