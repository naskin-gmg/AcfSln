#ifndef ifpf_CDirectoryMonitorParams_included
#define ifpf_CDirectoryMonitorParams_included


// Qt includes
#include <QtCore/QStringList>


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
	virtual QStringList GetAcceptPatterns() const;
	virtual void SetAcceptPatterns(const QStringList& acceptPatterns);
	virtual QStringList GetIgnorePatterns() const;
	virtual void SetIgnorePatterns(const QStringList& acceptPatterns);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_poolingIntervall;
	int m_observedItemTypes;
	int m_observedChanges;

	QStringList m_acceptPatterns;
	QStringList m_ignorePatterns;
};


} // namespace ifpf


#endif // !ifpf_CDirectoryMonitorParams_included


