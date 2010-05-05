#ifndef ifpf_CHotfolderLoaderComp_included
#define ifpf_CHotfolderLoaderComp_included


// ACF includes
#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "ibase/TFileSerializerComp.h"


namespace ifpf
{


/**
	Hotfolder data loader.
	This implementation supports persistence of the static and dynamic model of the hotfolder.
*/
class CHotfolderLoaderComp: public ibase::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive>
{
public:
	typedef ibase::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> BaseClass;

	I_BEGIN_COMPONENT(CHotfolderLoaderComp);
		I_ASSIGN(m_monitorSessionsParamIdAttrPtr, "MonitorSessionsParamId", "Parameter ID for monitoring sessions", false, "MonitoringSessions");
	I_END_COMPONENT;

	enum MessageId
	{
		MI_LOAD_ERROR = 680,
		MI_CANNOT_READ_STATIC_PARAMS,
		MI_CANNOT_READ_DYNAMIC_PARAMS
	};

	// reimplemented (iser::IFileLoader)
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(istd::CStringList& result, int flags = 0, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;

protected:
	/**
		Get the path of the static hotfolder parameter file.
	*/
	istd::CString GetStaticParamsPath(const istd::CString& objectPath) const;

	// reimplemented (ibase::TFileSerializerComp)
	virtual void OnReadError(
				const iser::CXmlFileReadArchive& archive,
				const istd::IChangeable& data,
				const istd::CString& filePath) const;

private:
	I_ATTR(istd::CString, m_monitorSessionsParamIdAttrPtr);
};


} // namespace ifpf


#endif // !ifpf_CHotfolderLoaderComp_included


