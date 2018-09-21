#pragma once


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <iser/IVersionInfo.h>


namespace icomm
{


/**
	Implementation of version information common for different version informations.
	Common version is simple the lowest version for each ID.
*/
class CCommonVersionInfo: virtual public iser::IVersionInfo
{
public:
	CCommonVersionInfo();

	/**
		Set master version information.
	*/
	void SetMasterVersionInfo(const iser::IVersionInfo* versionInfoPtr);
	/**
		Reset version informations.
		If master version information is registered, the stored versions will be overtaken.
	*/
	void Reset();
	/**
		Merge current set of version informations with the other one.
		\param	versionInfo	version informations.
	*/
	void MergeVersions(const iser::IVersionInfo& versionInfo);

	// reimplemented (iser::IVersionInfo)
	virtual VersionIds GetVersionIds() const;
	virtual bool GetVersionNumber(int versionId, quint32& result) const;
	virtual QString GetVersionIdDescription(int versionId) const;
	virtual QString GetEncodedVersionName(int versionId, quint32 versionNumber) const;

protected:
	struct VersionIdElement
	{
		quint32 versionNumber;
		QString description;
	};

private:
	typedef QMap<int, VersionIdElement> VersionElements;
	VersionElements m_versionIdList;

	const iser::IVersionInfo* m_masterVersionInfoPtr;
};


} // namespace icomm


