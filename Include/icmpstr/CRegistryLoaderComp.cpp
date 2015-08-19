#include "icmpstr/CRegistryLoaderComp.h"


// Qt includes
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>


// ACF includes
#include "istd/CChangeNotifier.h"
#include "ifile/TFileSerializerComp.h"
#include "ifile/CXmlFileReadArchive.h"
#include "ifile/CXmlFileWriteArchive.h"

// ACF-Solutions includes
#include "icmpstr/CVisualRegistry.h"


namespace icmpstr
{


// reimplemented (ifile::IFilePersistence)

bool CRegistryLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	const icomp::CRegistry* serializablePtr = CompCastPtr<icomp::CRegistry>(dataObjectPtr);
	if ((dataObjectPtr != NULL) && (serializablePtr == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, QObject::tr("Only ACF registries can be serialized"));
		}

		return false;
	}

	if ((flags & (QF_FILE)) == 0){
		return false;
	}

	if (filePathPtr != NULL){
		QFileInfo fileInfo(*filePathPtr);

		if ((flags & (QF_LOAD | QF_SAVE)) == QF_LOAD){
			if (!fileInfo.exists()){
				if (!beQuiet){
					SendWarningMessage(MI_FILE_NOT_EXIST, QString("File %1 does not exist").arg(*filePathPtr));
				}

				return false;
			}
		}

		if ((flags & QF_SAVE) != 0){
			QDir directory = fileInfo.dir();
			if (!directory.exists()){
				if (!beQuiet){
					SendWarningMessage(MI_FILE_NOT_EXIST, QString("Save target directory %1 does not exist").arg(directory.canonicalPath()));
				}

				return false;
			}
		}

		if (fileInfo.suffix().compare("arx", Qt::CaseInsensitive) != 0){
			if (!beQuiet){
				SendInfoMessage(MI_BAD_EXTENSION, QObject::tr("File extension is not supported"));
			}

			return false;
		}
	}

	return true;
}


int CRegistryLoaderComp::LoadFromFile(
			istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	istd::CChangeNotifier notifier(&data);

	if (!IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, false)){
		return OS_FAILED;
	}

	icomp::CRegistry* registryPtr = dynamic_cast<icomp::CRegistry*>(&data);
	if (registryPtr != NULL){
		ifile::TFileSerializerComp<ifile::CXmlFileReadArchive, ifile::CXmlFileWriteArchive>::ReadArchiveEx registryArchive(filePath, this);
		Q_ASSERT(!registryArchive.IsStoring());

		CVisualRegistry* geometricalRegistryPtr = dynamic_cast<CVisualRegistry*>(registryPtr);
		if (geometricalRegistryPtr != NULL){
			if (!geometricalRegistryPtr->SerializeRegistry(registryArchive)){
				int lastReadLine = registryArchive.GetLastReadLine();

				QString message = tr("%1(%2) : Cannot load registry file").arg(filePath).arg(lastReadLine);

				SendErrorMessage(MI_CANNOT_LOAD, message);

				return OS_FAILED;
			}

			ifile::TFileSerializerComp<ifile::CXmlFileReadArchive, ifile::CXmlFileWriteArchive>::ReadArchiveEx layoutArchive(GetLayoutPath(filePath), this);
			Q_ASSERT(!layoutArchive.IsStoring());

			if (!geometricalRegistryPtr->SerializeUserData(layoutArchive)){
				SendInfoMessage(
							MI_CANNOT_READ_LAYOUT,
							tr("Layout information cannot be loaded (%1)").arg(filePath));
			}

			return OS_OK;
		}
		else{
			if (!registryPtr->Serialize(registryArchive)){
				int lastReadLine = registryArchive.GetLastReadLine();

				QString message = tr("%1(%2) : Cannot load registry file").arg(filePath).arg(lastReadLine);

				SendErrorMessage(MI_CANNOT_LOAD, message);

				return OS_FAILED;
			}

			return OS_OK;
		}
	}

	return OS_FAILED;
}


int CRegistryLoaderComp::SaveToFile(
			const istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (!IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		return OS_FAILED;
	}

	const icomp::CRegistry* registryPtr = dynamic_cast<const icomp::CRegistry*>(&data);
	if (registryPtr != NULL){
		ifile::TFileSerializerComp<ifile::CXmlFileReadArchive, ifile::CXmlFileWriteArchive>::WriteArchiveEx registryArchive(filePath, m_versionInfoCompPtr.GetPtr(), this);
		Q_ASSERT(registryArchive.IsStoring());

		const CVisualRegistry* geometricalRegistryPtr = dynamic_cast<const CVisualRegistry*>(registryPtr);
		if (geometricalRegistryPtr != NULL){
			if (!const_cast<CVisualRegistry*>(geometricalRegistryPtr)->SerializeRegistry(registryArchive)){
				SendErrorMessage(
							MI_LOAD_ERROR,
							tr("Cannot store registry to file %1").arg(filePath));

				return OS_FAILED;
			}

			ifile::TFileSerializerComp<ifile::CXmlFileReadArchive, ifile::CXmlFileWriteArchive>::WriteArchiveEx layoutArchive(GetLayoutPath(filePath), m_versionInfoCompPtr.GetPtr(), this);
			Q_ASSERT(layoutArchive.IsStoring());

			if (!const_cast<CVisualRegistry*>(geometricalRegistryPtr)->SerializeUserData(layoutArchive)){
				SendInfoMessage(
							MI_CANNOT_READ_LAYOUT,
							tr("Layout information cannot be stored (%1)").arg(filePath));
			}

			return OS_OK;
		}
		else{
			if (!const_cast<icomp::CRegistry*>(registryPtr)->Serialize(registryArchive)){
				SendErrorMessage(
							MI_LOAD_ERROR,
							tr("Cannot store registry to file %1").arg(filePath));

				return OS_FAILED;
			}

			return OS_OK;
		}
	}

	return OS_FAILED;
}


// reimplemented (ifile::IFileTypeInfo)

bool CRegistryLoaderComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	result.push_back("arx");

	return true;
}


QString CRegistryLoaderComp::GetTypeDescription(const QString* extensionPtr) const
{
	if ((extensionPtr == NULL) || (extensionPtr->compare("arx", Qt::CaseInsensitive) == 0)){
		return tr("ACF registry file");
	}

	return "";
}


// protected methods

QString CRegistryLoaderComp::GetLayoutPath(const QString& registryPath) const
{
	QFileInfo fileInfo(registryPath);
	QString layoutPath = fileInfo.dir().absoluteFilePath(fileInfo.completeBaseName() + ".alx");

	return layoutPath;
}


} // namespace icmpstr


