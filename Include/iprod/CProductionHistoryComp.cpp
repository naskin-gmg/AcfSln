#include <iprod/CProductionHistoryComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/CPrimitiveTypesSerializer.h>
#include <iprm/COptionsManager.h>


namespace iprod
{


static const iser::CArchiveTag s_partsTag("Items", "List of history items", iser::CArchiveTag::TT_MULTIPLE);
static const iser::CArchiveTag s_partTag("Item", "Single item", iser::CArchiveTag::TT_GROUP);
static const iser::CArchiveTag s_partUuidTag("Uuid", "UUID of the part", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_partSerialTag("SerialNuber", "Serial number of the part", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_partNameTag("Name", "Name of the part", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_partTimeStampTag("Time", "Time of the part production", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_partStatusTag("Status", "Status of the part production", iser::CArchiveTag::TT_LEAF);


// public methods

CProductionHistoryComp::CProductionHistoryComp()
{
	m_partList.SetParent(this);
}


// reimplemented (IProductionHistory)

const iprm::IOptionsList& CProductionHistoryComp::GetPartsInfoList() const
{
	return m_partList;
}


const iprm::IOptionsList& CProductionHistoryComp::GetResultInfoList(const QByteArray& productionPartId) const
{
	static iprm::COptionsManager empty;

	for (const HistoryItem& item : m_historyItems){
		if (item.uuid == productionPartId){
			return item;
		}
	}

	return empty;
}


IProductionHistory::PartInfo CProductionHistoryComp::GetPartInfo(const QByteArray& productionPartId) const
{
	PartInfo retVal;

	for (const HistoryItem& item : m_historyItems){
		if (item.uuid == productionPartId){
			retVal.name = item.name;
			retVal.serialNumber = item.serialNumber;

			break;
		}
	}

	return retVal;
}


IProductionHistory::ObjectInfoList CProductionHistoryComp::GetInputObjectInfoList(const QByteArray& productionPartId, const QByteArray& resultId) const
{
	for (const HistoryItem& item : m_historyItems){
		if (item.uuid == productionPartId){
			for (const ResultInfo& result : item.resultInfoList){
				if (result.uuid == resultId){
					return result.inputObjects;
				}
			}
		}
	}

	return ObjectInfoList();
}


IProductionHistory::ObjectInfoList CProductionHistoryComp::GetResultObjectInfoList(const QByteArray & productionPartId, const QByteArray & resultId) const
{
	for (const HistoryItem& item : m_historyItems){
		if (item.uuid == productionPartId){
			for (const ResultInfo& result : item.resultInfoList){
				if (result.uuid == resultId){
					return result.outputObjects;
				}
			}
		}
	}

	return ObjectInfoList();
}


IProductionHistory::ProcessingInfo CProductionHistoryComp::GetPartProcessingInfo(const QByteArray& productionPartId) const
{
	ProcessingInfo retVal;

	for (const HistoryItem& item : m_historyItems){
		if (item.uuid == productionPartId){
			retVal.status = item.status;
			retVal.time = item.timestamp;
			break;
		}
	}

	return retVal;
}


IProductionHistory::ProcessingInfo CProductionHistoryComp::GetResultInfo(const QByteArray& productionPartId, const QByteArray& resultId) const
{
	ProcessingInfo retVal;

	for (const HistoryItem& item : m_historyItems){
		if (item.uuid == productionPartId){
			for (const ResultInfo& result : item.resultInfoList){
				if (result.uuid == resultId){
					retVal.status = result.status;
					retVal.time = item.timestamp;

					return retVal;
				}
			}
		}
	}

	return retVal;
}


QString CProductionHistoryComp::GetInspectionResultsFilePath(
			const QByteArray& productionPartId,
			const QByteArray& resultId,
			const QByteArray& outputObjectId) const
{
	for (const HistoryItem& item : m_historyItems){
		if (item.uuid == productionPartId){
			for (const ResultInfo& resultInfo : item.resultInfoList){
				if (resultInfo.uuid == resultId){
					if (!outputObjectId.isEmpty()){
						for (const ObjectInfo& outputObject : resultInfo.outputObjects){
							if (outputObject.uuid == outputObjectId){
								return outputObject.filePath;
							}
						}
					}
					else if(!resultInfo.outputObjects.isEmpty()){
						return resultInfo.outputObjects[0].filePath;
					}
				}
			}
		}
	}

	return QString();
}


QByteArray CProductionHistoryComp::InsertNewProductionPart(
			const QString& partName,
			const QString& serialNumber,
			istd::IInformationProvider::InformationCategory status,
			const QDateTime& productionTime)
{
	istd::CChangeNotifier changeNotifier(this);
	
	HistoryItem newItem;
	newItem.name = partName;
	newItem.serialNumber = serialNumber;
	newItem.timestamp = productionTime.isValid() ? productionTime : QDateTime::currentDateTime();
	newItem.status = status;

	m_historyItems.push_back(newItem);
	
	return newItem.uuid;
}


QByteArray CProductionHistoryComp::InsertNewInspectionResult(
			const QByteArray& productionPartId,
			const QByteArray& inspectionId,
			const QString & inspectionName,
			istd::IInformationProvider::InformationCategory status)
{
	for (HistoryItem& item : m_historyItems){
		if (item.uuid == productionPartId){
			istd::CChangeNotifier changeNotifier(this);

			ResultInfo newResult;

			newResult.inspectionId = inspectionId;
			newResult.name = inspectionName;
			newResult.status = status;
			
			item.resultInfoList.push_back(newResult);

			return newResult.uuid;
		}
	}

	return QByteArray();
}


QByteArray CProductionHistoryComp::InsertInspectionResultPath(
			const QString& filePath,
			const QByteArray& productionPartId,
			const QByteArray& resultId,
			const QByteArray& objectTypeId)
{
	for (HistoryItem& item : m_historyItems){
		if (item.uuid == productionPartId){
			for (ResultInfo& resultInfo : item.resultInfoList){
				if (resultInfo.uuid == resultId){
					if (resultInfo.outputObjects.isEmpty()){
						ObjectInfo resultObject;
						resultObject.filePath = filePath;
						resultObject.typeId = objectTypeId;

						istd::CChangeNotifier changeNotifier(this);

						resultInfo.outputObjects.push_back(resultObject);

						return resultObject.uuid;
					}
				}
			}
		}
	}

	return QByteArray();
}


QByteArray CProductionHistoryComp::InsertInputObjectPath(
			const QString& filePath,
			const QByteArray& productionPartId,
			const QByteArray& resultId,
			const QByteArray& objectTypeId)
{
	for (HistoryItem& item : m_historyItems){
		if (item.uuid == productionPartId){
			for (ResultInfo& resultInfo : item.resultInfoList){
				if (resultInfo.uuid == resultId){
					ObjectInfo inputObject;
					inputObject.filePath = filePath;
					inputObject.typeId = objectTypeId;

					istd::CChangeNotifier changeNotifier(this);

					resultInfo.inputObjects.push_back(inputObject);

					return inputObject.uuid;
				}
			}
		}
	}

	return QByteArray();
}


void CProductionHistoryComp::RemoveProductionPart(const QByteArray& productionPartId)
{
	QMutableListIterator<HistoryItem> iterator(m_historyItems);

	while (iterator.hasNext()){
		HistoryItem& item = iterator.next();
		if (item.uuid == productionPartId){
			istd::CChangeNotifier changeNotifier(this);

			iterator.remove();

			break;
		}

		iterator.next();
	}
}


// reimplemented (iser::ISerializable)

bool CProductionHistoryComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	istd::CChangeNotifier changeNotifier(archive.IsStoring() ? NULL : this);

	if (!archive.IsStoring()){
		m_historyItems.clear();
	}

	int partsCount = m_historyItems.count();

	retVal = retVal && archive.BeginMultiTag(s_partsTag, s_partTag, partsCount);

	for (int i = 0; i < partsCount; ++i){
		if(!archive.IsStoring()){
			m_historyItems.push_back(HistoryItem());
		}

		retVal = retVal && archive.BeginTag(s_partTag);

		HistoryItem& part = m_historyItems[i];

		retVal = retVal && archive.BeginTag(s_partUuidTag);
		retVal = retVal && archive.Process(part.uuid);
		retVal = retVal && archive.EndTag(s_partUuidTag);

		retVal = retVal && archive.BeginTag(s_partNameTag);
		retVal = retVal && archive.Process(part.name);
		retVal = retVal && archive.EndTag(s_partNameTag);

		retVal = retVal && archive.BeginTag(s_partSerialTag);
		retVal = retVal && archive.Process(part.serialNumber);
		retVal = retVal && archive.EndTag(s_partSerialTag);

		retVal = retVal && archive.BeginTag(s_partTimeStampTag);
		retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeDateTime(archive, part.timestamp);
		retVal = retVal && archive.EndTag(s_partTimeStampTag);

		retVal = retVal && archive.BeginTag(s_partStatusTag);
		retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum<
					istd::IInformationProvider::InformationCategory,
					istd::IInformationProvider::ToString,
					istd::IInformationProvider::FromString>(archive, part.status);
		retVal = retVal && archive.EndTag(s_partStatusTag);

		retVal = retVal && SerializeResultInfoList(archive, part.resultInfoList);

		retVal = retVal && archive.EndTag(s_partTag);

		if (!retVal){
			return false;
		}
	}

	retVal = retVal && archive.EndTag(s_partsTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CProductionHistoryComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();
}


void CProductionHistoryComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();
}


// private methods

bool CProductionHistoryComp::SerializeResultInfoList(iser::IArchive& archive, ResultInfoList& resultInfoList) const
{
	bool retVal = true;
	int resultInfosCount = resultInfoList.count();

	static const iser::CArchiveTag s_resultInfosTag("Results", "List of results", iser::CArchiveTag::TT_MULTIPLE);
	static const iser::CArchiveTag s_resultInfoTag("Result", "Single result item", iser::CArchiveTag::TT_GROUP);

	retVal = retVal && archive.BeginMultiTag(s_resultInfosTag, s_resultInfoTag, resultInfosCount);

	for (int i = 0; i < resultInfosCount; ++i){
		retVal = retVal && archive.BeginTag(s_resultInfoTag);

		if(!archive.IsStoring()){
			resultInfoList.push_back(ResultInfo());
		}

		ResultInfo& resultInfo = resultInfoList[i];

		static const iser::CArchiveTag s_inspectionUuidTag("UUID", "Unique ID of the result object", iser::CArchiveTag::TT_LEAF, &s_resultInfoTag);
		retVal = retVal && archive.BeginTag(s_inspectionUuidTag);
		retVal = retVal && archive.Process(resultInfo.uuid);
		retVal = retVal && archive.EndTag(s_inspectionUuidTag);

		static const iser::CArchiveTag s_inspectionIdTag("InspectionID", "ID of the inspection inside of product", iser::CArchiveTag::TT_LEAF, &s_resultInfoTag);
		retVal = retVal && archive.BeginTag(s_inspectionIdTag);
		retVal = retVal && archive.Process(resultInfo.inspectionId);
		retVal = retVal && archive.EndTag(s_inspectionIdTag);

		static const iser::CArchiveTag s_inspectionNameTag("Name", "Name of the inspection", iser::CArchiveTag::TT_LEAF, &s_resultInfoTag);
		retVal = retVal && archive.BeginTag(s_inspectionNameTag);
		retVal = retVal && archive.Process(resultInfo.name);
		retVal = retVal && archive.EndTag(s_inspectionNameTag);

		static const iser::CArchiveTag s_inspectionStatusTag("Status", "Processing status of the inspection", iser::CArchiveTag::TT_LEAF, &s_resultInfoTag);
		retVal = retVal && archive.BeginTag(s_inspectionStatusTag);
		retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum<
					istd::IInformationProvider::InformationCategory,
					istd::IInformationProvider::ToString,
					istd::IInformationProvider::FromString>(archive, resultInfo.status);
		retVal = retVal && archive.EndTag(s_inspectionStatusTag);

		static const iser::CArchiveTag inputObjectListTag("InputObjects", "List of input objects", iser::CArchiveTag::TT_GROUP, &s_resultInfoTag);
		retVal = retVal && archive.BeginTag(inputObjectListTag);
		retVal = retVal && SerializeObjectList(archive, resultInfo.inputObjects);
		retVal = retVal && archive.EndTag(inputObjectListTag);

		static const iser::CArchiveTag outputObjectListTag("OutputObjects", "List of output objects", iser::CArchiveTag::TT_GROUP, &s_resultInfoTag);
		retVal = retVal && archive.BeginTag(outputObjectListTag);
		retVal = retVal && SerializeObjectList(archive, resultInfo.outputObjects);
		retVal = retVal && archive.EndTag(outputObjectListTag);
	
		retVal = retVal && archive.EndTag(s_resultInfoTag);
	}

	retVal = retVal && archive.EndTag(s_resultInfosTag);

	return retVal;
}


bool CProductionHistoryComp::SerializeObjectList(iser::IArchive& archive, ObjectInfoList& objectInfoList) const
{
	bool retVal = true;
	int objectsCount = objectInfoList.count();

	static const iser::CArchiveTag objectsTag("Objects", "List of objects", iser::CArchiveTag::TT_MULTIPLE);
	static const iser::CArchiveTag objectTag("Object", "Single object item", iser::CArchiveTag::TT_GROUP);
	retVal = retVal && archive.BeginMultiTag(objectsTag, objectTag, objectsCount);

	for (int i = 0; i < objectsCount; ++i){
		retVal = retVal && archive.BeginTag(objectTag);

		if(!archive.IsStoring()){
			objectInfoList.push_back(ObjectInfo());
		}

		ObjectInfo& objectInfo = objectInfoList[i];

		static const iser::CArchiveTag uuidTag("Uuid", "", iser::CArchiveTag::TT_LEAF, &objectTag);
		retVal = retVal && archive.BeginTag(uuidTag);
		retVal = retVal && archive.Process(objectInfo.uuid);
		retVal = retVal && archive.EndTag(uuidTag);

		static const iser::CArchiveTag typeIdTag("TypeID", "", iser::CArchiveTag::TT_LEAF, &objectTag);
		retVal = retVal && archive.BeginTag(typeIdTag);
		retVal = retVal && archive.Process(objectInfo.typeId);
		retVal = retVal && archive.EndTag(typeIdTag);

		static const iser::CArchiveTag pathTag("Path", "", iser::CArchiveTag::TT_LEAF, &objectTag);
		retVal = retVal && archive.BeginTag(pathTag);
		retVal = retVal && archive.Process(objectInfo.filePath);
		retVal = retVal && archive.EndTag(pathTag);

		retVal = retVal && archive.EndTag(objectTag);
	}

	retVal = retVal && archive.EndTag(objectsTag);

	return retVal;
}



// public methods of the embedded class HistoryItem

CProductionHistoryComp::HistoryItem::HistoryItem()
	:status(istd::IInformationProvider::IC_NONE)
{
	uuid = QUuid::createUuid().toByteArray();
}


// reimplemented (iprm::IOptionsList)

int CProductionHistoryComp::HistoryItem::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CProductionHistoryComp::HistoryItem::GetOptionsCount() const
{
	return resultInfoList.count();
}


QString CProductionHistoryComp::HistoryItem::GetOptionName(int index) const
{
	return resultInfoList[index].name;
}


QString CProductionHistoryComp::HistoryItem::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CProductionHistoryComp::HistoryItem::GetOptionId(int index) const
{
	return resultInfoList[index].uuid;
}


bool CProductionHistoryComp::HistoryItem::IsOptionEnabled(int /*index*/) const
{
	return true;
}


// public methods of the embedded class PartList

CProductionHistoryComp::PartList::PartList()
	:m_parentPtr(NULL)
{
}


void CProductionHistoryComp::PartList::SetParent(const CProductionHistoryComp* parentPtr)
{
	m_parentPtr = parentPtr;
}


// reimplemented (iprm::IOptionsList)

int CProductionHistoryComp::PartList::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CProductionHistoryComp::PartList::GetOptionsCount() const
{
	return m_parentPtr->m_historyItems.count();
}


QString CProductionHistoryComp::PartList::GetOptionName(int index) const
{
	return m_parentPtr->m_historyItems[index].name;
}


QString CProductionHistoryComp::PartList::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CProductionHistoryComp::PartList::GetOptionId(int index) const
{
	return m_parentPtr->m_historyItems[index].uuid;
}


bool CProductionHistoryComp::PartList::IsOptionEnabled(int /*index*/) const
{
	return true;
}


} // namespace iprod


