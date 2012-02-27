#include "iqtex/CXslSerializerComp.h"


//Qt includes
#include <QObject>


namespace iqtex
{


bool CXslSerializerComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, QObject::tr("Object is not serializable"));
		}

		return false;
	}

	if ((flags & QF_FILE) == 0){
		return false;
	}

	bool isSupported =
				(((flags & QF_LOAD) != 0) && m_xslReadFilePath.IsValid() && !m_xslReadFilePath->GetPath().isEmpty()) ||
				(((flags & QF_SAVE) != 0) && m_xslWriteFilePath.IsValid() && !m_xslWriteFilePath->GetPath().isEmpty());
	if (!isSupported){
		return false;
	}

	if (filePathPtr != NULL){
		if ((flags & QF_LOAD) != 0){
			isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
			if (fileSystemPtr != NULL){
				if (!fileSystemPtr->IsPresent(*filePathPtr)){
					return false;
				}
			}
		}

		QStringList fileExtensions;
		if (GetFileExtensions(fileExtensions)){
			int extensionsCount = fileExtensions.size();
			for (int i = 0; i < extensionsCount; ++i){
				const QString& extension = fileExtensions[i];
				if (!filePathPtr->isEmpty() && filePathPtr->mid(filePathPtr->length() - extension.length() - 1) == QString(".") + extension.toLower()){
					return true;
				}
			}

			if (!beQuiet){
				SendInfoMessage(MI_BAD_EXTENSION, QObject::tr("File extension is not supported"));
			}

			return false;
		}
	}

	return true;
}


int CXslSerializerComp::LoadFromFile(istd::IChangeable& data, const QString& filePath) const
{
	if (!CheckInputFilePath(filePath)){
		return false;
	}

	if (IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, false)){
		CXslTransformationReadArchive archive(filePath, m_xslReadFilePath->GetPath());

		I_ASSERT(!archive.IsStoring());

		iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(&data);
		I_ASSERT(serializablePtr != NULL);

		istd::CChangeNotifier changePtr(NULL, istd::IChangeable::CF_MODEL);

		if (serializablePtr->Serialize(archive)){
			changePtr.SetPtr(&data);

			return StateOk;
		}
		else{
			OnReadError(archive, data, filePath);
		}
	}
	return StateFailed;
}


int CXslSerializerComp::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	if (!CheckTargetDirectory(filePath)){
		return StateFailed;
	}

	if (IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		CXslTransformationWriteArchive archive(filePath, m_xslWriteFilePath->GetPath(), GetVersionInfo(), this);
		I_ASSERT(archive.IsStoring());

		const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&data);
		I_ASSERT(serializablePtr != NULL);

		if (!CheckMinimalVersion(*serializablePtr, archive.GetVersionInfo())){
			SendWarningMessage(MI_UNSUPPORTED_VERSION, QObject::tr("Archive version is not supported, possible lost of data"));
		}

		if ((const_cast<iser::ISerializable*>(serializablePtr))->Serialize(archive)){
			return StateOk;
		}
		else{
			SendInfoMessage(MI_CANNOT_SAVE, QObject::tr("Cannot serialize object to file"));
		}
	}

	return StateFailed;
}


// protected methods


const iser::IVersionInfo* CXslSerializerComp::GetVersionInfo() const
{
	return m_versionInfoCompPtr.GetPtr();
}


void CXslSerializerComp::OnReadError(const iser::IArchive& /*archive*/, const istd::IChangeable& /*data*/, const QString& filePath) const
{
	SendWarningMessage(MI_CANNOT_LOAD, QObject::tr("Cannot load object from file ") + filePath);
}


bool CXslSerializerComp::CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const
{
	iser::IVersionInfo::VersionIds ids = versionInfo.GetVersionIds();

	for (		iser::IVersionInfo::VersionIds::const_iterator iter = ids.begin();
				iter != ids.end();
				++iter){
		int id = *iter;

		I_DWORD objectMinimalVersion = object.GetMinimalVersion(id);

		I_DWORD versionNumber;
		if (versionInfo.GetVersionNumber(id, versionNumber)){
			if (versionNumber < objectMinimalVersion){
				return false;
			}
		}
	}

	return true;
}


bool CXslSerializerComp::CheckInputFilePath(const QString filePath) const
{
	isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
	if (fileSystemPtr == NULL){
		SendWarningMessage(0, "File path could not be checked");

		return true;
	}
	
	if (!fileSystemPtr->IsPresent(filePath)){
		SendWarningMessage(MI_CANNOT_LOAD, QString("File %1  does not exist").arg(filePath));

		return false;
	}
	
	return true;
}


bool CXslSerializerComp::CheckTargetDirectory(const QString dirPath) const
{
	isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
	if (fileSystemPtr == NULL){
		SendWarningMessage(0, "Target directory path could not be checked");

		return true;
	}

	if (!fileSystemPtr->IsPresent(fileSystemPtr->GetDirPath(dirPath))){
		SendWarningMessage(MI_CANNOT_SAVE, QString("Save target directory %1  does not exist").arg(fileSystemPtr->GetDirPath(dirPath)));

		return false;
	}

	return true;
}


} // namespace iqtex


