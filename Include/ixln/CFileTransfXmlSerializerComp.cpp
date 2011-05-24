#include "ixln/CFileTransfXmlSerializerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"
#include "isys/IFileSystem.h"

#include "ixln/CFileTransfXmlReadArchive.h"


namespace ixln
{


// public methods

// reimplemented (iser::IFileLoader)

bool CFileTransfXmlSerializerComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "Object is not serializable");
		}

		return false;
	}

	if ((flags & QF_NO_LOADING) != 0){
		if (!beQuiet){
			SendCriticalMessage(MI_CANNOT_SAVE, "Save operation is not implemented");
		}

		return false;
	}

	if ((flags & (QF_ANONYMOUS_ONLY | QF_DIRECTORY_ONLY)) != 0){
		return false;
	}

	if (filePathPtr != NULL){
		if ((flags & QF_NO_SAVING) != 0){
			isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
			if (fileSystemPtr != NULL){
				if (!fileSystemPtr->IsPresent(*filePathPtr)){
					return false;
				}
			}
		}

		istd::CStringList fileExtensions;
		if (GetFileExtensions(fileExtensions)){
			int extensionsCount = fileExtensions.size();
			for (int i = 0; i < extensionsCount; ++i){
				const istd::CString& extension = fileExtensions[i];
				if (filePathPtr->substr(filePathPtr->length() - extension.length() - 1) == istd::CString(".") + extension.ToLower()){
					return true;
				}
			}

			if (!beQuiet){
				SendInfoMessage(MI_BAD_EXTENSION, "File extension is not supported");
			}

			return false;
		}
	}

	return true;
}


int CFileTransfXmlSerializerComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	istd::CString transformFileName;
	if (m_transformFileNameParamCompPtr.IsValid()){
		transformFileName = m_transformFileNameParamCompPtr->GetPath();
	}

	if (transformFileName.IsEmpty()){
		SendInfoMessage(MI_BAD_TRANSFORMATION, "No transformation file");

		return StateFailed;
	}

	if (IsOperationSupported(&data, &filePath, QF_NO_SAVING, false)){
		CFileTransfXmlReadArchive archive(filePath, transformFileName);

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


int CFileTransfXmlSerializerComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	if (IsOperationSupported(&data, &filePath, QF_NO_LOADING, false)){
/*
		WriteArchiveEx archive(filePath, GetVersionInfo(), this);
		I_ASSERT(archive.IsStoring());

		const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&data);
		I_ASSERT(serializablePtr != NULL);

		if (!CheckMinimalVersion(*serializablePtr, archive.GetVersionInfo())){
			SendWarningMessage(MI_UNSUPPORTED_VERSION, "Archive version is not supported, possible lost of data");
		}

		if ((const_cast<iser::ISerializable*>(serializablePtr))->Serialize(archive)){
			return StateOk;
		}
		else{
			SendInfoMessage(MI_CANNOT_SAVE, "Cannot serialize object to file");
		}
*/
	}

	return StateFailed;
}


// protected methods

const iser::IVersionInfo* CFileTransfXmlSerializerComp::GetVersionInfo() const
{
	return m_versionInfoCompPtr.GetPtr();
}


void CFileTransfXmlSerializerComp::OnReadError(const iser::IArchive& /*archive*/, const istd::IChangeable& /*data*/, const istd::CString& filePath) const
{
	SendWarningMessage(MI_CANNOT_LOAD, istd::CString("Cannot load object from file ") + filePath);
}


bool CFileTransfXmlSerializerComp::CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const
{
	iser::IVersionInfo::VersionIds ids = versionInfo.GetVersionIds();

	for (		iser::IVersionInfo::VersionIds::const_iterator iter = ids.begin();
				iter != ids.end();
				++iter){
		int id = *iter;

		I_DWORD minimalVersionInfo = object.GetMinimalVersion(id);

		I_DWORD versionNumber;
		if (versionInfo.GetVersionNumber(id, versionNumber)){
			if (versionNumber < minimalVersionInfo){
				return false;
			}
		}
	}

	return true;
}


} // namespace ixln


