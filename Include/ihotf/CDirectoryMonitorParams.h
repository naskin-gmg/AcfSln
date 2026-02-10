// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef ihotf_CDirectoryMonitorParams_included
#define ihotf_CDirectoryMonitorParams_included


// Qt includes
#include <QtCore/QStringList>


// AcfSln includes
#include <ihotf/IDirectoryMonitorParams.h>


namespace ihotf
{


/**
	Implementation of the IDirectoryMonitorParams interface.
*/
class CDirectoryMonitorParams: virtual public ihotf::IDirectoryMonitorParams
{
public:
	CDirectoryMonitorParams();

	// reimplemented (ihotf::IDirectoryMonitorParams)
	virtual WorkingMode GetWorkingMode() const override;
	virtual void SetWorkingMode(WorkingMode workginMode) override;
	virtual double GetPollingInterval() const override;
	virtual void SetPollingInterval(double pollingInterval) override;
	virtual int GetMinLastModificationTimeDifference() const override;
	virtual void SetMinLastModificationTimeDifference(int minLastModificationTimeDifference) override;
	virtual int GetObservedItemTypes() const override;
	virtual void SetObservedItemTypes(int observedItemTypes) override;
	virtual int GetObservedChanges() const override;
	virtual void SetObservedChanges(int observedChanges) override;
	virtual QStringList GetAcceptPatterns() const override;
	virtual void SetAcceptPatterns(const QStringList& acceptPatterns) override;
	virtual QStringList GetIgnorePatterns() const override;
	virtual void SetIgnorePatterns(const QStringList& acceptPatterns) override;
	virtual int GetFolderDepth() const override;
	virtual void SetFolderDepth(int folderDepth) override;
	virtual int GetFileTimeStampMode() const override;
	virtual void SetFileTimeStampMode(int fileTimeStampMode) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	WorkingMode m_workingMode;
	double m_pollingInterval;
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


