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


// CHistoryReader public methods

CHistoryReader::CHistoryReader()
{
	m_history.reserve(s_historyChunkSize);
	m_historyChunk.reserve(s_historyChunkSize);
}


void CHistoryReader::SetHistoryPath(const QString& historyPath)
{
	m_historyPath = historyPath;
}


const CHistoryReader::HistoryChunk& CHistoryReader::GetHistoryChunk()
{
	m_historyChunk.clear();

	QMutexLocker lock(&m_historyMutex);

	int chunkSize = qMin(m_history.size(), s_historyChunkSize);

	for (int i = 0; i < chunkSize; ++i){
		const iprod::IProductionHistory::PartInfo& partInfo = m_history.head();

		m_historyChunk.push_back(partInfo);

		m_history.dequeue();
	}

	return m_historyChunk;
}


const QStringList& CHistoryReader::GetErrors() const
{
	return m_errors;
}


// CHistoryReader protected methods

// reimplemented (QThread)

void CHistoryReader::run()
{
	if (m_historyPath.isEmpty()){
		return;
	}

	istd::CSystem::EnsurePathExists(m_historyPath);

	QDir repositoryRootDir(m_historyPath);

	QFileInfoList repositoryFiles;
	ifile::CFileListProviderComp::CreateFileList(repositoryRootDir, 0, 1, QStringList() << "*.xml", QDir::Time, repositoryFiles);

	for (int fileIndex = 0; fileIndex < repositoryFiles.count(); ++fileIndex){
		if (isInterruptionRequested()){
			break;
		}

		QString itemFilePath = repositoryFiles[fileIndex].absoluteFilePath();

		ifile::CCompactXmlFileReadArchive archive(itemFilePath);

		iprod::IProductionHistory::PartInfo partInfo;

		if (!CProductionHistoryComp::SerializePartInfo(archive, partInfo)){
			m_errors.push_back(QString("Repository item could not be loaded from '%1'").arg(itemFilePath));

			continue;
		}

		QMutexLocker lock(&m_historyMutex);

		m_history.enqueue(partInfo);

		if (m_history.size() >= s_historyChunkSize){
			emit HistoryChunkReady(false);
		}
	}

	emit HistoryChunkReady(true);
}


// CProductionHistoryComp public methods

CProductionHistoryComp::CProductionHistoryComp()
	:m_newObjectChangeSet(iprod::IProductionHistory::CF_NEW_OBJECT)
{
	m_historyReaderPtr = new CHistoryReader();

	QObject::connect(m_historyReaderPtr.GetPtr(), SIGNAL(HistoryChunkReady(bool)), this, SLOT(OnHistoryChunkReady(bool)), Qt::QueuedConnection);
}


// reimplemented (IProductionHistory)

QList<QByteArray> CProductionHistoryComp::GetPartInfoIds() const
{
	QReadLocker locker(&m_historyItemsLock);

	return m_historyItems.keys();
}


IProductionHistory::PartInfo CProductionHistoryComp::GetPartInfo(const QByteArray& partId) const
{
	QReadLocker locker(&m_historyItemsLock);

	HistoryItems::ConstIterator it = m_historyItems.constFind(partId);

	if (it != m_historyItems.constEnd()){
		return *it;
	}

	return IProductionHistory::PartInfo();
}


IProductionHistory::ResultInfo CProductionHistoryComp::GetResultInfo(const QByteArray& partId, const QByteArray& resultId) const
{
	QReadLocker locker(&m_historyItemsLock);

	HistoryItems::ConstIterator it = m_historyItems.constFind(partId);

	if (it != m_historyItems.constEnd()){
		for (int i = 0; i < it->results.size(); ++i){
			const ResultInfo resultInfo = it->results[i];

			if (resultInfo.uuid == resultId){
				return resultInfo;
			}
		}
	}

	return IProductionHistory::ResultInfo();
}


// reimplemented (IProductionHistoryController)

