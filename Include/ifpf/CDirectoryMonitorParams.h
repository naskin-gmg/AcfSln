#ifndef ifpf_CDirectoryMonitorParams_included
#define ifpf_CDirectoryMonitorParams_included


// AcfSln includes
#include "ifpf/IDirectoryMonitorParams.h"


namespace ifpf
{


/**
	Implementation of the IDirectoryMonitorParams interface.
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
	virtual istd::CStringList GetAcceptPatterns() const;
	virtual void SetAcceptPatterns(const istd::CStringList& acceptPatterns);
	virtual istd::CStringList GetIgnorePatterns() const;
	virtual void SetIgnorePatterns(const istd::CStringList& acceptPatterns);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_poolingIntervall;
	int m_observedItemTypes;
	int m_observedChanges;

	istd::CStringList m_acceptPatterns;
	istd::CStringList m_ignorePatterns;
};


} // namespace ifpf


#endif // !ifpf_CDirectoryMonitorParams_included


