#ifndef iqtex_CXslTransformationWriteArchive_included
#define iqtex_CXslTransformationWriteArchive_included


#if QT_VERSION < 0x060000


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
	virtual bool IsTagSkippingSupported() const override;
	virtual bool BeginTag(const iser::CArchiveTag& tag) override;
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count) override;
	virtual bool EndTag(const iser::CArchiveTag& tag) override;
	virtual bool Process(QString& value) override;
	using BaseClass::Process;

protected:
	bool WriteStringNode(const QString& text);

	// reimplemented (iser::CTextWriteArchiveBase)
	virtual bool WriteTextNode(const QByteArray& text) override;

	virtual bool SendLogMessage(
		istd::IInformationProvider::InformationCategory category,
		int id,
		const QString& message,
		const QString& messageSource,
		int flags = 0) const override
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


#endif // Qt5

#endif // !iqtex_CXslTransformationWriteArchive_included


