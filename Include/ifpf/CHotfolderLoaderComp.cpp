#include "ifpf/CHotfolderLoaderComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingInfo.h"


namespace ifpf
{


// reimplemented (iser::IFileLoader)

int CHotfolderLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, false)){
		return StateFailed;
	}

	istd::CChangeNotifier notifier(&data);
	iprm::IParamsSet* hotfolderParamsSet = dynamic_cast<iprm::IParamsSet*>(&data);
	if (hotfolderParamsSet == NULL){
		return StateFailed;
	}

	if (m_monitorSessionsParamIdAttrPtr.IsValid()){
		const iser::ISerializable* monitoringSessionsPtr = hotfolderParamsSet->GetParameter((*m_monitorSessionsParamIdAttrPtr).ToString());
		if (monitoringSessionsPtr != NULL){
			ReadArchiveEx staticParamsArchive(GetStaticParamsPath(filePath), this);

			(const_cast<iser::ISerializable*>(monitoringSessionsPtr))->Serialize(staticParamsArchive);
		}
	}
	
	return BaseClass::LoadFromFile(data, filePath);
}


int CHotfolderLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		return StateFailed;
	}

	const iprm::IParamsSet* hotfolderParamsSet = dynamic_cast<const iprm::IParamsSet*>(&data);
	if (hotfolderParamsSet == NULL){
		return StateFailed;
	}

	int retVal = BaseClass::SaveToFile(data, filePath);
	if (retVal != StateFailed){
		if (m_monitorSessionsParamIdAttrPtr.IsValid()){
			const iser::ISerializable* monitoringSessionsPtr = dynamic_cast<const iser::ISerializable*>(hotfolderParamsSet->GetParameter((*m_monitorSessionsParamIdAttrPtr).ToString()));
			if (monitoringSessionsPtr != NULL){
				WriteArchiveEx staticParamsArchive(GetStaticParamsPath(filePath), GetVersionInfo(), this);

				if ((const_cast<iser::ISerializable*>(monitoringSessionsPtr))->Serialize(staticParamsArchive)){
					return retVal;
				}
			}
		}
	}

	return retVal;
}


// reimplemented (iser::IFileTypeInfo)

bool CHotfolderLoaderComp::GetFileExtensions(istd::CStringList& result, int flags, bool doAppend) const
{
	if (!BaseClass::GetFileExtensions(result, flags, doAppend)){
		if (!doAppend){
			result.clear();
		}

		result.push_back("hot");
	}

	return true;
}


istd::CString CHotfolderLoaderComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if ((extensionPtr == NULL) || extensionPtr->IsEqualNoCase("hot")){
		return istd::CString("Hotfolder Data File");
	}

	return "";
}


// protected methods

istd::CString CHotfolderLoaderComp::GetStaticParamsPath(const istd::CString& objectPath) const
{
	isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
	I_ASSERT(fileSystemPtr != NULL);

	if (fileSystemPtr == NULL){
		return istd::CString();
	}

	return (fileSystemPtr->GetDirPath(objectPath) + "/" + fileSystemPtr->GetBaseFileName(objectPath) + ".dms");
}


// reimplemented (ibase::TFileSerializerComp)

void CHotfolderLoaderComp::OnReadError(
			const iser::CXmlFileReadArchive& /*archive*/,
			const istd::IChangeable& /*data*/,
			const istd::CString& /*filePath*/) const
{
}


} // namespace ifpf


