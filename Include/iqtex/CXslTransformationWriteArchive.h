#ifndef iqtex_CXslTransformationWriteArchive_included
#define iqtex_CXslTransformationWriteArchive_included


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QBuffer>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

// ACF includes
#include <iser/CTextWriteArchiveBase.h>
#include <ifile/CFileArchiveInfo.h>
#include <iser/CXmlDocumentInfoBase.h>

#include <iqtex/iqtex.h>


namespace iqtex
{


/**
	Qt-based implementation of archive for writing in XML format.
	Please note that it doesn't create \c counter attribute needed by \c ifile::CSimpleXmlFileReadArchive.

	\ingroup Persistence
*/
class CXslTransformationWriteArchive:
			public iser::CTextWriteArchiveBase,
			public ifile::CFileArchiveInfo,
			public iser::CXmlDocumentInfoBase
{
public:
	typedef iser::CTextWriteArchiveBase BaseClass;
	typedef ifile::CFileArchiveInfo BaseClass2;

	CXslTransformationWriteArchive(
				const QString& filePath = "",
				const QString& xslFilePath = "",
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
	~CXslTransformationWriteArchive();

	bool Flush();

	bool OpenFile(const QString& filePath, const QString& xslFilePath);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(QString& value);
	using BaseClass::Process;

protected:
	bool WriteStringNode(const QString& text);

	// reimplemented (iser::CTextWriteArchiveBase)
	bool WriteTextNode(const QByteArray& text);

	virtual bool SendLogMessage(
		istd::IInformationProvider::InformationCategory category,
		int id,
		const QString& message,
		const QString& messageSource,
		int flags = 0) const
	{
		return iser::CWriteArchiveBase::SendLogMessage(category, id, message, messageSource, flags);
	}
private:
	friend class WriteArchiveMessageHandler;

	QDomDocument m_document;
	QDomElement m_currentParent;

	QFile m_file;
	QString m_xslFilePath;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;

	bool m_isSeparatorNeeded;
};


} // namespace iqtex


#endif // !iqtex_CXslTransformationWriteArchive_included