QByteArray CProductionHistoryComp::InsertNewProductionPart(
			const QString& productName,
			const QByteArray& productId,
			const QString& serialNumber,
			istd::IInformationProvider::InformationCategory status,
			const QDateTime& productionTime)
{
	PartInfo newItem;
	newItem.productName = productName;
	newItem.productId = productId;
	newItem.serialNumber = serialNumber;

	QDateTime timestamp = productionTime.isValid() ? productionTime : QDateTime::currentDateTime();
	newItem.processingInfo.time = timestamp.toMSecsSinceEpoch() / 1000;
	newItem.processingInfo.status = status;

	istd::CChangeNotifier changeNotifier(this);

	QWriteLocker locker(&m_historyItemsLock);

	m_historyItems.insert(newItem.uuid, newItem);

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
	QWriteLocker locker(&m_historyItemsLock);

	HistoryItems::Iterator it = m_historyItems.find(productionPartId);

	if (it != m_historyItems.end()){
		PartInfo& partInfo = *it;

		ResultInfo newResult;
		newResult.inspectionId = inspectionId;
		newResult.name = inspectionName;
		newResult.processingInfo.status = status;

		QDateTime processingInfoTime = resultTime.isValid() ? resultTime : QDateTime::currentDateTime();
		newResult.processingInfo.time = processingInfoTime.toMSecsSinceEpoch() / 1000;

		istd::CChangeNotifier changeNotifier(this);

		partInfo.results.push_back(newResult);

		SaveRepositoryItem(partInfo);

		return newResult.uuid;
	}

	return QByteArray();
}


QByteArray CProductionHistoryComp::InsertInspectionResultPath(
	const QString& filePath,
	const QByteArray& partId,
	const QByteArray& resultId,
	const QByteArray& objectTypeId)
{
	return InsertResultObject(filePath, partId, resultId, objectTypeId, false);
}


QByteArray CProductionHistoryComp::InsertInputObjectPath(
	const QString& filePath,
	const QByteArray& partId,
	const QByteArray& resultId,
	const QByteArray& objectTypeId)
{
	return InsertResultObject(filePath, partId, resultId, objectTypeId, true);
}


void CProductionHistoryComp::RemoveProductionPart(const QByteArray& productionPartId)
{
	QWriteLocker locker(&m_historyItemsLock);

	HistoryItems::Iterator it = m_historyItems.find(productionPartId);

	if (it != m_historyItems.end()){
		PartInfo& partInfo = *it;

		QString itemPath = GetItemPath(partInfo);

		istd::CChangeNotifier changeNotifier(this);
		QFile::remove(itemPath);

		m_historyItems.erase(it);
	}
}


// CProductionHistoryComp protected methods

// reimplemented (icomp::CComponentBase)

void CProductionHistoryComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	ReadHistory();
}


void CProductionHistoryComp::OnComponentDestroyed()
{
	if (m_historyReaderPtr.IsValid()){
		m_historyReaderPtr->requestInterruption();
		m_historyReaderPtr->wait();
	}

	BaseClass::OnComponentDestroyed();
}


// CProductionHistoryComp private methods

bool CProductionHistoryComp::SerializePartInfo(iser::IArchive& archive, PartInfo& partInfo)
{
	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_partUuidTag);
	retVal = retVal && archive.Process(partInfo.uuid);
	retVal = retVal && archive.EndTag(s_partUuidTag);

	retVal = retVal && archive.BeginTag(s_partSerialTag);
	retVal = retVal && archive.Process(partInfo.serialNumber);
	retVal = retVal && archive.EndTag(s_partSerialTag);

	retVal = retVal && archive.BeginTag(s_productNameTag);
	retVal = retVal && archive.Process(partInfo.productName);
	retVal = retVal && archive.EndTag(s_productNameTag);

	retVal = retVal && archive.BeginTag(s_productIdTag);
	retVal = retVal && archive.Process(partInfo.productId);
	retVal = retVal && archive.EndTag(s_productIdTag);

	retVal = retVal && archive.BeginTag(s_partTimeStampTag);
	retVal = retVal && archive.Process(partInfo.processingInfo.time);
	retVal = retVal && archive.EndTag(s_partTimeStampTag);

	retVal = retVal && archive.BeginTag(s_partStatusTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum<
		istd::IInformationProvider::InformationCategory,
		istd::IInformationProvider::ToString,
		istd::IInformationProvider::FromString>(archive, partInfo.processingInfo.status);
	retVal = retVal && archive.EndTag(s_partStatusTag);

	retVal = retVal && SerializeResults(archive, partInfo.results);

	return retVal;
}


