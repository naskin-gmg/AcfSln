#include "ifpf/CDirectoryMonitorParams.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchivetag.h"


namespace ifpf
{


// public methods

CDirectoryMonitorParams::CDirectoryMonitorParams()
	:m_poolingIntervall(5),
	m_observedItemTypes(OI_ALL),
	m_observedChanges(OC_ALL)
{
}


// reimüplemented (ifpf::IDirectoryMonitorParams)

double CDirectoryMonitorParams::GetPoolingIntervall() const
{
	return m_poolingIntervall;
}


void CDirectoryMonitorParams::SetPoolingIntervall(double poolingIntervall)
{
	if (m_poolingIntervall != poolingIntervall){
		istd::CChangeNotifier changePtr(this);

		m_poolingIntervall = poolingIntervall;
	}
}


int CDirectoryMonitorParams::GetObservedItemTypes() const
{
	return m_observedItemTypes;
}


void CDirectoryMonitorParams::SetObservedItemTypes(int observedItemTypes)
{
	if (m_observedItemTypes != observedItemTypes){
		istd::CChangeNotifier changePtr(this);

		m_observedItemTypes = observedItemTypes;
	}
}


int CDirectoryMonitorParams::GetObservedChanges() const
{
	return m_observedChanges;
}


void CDirectoryMonitorParams::SetObservedChanges(int ovservedChanges)
{
	if (m_observedChanges != ovservedChanges){
		istd::CChangeNotifier changePtr(this);

		m_observedChanges = ovservedChanges;
	}
}


istd::CStringList CDirectoryMonitorParams::GetFileFilters() const
{
	return m_fileFilters;
}


void CDirectoryMonitorParams::SetFileFilters(const istd::CStringList& fileFilters)
{
	if (fileFilters != m_fileFilters){
		istd::CChangeNotifier changePtr(this);

		m_fileFilters = fileFilters;
	}
}



// reimplemented (iprm::IFileNameParam)

int CDirectoryMonitorParams::GetPathType() const
{
	return iprm::IFileNameParam::PT_DIRECTORY;
}


const istd::CString& CDirectoryMonitorParams::GetPath() const
{
	return m_directoryPath;
}


void CDirectoryMonitorParams::SetPath(const istd::CString& path)
{
	if (path != m_directoryPath){
		istd::CChangeNotifier notifier(this);

		m_directoryPath = path;
	}
}


// reimplemented (iser::ISerializable)

bool CDirectoryMonitorParams::Serialize(iser::IArchive& archive)
{	
	istd::CStringList m_fileFilters;
	
	static iser::CArchiveTag pathTag("Path", "Observing directory path");
	bool retVal = archive.BeginTag(pathTag);
	retVal = retVal && archive.Process(m_directoryPath);
	retVal = retVal && archive.EndTag(pathTag);

	static iser::CArchiveTag poolingIntervallTag("PoolingIntervall", "Intervall for state update by pooling of file system infos");
	retVal = retVal && archive.BeginTag(poolingIntervallTag);
	retVal = retVal && archive.Process(m_poolingIntervall);
	retVal = retVal && archive.EndTag(poolingIntervallTag);

	static iser::CArchiveTag observedItemTypesTag("ObservedItemTypes", "Item types to be observed");
	retVal = retVal && archive.BeginTag(observedItemTypesTag);
	retVal = retVal && archive.Process(m_observedItemTypes);
	retVal = retVal && archive.EndTag(observedItemTypesTag);

	static iser::CArchiveTag observedChangesTag("ObservedChanges", "Changes in file system to be observed");
	retVal = retVal && archive.BeginTag(observedChangesTag);
	retVal = retVal && archive.Process(m_observedChanges);
	retVal = retVal && archive.EndTag(observedChangesTag);

	static iser::CArchiveTag fileFiltersTag("FileFilters", "List file filters");
	static iser::CArchiveTag fileFilterTag("FileFilter", "Single file filter");

	int fileFiltersCount = m_fileFilters.size();
	retVal = retVal && archive.BeginMultiTag(fileFiltersTag, fileFilterTag, fileFiltersCount);
	if (archive.IsStoring()){
		for (int fileFilterIndex = 0; fileFilterIndex < fileFiltersCount; fileFilterIndex++){
			retVal = retVal && archive.BeginTag(fileFilterTag);
			retVal = retVal && archive.Process(m_fileFilters[fileFilterIndex]);
			retVal = retVal && archive.EndTag(fileFilterTag);		
		}
	}
	else{
		for (int fileFilterIndex = 0; fileFilterIndex < fileFiltersCount; fileFilterIndex++){
			istd::CString fileFilter;
			retVal = retVal && archive.BeginTag(fileFilterTag);
			retVal = retVal && archive.Process(fileFilter);
			retVal = retVal && archive.EndTag(fileFilterTag);

			if (retVal){
				m_fileFilters.push_back(fileFilter);
			}
		}	
	}

	retVal = retVal && archive.EndTag(fileFiltersTag);

	return retVal;
}


} // namespace ifpf


