#include <iprod/CProductionHistoryComp.h>


// Qt includes
#include <QtCore/QDir>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CSystem.h>
#include <iser/CPrimitiveTypesSerializer.h>
#include <ifile/CCompactXmlFileReadArchive.h>
#include <ifile/CCompactXmlFileWriteArchive.h>
#include <ifile/CFileListProviderComp.h>
#include <iprm/COptionsManager.h>


namespace iprod
{


static const iser::CArchiveTag s_partsTag("Items", "List of history items", iser::CArchiveTag::TT_MULTIPLE);
static const iser::CArchiveTag s_partTag("Item", "Single item", iser::CArchiveTag::TT_GROUP);
static const iser::CArchiveTag s_partUuidTag("Uuid", "UUID of the part", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_partSerialTag("SerialNumber", "Serial number of the part", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_productNameTag("ProductName", "Product name of the part", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_productIdTag("ProductId", "Product id of the part", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_partTimeStampTag("Time", "Time of the part production", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_partStatusTag("Status", "Status of the part production", iser::CArchiveTag::TT_LEAF);


// public methods

CProductionHistoryComp::CProductionHistoryComp()
	: m_newObjectChangeSet(iprod::IProductionHistory::CF_NEW_OBJECT)
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

	for (int i = 0; i < m_historyItems.count(); ++i){
		const HistoryItem& item = m_historyItems[i];

		if (item.uuid == productionPartId){
			return item;
		}
	}

	return empty;
}


IProductionHistory::PartInfo CProductionHistoryComp::GetPartInfo(const QByteArray& productionPartId) const
{
	PartInfo retVal;

	for (int i = 0; i < m_historyItems.count(); ++i){
		const HistoryItem& item = m_historyItems[i];

		if (item.uuid == productionPartId){
			retVal.serialNumber = item.serialNumber;
			retVal.productName = item.productName;
			retVal.productId = item.productId;
			retVal.processingInfo.status = item.status;
			retVal.processingInfo.time = item.timestamp;

			break;
		}
	}

	return retVal;
}


IProductionHistory::ResultInfo CProductionHistoryComp::GetResultInfo(const QByteArray& productionPartId, const QByteArray& resultId) const
{
	for (int i = 0; i < m_historyItems.count(); ++i){
		const HistoryItem& item = m_historyItems[i];

		if (item.uuid == productionPartId){
			for (int resultIndex = 0; resultIndex < item.resultInfoList.count(); ++resultIndex){
				const ResultInfo& resultInfo = item.resultInfoList[resultIndex];

				if (resultInfo.uuid == resultId){
					return resultInfo;
				}
			}
		}
	}

	return ResultInfo();
}


// reimplemented (IProductionHistoryController)

QByteArray CProductionHistoryComp::InsertNewProductionPart(
			const QString& productName,
			const QByteArray& productId,
			const QString& serialNumber,
			istd::IInformationProvider::InformationCategory status,
			const QDateTime& productionTime)
{
	istd::CChangeNotifier changeNotifier(this);
	
	HistoryItem newItem;
	newItem.productName = productName;
	newItem.productId = productId;
	newItem.serialNumber = serialNumber;
	newItem.timestamp = productionTime.isValid() ? productionTime : QDateTime::currentDateTime();
	newItem.status = status;

	m_historyItems.push_back(newItem);

	SaveRepositoryItem(newItem);
	
	return newItem.uuid;
}


QByteArray CProductionHistoryComp::InsertNewInspectionResult(
			const QByteArray& productionPartId,
			const QByteArray& inspectionId,
			const QString & inspectionName,
			istd::IInformationProvider::InformationCategory status,
			const QDateTime& resultTime)
{
	for (int i = 0; i < m_historyItems.count(); ++i){
		HistoryItem& item = m_historyItems[i];

		if (item.uuid == productionPartId){
			istd::CChangeNotifier changeNotifier(this);

			ResultInfo newResult;

			newResult.inspectionId = inspectionId;
			newResult.name = inspectionName;
			newResult.processingInfo.status = status;
			newResult.processingInfo.time = !resultTime.isValid() ? QDateTime::currentDateTime() : resultTime;

			item.resultInfoList.push_back(newResult);

			SaveRepositoryItem(item);

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
	for (int i = 0; i < m_historyItems.count(); ++i){
		HistoryItem& item = m_historyItems[i];

		if (item.uuid == productionPartId){
			for (int resultIndex = 0; resultIndex < item.resultInfoList.count(); ++resultIndex){
				ResultInfo& resultInfo = item.resultInfoList[resultIndex];
				if (resultInfo.uuid == resultId){
					if (resultInfo.outputObjects.isEmpty()){
						ObjectInfo resultObject;
						resultObject.filePath = filePath;
						resultObject.typeId = objectTypeId;

						istd::CChangeNotifier changeNotifier(this, &m_newObjectChangeSet);

						resultInfo.outputObjects.push_back(resultObject);

						SaveRepositoryItem(item);

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
	for (int i = 0; i < m_historyItems.count(); ++i){
		HistoryItem& item = m_historyItems[i];

		if (item.uuid == productionPartId){
			for (int resultIndex = 0; resultIndex < item.resultInfoList.count(); ++resultIndex){
				ResultInfo& resultInfo = item.resultInfoList[resultIndex];

				if (resultInfo.uuid == resultId){
					ObjectInfo inputObject;
					inputObject.filePath = filePath;
					inputObject.typeId = objectTypeId;

					istd::CChangeNotifier changeNotifier(this, &m_newObjectChangeSet);

					resultInfo.inputObjects.push_back(inputObject);

					SaveRepositoryItem(item);

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

			QString itemPath = GetItemPath(item);

			QFile::remove(itemPath);

			iterator.remove();

			break;
		}

		iterator.next();
	}
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CProductionHistoryComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_productionHistoryFolderCompPtr.IsValid()){
		QString repositoryPath = m_productionHistoryFolderCompPtr->GetPath();
		if (!repositoryPath.isEmpty()){
			istd::CSystem::EnsurePathExists(repositoryPath);
		}
	}

	ReadHistoryItems();
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

		static const iser::CArchiveTag s_processingInfoTag("ProcessingInfo", "Processing status of the inspection", iser::CArchiveTag::TT_GROUP, &s_resultInfoTag);
		retVal = retVal && archive.BeginTag(s_processingInfoTag);

		static const iser::CArchiveTag s_inspectionStatusTag("Status", "Processing status of the inspection", iser::CArchiveTag::TT_LEAF, &s_processingInfoTag);
		retVal = retVal && archive.BeginTag(s_inspectionStatusTag);
		retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum<
					istd::IInformationProvider::InformationCategory,
					istd::IInformationProvider::ToString,
					istd::IInformationProvider::FromString>(archive, resultInfo.processingInfo.status);
		retVal = retVal && archive.EndTag(s_inspectionStatusTag);

		static const iser::CArchiveTag s_inspectionTimeTag("Time", "Processing time of the inspection", iser::CArchiveTag::TT_LEAF, &s_processingInfoTag);
		retVal = retVal && archive.BeginTag(s_inspectionTimeTag);
		retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeDateTime(archive, resultInfo.processingInfo.time);
		retVal = retVal && archive.EndTag(s_inspectionTimeTag);

		retVal = retVal && archive.EndTag(s_processingInfoTag);

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


bool CProductionHistoryComp::SerializeHistoryItem(iser::IArchive& archive, HistoryItem& historyItem) const
{
	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_partUuidTag);
	retVal = retVal && archive.Process(historyItem.uuid);
	retVal = retVal && archive.EndTag(s_partUuidTag);

	retVal = retVal && archive.BeginTag(s_productNameTag);
	retVal = retVal && archive.Process(historyItem.productName);
	retVal = retVal && archive.EndTag(s_productNameTag);

	retVal = retVal && archive.BeginTag(s_productIdTag);
	retVal = retVal && archive.Process(historyItem.productId);
	retVal = retVal && archive.EndTag(s_productIdTag);

	retVal = retVal && archive.BeginTag(s_partSerialTag);
	retVal = retVal && archive.Process(historyItem.serialNumber);
	retVal = retVal && archive.EndTag(s_partSerialTag);

	retVal = retVal && archive.BeginTag(s_partTimeStampTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeDateTime(archive, historyItem.timestamp);
	retVal = retVal && archive.EndTag(s_partTimeStampTag);

	retVal = retVal && archive.BeginTag(s_partStatusTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum<
				istd::IInformationProvider::InformationCategory,
				istd::IInformationProvider::ToString,
				istd::IInformationProvider::FromString>(archive, historyItem.status);
	retVal = retVal && archive.EndTag(s_partStatusTag);

	retVal = retVal && SerializeResultInfoList(archive, historyItem.resultInfoList);

	return retVal;
}


void CProductionHistoryComp::ReadHistoryItems()
{
	if (!m_productionHistoryFolderCompPtr.IsValid()){
		return;
	}

	istd::CChangeNotifier changeNotifier(this);

	m_historyItems.clear();

	QString repositoryPath = m_productionHistoryFolderCompPtr->GetPath();
	QDir repositoryRootDir(repositoryPath);

	QFileInfoList repositoryFiles;
	ifile::CFileListProviderComp::CreateFileList(repositoryRootDir, 0, 1, QStringList() << "*.xml", QDir::Name, repositoryFiles);

	for (int fileIndex = 0; fileIndex < repositoryFiles.count(); ++fileIndex){
		QString itemFilePath = repositoryFiles[fileIndex].absoluteFilePath();

		ifile::CCompactXmlFileReadArchive archive(itemFilePath, m_versionInfoCompPtr.GetPtr());
		HistoryItem historyItem;

		if (!SerializeHistoryItem(archive, historyItem)){
			SendErrorMessage(0, QString("Repository item could not be loaded from '%1'").arg(itemFilePath));

			continue;
		}

		m_historyItems.push_back(historyItem);
	}

	std::sort(m_historyItems.begin(), m_historyItems.end());
}


void CProductionHistoryComp::SaveRepositoryItem(const HistoryItem& historyItem) const
{
	QString itemFilePath = GetItemPath(historyItem);
	if (!itemFilePath.isEmpty()){
		ifile::CCompactXmlFileWriteArchive archive(itemFilePath, m_versionInfoCompPtr.GetPtr());

		if (!SerializeHistoryItem(archive, const_cast<HistoryItem&>(historyItem))){
			SendErrorMessage(0, QString("Repository item could not be saved into '%1'").arg(itemFilePath));
		}
	}
}


QString CProductionHistoryComp::GetItemPath(const HistoryItem & historyItem) const
{
	QString repositoryPath = m_productionHistoryFolderCompPtr->GetPath();

	QString fileName = historyItem.productName  + "_" + historyItem.timestamp.toString("yyyyMMdd_hhmmsszzz") + ".xml";

	QString itemFilePath = repositoryPath + "/" + fileName;

	return itemFilePath;
}


// public methods of the embedded class HistoryItem

CProductionHistoryComp::HistoryItem::HistoryItem()
	:status(istd::IInformationProvider::IC_NONE)
{
	uuid = QUuid::createUuid().toByteArray();
}


bool CProductionHistoryComp::HistoryItem::operator < (const HistoryItem& other) const
{
	return timestamp < other.timestamp;
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
	return m_parentPtr->m_historyItems[index].productName;
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


