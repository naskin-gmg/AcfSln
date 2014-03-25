#ifndef ihotf_CHotfolderLoaderComp_included
#define ihotf_CHotfolderLoaderComp_included


// ACF includes
#include "ifile/CXmlFileReadArchive.h"
#include "ifile/CXmlFileWriteArchive.h"

#include "ifile/TFileSerializerComp.h"


namespace ihotf
{


/**
	Hotfolder data loader.
	This implementation supports persistence of the static and dynamic model of the hotfolder.
*/
class CHotfolderLoaderComp: public ifile::TFileSerializerComp<ifile::CXmlFileReadArchive, ifile::CXmlFileWriteArchive>
{
public:
	typedef ifile::TFileSerializerComp<ifile::CXmlFileReadArchive, ifile::CXmlFileWriteArchive> BaseClass;

	I_BEGIN_COMPONENT(CHotfolderLoaderComp);
		I_ASSIGN(m_monitorSessionsParamIdAttrPtr, "MonitorSessionsParamId", "Parameter ID for monitoring sessions", true, "MonitoringSessions");
	I_END_COMPONENT;

	enum MessageId
	{
		MI_LOAD_ERROR = 680,
		MI_CANNOT_READ_STATIC_PARAMS,
		MI_CANNOT_READ_DYNAMIC_PARAMS
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
	virtual bool GetFileExtensions(QStringList& result, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	/**
		Get the path of the static hotfolder parameter file.
	*/
	QString GetStaticParamsPath(const QString& objectPath) const;

	// reimplemented (ifile::TFileSerializerComp)
	virtual void OnReadError(
				const ifile::CXmlFileReadArchive& archive,
				const istd::IChangeable& data,
				const QString& filePath) const;

private:
	I_ATTR(QByteArray, m_monitorSessionsParamIdAttrPtr);
};


} // namespace ihotf


#endif // !ihotf_CHotfolderLoaderComp_included


