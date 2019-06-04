#ifndef iprod_IProductionHistory_included
#define iprod_IProductionHistory_included


// Qt includes
#include <QtCore/QUuid>

// ACF includes
#include <iprm/IOptionsList.h>
#include <ilog/IMessageContainer.h>


namespace iprod
{


/**
	Interface for describing the chronological review of the production process.
	Production history is a container of all data related to the executed inspections for a produced part.
*/
class IProductionHistory: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_NEW_OBJECT = 0x123456
	};

	struct ProcessingInfo
	{
		ProcessingInfo()
			:status(istd::IInformationProvider::IC_NONE),
			time(0)
		{
		}

		istd::IInformationProvider::InformationCategory status;
		qint64 time; // seconds since epoch
	};

	/**
		Description of the data object used as input or as output of the inspection.
	*/
	struct ObjectInfo
	{
		ObjectInfo()
		{
			uuid = QUuid::createUuid().toByteArray();
		}

		/**
			Unique identifier.
		*/
		QByteArray uuid;

		/**
			Path to the object's data.
		*/
		QString filePath;

		/**
			Type-ID of the obect. Used for factorization.
		*/
		QByteArray typeId;
	};

	typedef QList<ObjectInfo> ObjectInfoList;

	/**
		Description of a single inspection result.
	*/
	struct ResultInfo
	{
		ResultInfo()
		{
			uuid = QUuid::createUuid().toByteArray();
		}

		QByteArray uuid;
		QString name;
		QByteArray inspectionId;
		ObjectInfoList inputObjects;
		ObjectInfoList outputObjects;
		ProcessingInfo processingInfo;
	};

	typedef QList<ResultInfo> ResultInfoList;

	/**
		Part information.
	*/
	struct PartInfo
	{
		PartInfo()
		{
			uuid = QUuid::createUuid().toByteArray();
		}

		QByteArray uuid;
		QString serialNumber;
		QString productName;
		QByteArray productId;
		ProcessingInfo processingInfo;
		ResultInfoList results;
	};

	/**
		Get the produced parts identifiers list.
	*/
	virtual QByteArrayList GetPartInfoIds() const = 0;

	/**
		Get processed part information.
	*/
	virtual PartInfo GetPartInfo(const QByteArray& productionPartId) const = 0;

	/**
		Get processed result information.
	*/
	virtual ResultInfo GetResultInfo(const QByteArray& productionPartId, const QByteArray& resultId) const = 0;
};


} // namespace iprod


#endif // !iprod_IProductionHistory_included

