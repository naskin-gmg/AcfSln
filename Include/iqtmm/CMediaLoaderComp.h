#ifndef iqtmm_CMediaLoaderComp_included
#define iqtmm_CMediaLoaderComp_included


#include "ifile/IFilePersistence.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iqtmm/iqtmm.h"


namespace iqtmm
{


class CMediaLoaderComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			virtual public ifile::IFilePersistence
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMediaLoaderComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN(m_autoPlayAttrPtr, "AutoPlay", "If enabled, movie will be automatically played after loading", true, true);
	I_END_COMPONENT;

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xa7e0,
		MI_FILE_NOT_EXIST,
		MI_BAD_FORMAT
	};

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath) const;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

private:
	I_ATTR(bool, m_autoPlayAttrPtr);
};


} // namespace iqtmm


#endif // !iqtmm_CMediaLoaderComp_included

