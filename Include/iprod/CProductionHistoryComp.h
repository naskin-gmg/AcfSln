#ifndef iprod_CProductionHistoryComp_included
#define iprod_CProductionHistoryComp_included


// ACF includes
#include <imod/TModelWrap.h>
#include <icomp/CComponentBase.h>
#include <iprod/IProductionHistoryController.h>


namespace iprod
{


class CProductionHistoryComp:
			public icomp::CComponentBase,
			virtual public IProductionHistoryController,
			virtual public iser::ISerializable
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CProductionHistoryComp);
		I_REGISTER_INTERFACE(IProductionHistory);
		I_REGISTER_INTERFACE(IProductionHistoryController);
		I_REGISTER_INTERFACE(iser::ISerializable);
	I_END_COMPONENT;

	CProductionHistoryComp();

	// reimplemented (IProductionHistory)
	virtual const iprm::IOptionsList& GetPartsInfoList() const;
	virtual PartInfo GetPartInfo(const QByteArray& productionPartId) const;
	virtual const iprm::IOptionsList& GetResultInfoList(const QByteArray& productionPartId) const;
	virtual ObjectInfoList GetInputObjectInfoList(const QByteArray& productionPartId, const QByteArray& resultId) const;
	virtual ObjectInfoList GetResultObjectInfoList(const QByteArray& productionPartId, const QByteArray& resultId) const;
	virtual ProcessingInfo GetPartProcessingInfo(const QByteArray& productionPartId) const;
	virtual ProcessingInfo GetResultInfo(const QByteArray& productionPartId, const QByteArray& resultId) const;
	virtual QString GetInspectionResultsFilePath(
				const QByteArray& productionPartId,
				const QByteArray& resultId,
				const QByteArray& inputObjectId = QByteArray()) const;


	// reimplemented (IProductionHistoryController)
	virtual QByteArray InsertNewProductionPart(
				const QString& productName,
				const QByteArray& productId,
				const QString& serialNumber = QString(),
				istd::IInformationProvider::InformationCategory = istd::IInformationProvider::IC_NONE,
				const QDateTime& productionTime = QDateTime());
	virtual QByteArray InsertNewInspectionResult(
				const QByteArray& productionPartId,
				const QByteArray& resultId,
				const QString& inspectionName,
				istd::IInformationProvider::InformationCategory status = istd::IInformationProvider::IC_NONE);
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

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	struct ResultInfo
	{
		ResultInfo()
			:status(istd::IInformationProvider::IC_NONE)
		{
			uuid = QUuid::createUuid().toByteArray();
		}

		QByteArray uuid;
		QString name;
		QByteArray inspectionId;
		ObjectInfoList inputObjects;
		ObjectInfoList outputObjects;
		istd::IInformationProvider::InformationCategory status;
	};

	typedef QList<ResultInfo> ResultInfoList;

	struct HistoryItem: virtual public iprm::IOptionsList
	{
		HistoryItem();

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

private:
	typedef imod::TModelWrap<PartList> PartListModel;
	PartListModel m_partList;

	HistoryItems m_historyItems;
};


} // namespace iprod


#endif // !iprod_CProductionHistoryComp_included


