#ifndef iprod_CProductionHistoryComp_included
#define iprod_CProductionHistoryComp_included


// ACF includes
#include <imod/TModelWrap.h>
#include <ilog/TLoggerCompWrap.h>
#include <ifile/IFileNameParam.h>
#include <iprod/IProductionHistoryController.h>


namespace iprod
{


class CProductionHistoryComp:
			public ilog::CLoggerComponentBase,
			virtual public IProductionHistoryController
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CProductionHistoryComp);
		I_REGISTER_INTERFACE(IProductionHistory);
		I_REGISTER_INTERFACE(IProductionHistoryController);
		I_ASSIGN(m_productionHistoryFolderCompPtr, "RepositoryFolder", "Folder containing production history items", false, "RepositoryFolder");
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Version info", false, "VersionInfo");
	I_END_COMPONENT;

	CProductionHistoryComp();

	// reimplemented (IProductionHistory)
	virtual const iprm::IOptionsList& GetPartsInfoList() const;
	virtual PartInfo GetPartInfo(const QByteArray& productionPartId) const;
	virtual const iprm::IOptionsList& GetResultInfoList(const QByteArray& productionPartId) const;
	virtual ResultInfo GetResultInfo(const QByteArray& productionPartId, const QByteArray& resultId) const;

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
	typedef QList<ResultInfo> ResultInfoList;

	struct HistoryItem: virtual public iprm::IOptionsList
	{
		HistoryItem();

		bool operator < (const HistoryItem& other) const;

		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;
		virtual bool IsOptionEnabled(int index) const;

		QString productName;
		QString serialNumber;
		QByteArray productId;
		QByteArray uuid;
		QDateTime timestamp;
		istd::IInformationProvider::InformationCategory status;
		ResultInfoList resultInfoList;
	};

	typedef QList<HistoryItem> HistoryItems;

	/**
		List of produced parts.
	*/
	class PartList: virtual public iprm::IOptionsList
	{
	public:
		PartList();
		void SetParent(const CProductionHistoryComp* parentPtr);

		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;
		virtual bool IsOptionEnabled(int index) const;

	private:
		const CProductionHistoryComp* m_parentPtr;
	};

private:
	bool SerializeResultInfoList(iser::IArchive& archive, ResultInfoList& resultInfoList) const;
	bool SerializeObjectList(iser::IArchive& archive, ObjectInfoList& objectInfoList) const;
	bool SerializeHistoryItem(iser::IArchive& archive, HistoryItem& historyItem) const;
	void ReadHistoryItems();
	void SaveRepositoryItem(const HistoryItem& historyItem) const;
	QString GetItemPath(const HistoryItem& historyItem) const;

private:
	typedef imod::TModelWrap<PartList> PartListModel;
	PartListModel m_partList;

	HistoryItems m_historyItems;

	const istd::IChangeable::ChangeSet m_newObjectChangeSet;

	I_REF(ifile::IFileNameParam, m_productionHistoryFolderCompPtr);
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
};


} // namespace iprod


#endif // !iprod_CProductionHistoryComp_included


