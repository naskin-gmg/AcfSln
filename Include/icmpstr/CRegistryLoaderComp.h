#ifndef icmpstr_CRegistryLoaderComp_included
#define icmpstr_CRegistryLoaderComp_included


// Qt includes
#include <QtCore/QObject>


// ACF includes
#include "ifile/CXmlFileReadArchive.h"
#include "ifile/CXmlFileWriteArchive.h"

#include "ifile/TFileSerializerComp.h"


namespace icmpstr
{


/**
	Special registry loader supporting of loading layout data.
*/
class CRegistryLoaderComp:
	public QObject,
	public ifile::TFileSerializerComp<ifile::CXmlFileReadArchive, ifile::CXmlFileWriteArchive>
{
	Q_OBJECT

public:
	typedef ifile::TFileSerializerComp<ifile::CXmlFileReadArchive, ifile::CXmlFileWriteArchive> BaseClass;

	I_BEGIN_COMPONENT(CRegistryLoaderComp);
	I_END_COMPONENT;

	enum MessageId
	{
		MI_LOAD_ERROR = 680,
		MI_CANNOT_READ_LAYOUT
	};

	// reimplemented (ifile::IFilePersistence)
	virtual int LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;
	virtual int SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	QString GetLayoutPath(const QString& registryPath) const;

	// reimplemented (ifile::TFileSerializerComp)
	virtual void OnReadError(
				const ifile::CXmlFileReadArchive& archive,
				const istd::IChangeable& data,
				const QString& filePath) const;

	using QObject::tr;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryLoaderComp_included


