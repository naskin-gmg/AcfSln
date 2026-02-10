// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iprod_IProductionHistoryController_included
#define iprod_IProductionHistoryController_included


// ACF includes
#include <iprod/IProductionHistory.h>


namespace iprod
{


/**
	Interface for the creation and management of the production history.
*/
class IProductionHistoryController: virtual public iprod::IProductionHistory
{
public:
	/**
		Insert a new part into the history.
	*/
	virtual QByteArray InsertNewProductionPart(
				const QString& productName,
				const QByteArray& productId,
				const QString& serialNumber,
				istd::IInformationProvider::InformationCategory partStatus,
				const QDateTime& productionTime) = 0;

	/**
		Insert a new inspection result.
	*/
	virtual QByteArray InsertNewInspectionResult(
				const QByteArray& productionPartId,
				const QByteArray& inspectionId,
				const QString& inspectionName,
				istd::IInformationProvider::InformationCategory status,
				const QDateTime& resultTime) = 0;

	/**
		Set path to the inspection results file.
	*/
	virtual QByteArray InsertInspectionResultPath(
				const QString& filePath,
				const QByteArray& productionPartId,
				const QByteArray& resultId,
				const QByteArray& objectTypeId) = 0;

	/**
		Set path to the inspection's input file.
	*/
	virtual QByteArray InsertInputObjectPath(
				const QString& filePath,
				const QByteArray& productionPartId,
				const QByteArray& resultId,
				const QByteArray& objectTypeId) = 0;

	/**
		Remove an existing production part from the history list.
	*/
	virtual void RemoveProductionPart(const QByteArray& productionPartId) = 0;
};


} // namespace iprod


#endif // !iprod_IProductionHistoryController_included

