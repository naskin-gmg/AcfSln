#ifndef icmpstr_CRegistryLoaderComp_included
#define icmpstr_CRegistryLoaderComp_included


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <ifile/IFilePersistence.h>
#include <ilog/TLoggerCompWrap.h>


namespace icmpstr
{


/**
	Special registry loader supporting of loading layout data.
*/
class CRegistryLoaderComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			virtual public ifile::IFilePersistence
{
	Q_OBJECT

public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CRegistryLoaderComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
		I_ASSIGN(m_supportOldFormatAttrPtr, "SupportOldFormat", "If enabled old format of registries (ARX) wil be supported", true, true);
	I_END_COMPONENT;

	enum MessageId
	{
		MI_LOAD_ERROR = 680,
		MI_CANNOT_READ_LAYOUT
	};

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr,
				int flags,
				bool beQuiet) const;
	virtual OperationState LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;
	virtual OperationState SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	QString GetLayoutPath(const QString& registryPath, bool oldFormat) const;

	using QObject::tr;

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
	I_ATTR(bool, m_supportOldFormatAttrPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryLoaderComp_included


