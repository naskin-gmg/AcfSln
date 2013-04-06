#include "icmpstr/CRegistryLoaderComp.h"


// Qt includes
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "icmpstr/CVisualRegistry.h"


namespace icmpstr
{


// reimplemented (ifile::IFilePersistence)

int CRegistryLoaderComp::LoadFromFile(istd::IChangeable& data, const QString& filePath) const
{
	istd::CChangeNotifier notifier(&data);

	if (!IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, false)){
		return OS_FAILED;
	}

	CVisualRegistry* geometricalRegistryPtr = dynamic_cast<CVisualRegistry*>(&data);
	if (geometricalRegistryPtr != NULL){
		ReadArchiveEx registryArchive(filePath, this);
		Q_ASSERT(!registryArchive.IsStoring());

		if (!geometricalRegistryPtr->SerializeRegistry(registryArchive)){
			OnReadError(registryArchive, data, filePath);

			return OS_FAILED;
		}

		ReadArchiveEx layoutArchive(GetLayoutPath(filePath), this);
		Q_ASSERT(!layoutArchive.IsStoring());

		if (!geometricalRegistryPtr->SerializeUserData(layoutArchive)){
			SendInfoMessage(
						MI_CANNOT_READ_LAYOUT,
						tr("Layout information cannot be loaded (%1)").arg(filePath));
		}

		return OS_OK;
	}
	else{
		return BaseClass::LoadFromFile(data, filePath);
	}

	return OS_FAILED;
}


int CRegistryLoaderComp::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		return OS_FAILED;
	}

	const CVisualRegistry* geometricalRegistryPtr = dynamic_cast<const CVisualRegistry*>(&data);
	if (geometricalRegistryPtr != NULL){
		WriteArchiveEx registryArchive(filePath, GetVersionInfo(), this);
		Q_ASSERT(registryArchive.IsStoring());

		if (!const_cast<CVisualRegistry*>(geometricalRegistryPtr)->SerializeRegistry(registryArchive)){
			SendErrorMessage(
						MI_LOAD_ERROR,
						tr("Cannot store to file %1").arg(filePath));

			return OS_FAILED;
		}

		WriteArchiveEx layoutArchive(GetLayoutPath(filePath), GetVersionInfo(), this);
		Q_ASSERT(layoutArchive.IsStoring());

		if (!const_cast<CVisualRegistry*>(geometricalRegistryPtr)->SerializeUserData(layoutArchive)){
			SendInfoMessage(
						MI_CANNOT_READ_LAYOUT,
						tr("Layout information cannot be stored (%1)").arg(filePath));
		}

		return OS_OK;
	}

	return OS_FAILED;
}


// reimplemented (ifile::IFileTypeInfo)

bool CRegistryLoaderComp::GetFileExtensions(QStringList& result, int flags, bool doAppend) const
{
	if (!BaseClass::GetFileExtensions(result, flags, doAppend)){
		if (!doAppend){
			result.clear();
		}

		result.push_back("arx");
	}

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


// reimplemented (ifile::TFileSerializerComp)

void CRegistryLoaderComp::OnReadError(
			const ifile::CXmlFileReadArchive& archive,
			const istd::IChangeable& /*data*/,
			const QString& filePath) const
{
	int lastReadLine = archive.GetLastReadLine();

	QString message = tr("%1(%2) : Cannot load file").arg(filePath).arg(lastReadLine);

	SendErrorMessage(MI_CANNOT_LOAD, message);
}


} // namespace icmpstr


