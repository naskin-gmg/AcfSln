#ifndef ifpf_CDirectoryMonitorParams_included
#define ifpf_CDirectoryMonitorParams_included


// AcfSln includes
#include "ifpf/IDirectoryMonitorParams.h"


namespace ifpf
{


/**
	Imlementation of the IDirectoryMonitorParams interface.
*/
class CDirectoryMonitorParams: virtual public ifpf::IDirectoryMonitorParams
{
public:
	CDirectoryMonitorParams();

	// reimüplemented (ifpf::IDirectoryMonitorParams)
	virtual double GetPoolingIntervall() const;
	virtual void SetPoolingIntervall(double poolingIntervall);
	virtual int GetObservedItemTypes() const;
	virtual void SetObservedItemTypes(int observedItemTypes);
	virtual int GetObservedChanges() const;
	virtual void SetObservedChanges(int ovservedChanges);
	virtual istd::CStringList GetFileFilters() const;
	virtual void SetFileFilters(const istd::CStringList& fileFilters);

	// reimplemented (iprm::IFileNameParam)
	virtual int GetPathType() const;
	virtual const istd::CString& GetPath() const;
	virtual void SetPath(const istd::CString& path);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_directoryPath;

	double m_poolingIntervall;
	int m_observedItemTypes;
	int m_observedChanges;

	istd::CStringList m_fileFilters;
};


} // namespace ifpf


#endif // !ifpf_CDirectoryMonitorParams_included


