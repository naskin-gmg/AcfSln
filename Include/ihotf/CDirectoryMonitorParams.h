#ifndef ihotf_CDirectoryMonitorParams_included
#define ihotf_CDirectoryMonitorParams_included


// Qt includes
#include <QtCore/QStringList>


// AcfSln includes
#include "ihotf/IDirectoryMonitorParams.h"


namespace ihotf
{


/**
	Implementation of the IDirectoryMonitorParams interface.
*/
class CDirectoryMonitorParams: virtual public ihotf::IDirectoryMonitorParams
{
public:
	CDirectoryMonitorParams();

	// reimüplemented (ihotf::IDirectoryMonitorParams)
	virtual double GetPoolingIntervall() const;
	virtual void SetPoolingIntervall(double poolingIntervall);
	virtual int GetMinLastModificationTimeDifference() const;
	virtual void SetMinLastModificationTimeDifference(int minLastModificationTimeDifference);
	virtual int GetObservedItemTypes() const;
	virtual void SetObservedItemTypes(int observedItemTypes);
	virtual int GetObservedChanges() const;
	virtual void SetObservedChanges(int observedChanges);
	virtual QStringList GetAcceptPatterns() const;
	virtual void SetAcceptPatterns(const QStringList& acceptPatterns);
	virtual QStringList GetIgnorePatterns() const;
	virtual void SetIgnorePatterns(const QStringList& acceptPatterns);
	virtual int GetFolderDepth() const;
	virtual void SetFolderDepth(int folderDepth);
	virtual int GetFileTimeStampMode() const;
	virtual void SetFileTimeStampMode(int fileTimeStampMode);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_poolingIntervall;
	int m_observedItemTypes;
	int m_observedChanges;
	int m_minLastModificationTimeDifference;
	int m_folderDepth;
	int m_fileTimeStampMode;

	QStringList m_acceptPatterns;
	QStringList m_ignorePatterns;
};


} // namespace ihotf


#endif // !ihotf_CDirectoryMonitorParams_included


