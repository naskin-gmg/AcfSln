#ifndef iprod_CProductionHistoryComp_included
#define iprod_CProductionHistoryComp_included


// ACF includes
#include <ilog/TLoggerCompWrap.h>
#include <ifile/IFileNameParam.h>
#include <iprod/IProductionHistoryController.h>

// Qt includes
#include <QtCore/QReadWriteLock>
#include <QtCore/QFutureWatcher>


namespace iprod
{


class CProductionHistoryComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			virtual public IProductionHistoryController
{
	Q_OBJECT

public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CProductionHistoryComp);
		I_REGISTER_INTERFACE(IProductionHistory);
		I_REGISTER_INTERFACE(IProductionHistoryController);
		I_ASSIGN(m_productionHistoryFolderCompPtr, "RepositoryFolder", "Folder containing production history items", false, "RepositoryFolder");
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Version info", false, "VersionInfo");
		I_ASSIGN(m_doNotLoadHistoryAttrPtr, "DoNotLoadHistory", "Do not load history from disk", false, false);
	I_END_COMPONENT;

	CProductionHistoryComp();

	// reimplemented (IProductionHistory)
	QList<QByteArray> GetPartInfoIds() const;
	PartInfo GetPartInfo(const QByteArray& partId) const;
	ResultInfo GetResultInfo(const QByteArray& partId, const QByteArray& resultId) const;


	// reimplemented (IProductionHistoryController)
	virtual QByteArray InsertNewProductionPart(
				const QString& productName,
				const QByteArray& productId,
				const QString& serialNumber,
				istd::IInformationProvider::InformationCategory,
				const QDateTime& productionTime);
	virtual QByteArray InsertNewInspectionResult(
				const QByteArray& productionPartId,
				const QByteArray& resultId,
				const QString& inspectionName,
				istd::IInformationProvider::InformationCategory status,
				const QDateTime& resultTime);
	virtual QByteArray InsertInspectionResultPath(
				const QString& filePath,
				const QByteArray& productionPartId,
				const QByteArray& resultId,
				const QByteArray& objectTypeId);
	virtual QByteArray InsertInputObjectPath(
				const QString& filePath,
				const QByteArray& productionPartId,
				const QByteArray& resultId,
				const QByteArray& objectTypeId);
	virtual void RemoveProductionPart(const QByteArray& productionPartId);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	typedef QMap<QByteArray, PartInfo> HistoryItems;

private:
	bool SerializePartInfo(iser::IArchive& archive, PartInfo& partInfo) const;
	bool SerializeResults(iser::IArchive& archive, ResultInfoList& resultInfoList) const;
	bool SerializeObjects(iser::IArchive& archive, ObjectInfoList& objectInfoList) const;

	void ReadHistoryItems();

	void SaveRepositoryItem(const PartInfo& partInfo) const;
	QString GetItemPath(const PartInfo& partInfo) const;
	QByteArray InsertResultObject(const QString& filePath, const QByteArray& partId, const QByteArray& resultId, const QByteArray& objectTypeId, bool input);

private slots:
	void OnHistoryReadFinished();

private:
	HistoryItems m_historyItems;
	HistoryItems m_historyItemsToLoad;

	mutable QReadWriteLock m_historyItemsLock;

	QFutureWatcher<void> m_historyReaderWatcher;

	const istd::IChangeable::ChangeSet m_newObjectChangeSet;

	I_REF(ifile::IFileNameParam, m_productionHistoryFolderCompPtr);
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
	I_ATTR(bool, m_doNotLoadHistoryAttrPtr);
};


} // namespace iprod


#endif // !iprod_CProductionHistoryComp_included