bool CProductionHistoryComp::SerializeResults(iser::IArchive& archive, ResultInfoList& resultInfoList)
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
		retVal = retVal && archive.Process(resultInfo.processingInfo.time);
		retVal = retVal && archive.EndTag(s_inspectionTimeTag);

		retVal = retVal && archive.EndTag(s_processingInfoTag);

		static const iser::CArchiveTag inputObjectListTag("InputObjects", "List of input objects", iser::CArchiveTag::TT_GROUP, &s_resultInfoTag);
		retVal = retVal && archive.BeginTag(inputObjectListTag);
		retVal = retVal && SerializeObjects(archive, resultInfo.inputObjects);
		retVal = retVal && archive.EndTag(inputObjectListTag);

		static const iser::CArchiveTag outputObjectListTag("OutputObjects", "List of output objects", iser::CArchiveTag::TT_GROUP, &s_resultInfoTag);
		retVal = retVal && archive.BeginTag(outputObjectListTag);
		retVal = retVal && SerializeObjects(archive, resultInfo.outputObjects);
		retVal = retVal && archive.EndTag(outputObjectListTag);
	
		retVal = retVal && archive.EndTag(s_resultInfoTag);
	}

	retVal = retVal && archive.EndTag(s_resultInfosTag);

	return retVal;
}


bool CProductionHistoryComp::SerializeObjects(iser::IArchive& archive, ObjectInfoList& objectInfoList)
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


void CProductionHistoryComp::ReadHistory()
{
	if (!m_productionHistoryFolderCompPtr.IsValid() ||
		m_doNotLoadHistoryAttrPtr.IsValid() && *m_doNotLoadHistoryAttrPtr ||
		m_historyReaderPtr->isRunning()){
		return;
	}

	QString repositoryPath = m_productionHistoryFolderCompPtr->GetPath();

	m_historyReaderPtr->SetHistoryPath(repositoryPath);
	m_historyReaderPtr->start();
}


void CProductionHistoryComp::SaveRepositoryItem(const PartInfo& partInfo) const
{
	QString itemFilePath = GetItemPath(partInfo);

	if (!itemFilePath.isEmpty()){
		ifile::CCompactXmlFileWriteArchive archive(itemFilePath, m_versionInfoCompPtr.GetPtr());

		if (!SerializePartInfo(archive, const_cast<PartInfo&>(partInfo))){
			SendErrorMessage(0, QString("Repository item could not be saved into '%1'").arg(itemFilePath));
		}
	}
}


QString CProductionHistoryComp::GetItemPath(const PartInfo& partInfo) const
{
	Q_ASSERT(m_productionHistoryFolderCompPtr.IsValid());

	QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(partInfo.processingInfo.time * 1000);
	QString fileName = partInfo.productName  + "_" + timestamp.toString("yyyyMMdd_hhmmsszzz") + ".xml";

	QString repositoryPath = m_productionHistoryFolderCompPtr->GetPath();
	QString itemFilePath = repositoryPath + "/" + fileName;

	return itemFilePath;
}


QByteArray CProductionHistoryComp::InsertResultObject(
			const QString& filePath,
			const QByteArray& partId,
			const QByteArray& resultId,
			const QByteArray& objectTypeId,
			bool input)
{
	QWriteLocker locker(&m_historyItemsLock);

	HistoryItems::Iterator it = m_historyItems.find(partId);

	if (it != m_historyItems.end()){
		PartInfo& partInfo = *it;

		for (int resultIndex = 0; resultIndex < partInfo.results.size(); ++resultIndex){
			ResultInfo& resultInfo = partInfo.results[resultIndex];

			if (resultInfo.uuid == resultId){
				ObjectInfo resultObject;
				resultObject.filePath = filePath;
				resultObject.typeId = objectTypeId;

				istd::CChangeNotifier changeNotifier(this, &m_newObjectChangeSet);

				if (input){
					resultInfo.inputObjects.push_back(resultObject);
				}
				else{
					resultInfo.outputObjects.push_back(resultObject);
				}

				SaveRepositoryItem(partInfo);

				return resultObject.uuid;
			}
		}
	}

	return QByteArray();
}


void CProductionHistoryComp::OnHistoryChunkReady(bool lastChunk)
{
	istd::CChangeNotifier changeNotifier(this);

	const CHistoryReader::HistoryChunk& historyChunk = m_historyReaderPtr->GetHistoryChunk();

	for (int i = 0; i < historyChunk.size(); ++i){
		const PartInfo& partInfo = historyChunk[i];

		QWriteLocker locker(&m_historyItemsLock);

		m_historyItems.insert(partInfo.uuid, partInfo);
	}

	if (lastChunk){
		const QStringList& errors = m_historyReaderPtr->GetErrors();

		for (int i = 0; i < errors.size(); ++i){
			SendErrorMessage(0, errors[i]);
		}
	}
}


} // namespace iprod


