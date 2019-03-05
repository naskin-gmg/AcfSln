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
	struct ProcessingInfo
	{
		istd::IInformationProvider::InformationCategory status;
		QDateTime time;
	};

	/**
		Part information
	*/
	struct PartInfo
	{
		QString serialNumber;
		QString name;
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
		Get the list of the produced parts.
	*/
	virtual const iprm::IOptionsList& GetPartsInfoList() const = 0;

	/**
		Get the information about the processed part.
	*/
	virtual PartInfo GetPartInfo(const QByteArray& productionPartId) const = 0;

	/**
		Get the list of the executed inspections for the given part-ID.
	*/
	virtual const iprm::IOptionsList& GetResultInfoList(const QByteArray& productionPartId) const = 0;

	/**
		Get the list of the input objects for the given part-ID and inspection-ID.
	*/
	virtual ObjectInfoList GetInputObjectInfoList(const QByteArray& productionPartId, const QByteArray& resultId) const = 0;

	/**
		Get the list of the result objects for the given part-ID and inspection-ID.
	*/
	virtual ObjectInfoList GetResultObjectInfoList(const QByteArray& productionPartId, const QByteArray& resultId) const = 0;

	/**
		Get production info for the given part.
	*/
	virtual ProcessingInfo GetPartProcessingInfo(const QByteArray& productionPartId) const = 0;

	/**
		Get processing info of the result.
	*/
	virtual ProcessingInfo GetResultInfo(const QByteArray& productionPartId, const QByteArray& resultId) const = 0;

	/**
		Get the file path to the inspection-related output.
	*/
	virtual QString GetInspectionResultsFilePath(
				const QByteArray& productionPartId,
				const QByteArray& resultId,
				const QByteArray& outputObjectId) const = 0;
};


} // namespace iprod


#endif // !iprod_IProductionHistory_included

