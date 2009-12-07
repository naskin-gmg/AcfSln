#include "ifpf/CHotfolderLoaderComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "ibase/CFileSystem.h"


// AcfSln includes
#include "ifpf/IHotfolder.h"


namespace ifpf
{


// reimplemented (iser::IFileLoader)

int CHotfolderLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_NO_SAVING, false)){
		return StateFailed;
	}

	istd::CChangeNotifier notifier(&data);

	ifpf::IHotfolder* hotfolderPtr = dynamic_cast<ifpf::IHotfolder*>(&data);
	if (hotfolderPtr == NULL){
		return StateFailed;
	}

	int retVal = BaseClass::LoadFromFile(data, filePath);
/*	if (retVal != StateFailed){
		iprm::IParamsSet* hotfolderParametersPtr = hotfolderPtr->GetHotfolderParams();
		if (hotfolderParametersPtr != NULL){
			ReadArchiveEx staticParamsArchive(GetStaticParamsPath(filePath), this);

			if (hotfolderParametersPtr->Serialize(staticParamsArchive)){
				return retVal;
			}
		}
	}
*/
	return retVal;
}


int CHotfolderLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_NO_LOADING, false)){
		return StateFailed;
	}

	const ifpf::IHotfolder* hotfolderPtr = dynamic_cast<const ifpf::IHotfolder*>(&data);
	if (hotfolderPtr != NULL){
		int retVal = BaseClass::SaveToFile(data, filePath);

/*		if (retVal != StateFailed){
			iprm::IParamsSet* hotfolderParametersPtr = hotfolderPtr->GetHotfolderParams();
			if (hotfolderParametersPtr != NULL){
				WriteArchiveEx staticParamsArchive(GetStaticParamsPath(filePath), GetVersionInfo(), this);

				if (hotfolderParametersPtr->Serialize(staticParamsArchive)){
					return retVal;
				}
			}
		}
		*/
	}

	return StateFailed;
}


bool CHotfolderLoaderComp::GetFileExtensions(istd::CStringList& result, int flags, bool doAppend) const
{
	if (!BaseClass::GetFileExtensions(result, flags, doAppend)){
		if (!doAppend){
			result.clear();
		}

		result.push_back("hdf");
	}

	return true;
}


istd::CString CHotfolderLoaderComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if ((extensionPtr == NULL) || extensionPtr->IsEqualNoCase("hdf")){
		return istd::CString("Hotfolder Data File");
	}

	return "";
}


// protected methods

istd::CString CHotfolderLoaderComp::GetStaticParamsPath(const istd::CString& objectPath) const
{
	ibase::CFileSystem fileSystem;

	istd::CString test1 = fileSystem.GetDirPath(objectPath);
	istd::CString test2 = fileSystem.GetBaseFileName(objectPath);

	return (fileSystem.GetDirPath(objectPath) + "/" + fileSystem.GetBaseFileName(objectPath) + ".hfp");
}


// reimplemented (ibase::TFileSerializerComp)

void CHotfolderLoaderComp::OnReadError(
			const iser::CXmlFileReadArchive& archive,
			const istd::IChangeable& /*data*/,
			const istd::CString& filePath) const
{
}


} // namespace ifpf


