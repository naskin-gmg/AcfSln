#ifndef iqtex_CXslTransformationReadArchive_included
#define iqtex_CXslTransformationReadArchive_included


#if QT_VERSION < 0x060000

// Qt includes
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>


// ACF includes
#include <istd/ILogger.h>

#include <iser/CXmlDocumentInfoBase.h>
#include <iser/CTextReadArchiveBase.h>

#include <iqtex/iqtex.h>


namespace iqtex
{


/**
	Qt-based implementation of archive reading from XML file.

	\ingroup Persistence
*/
class CXslTransformationReadArchive: public iser::CTextReadArchiveBase, public iser::CXmlDocumentInfoBase
{
public:
	CXslTransformationReadArchive(
				const QString& filePath = "",
				const QString& xslFilePath = "",
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	bool OpenFile(const QString& filePath, const QString& xslFilePath);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const override;
	virtual bool BeginTag(const iser::CArchiveTag& tag) override;
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count) override;
	virtual bool EndTag(const iser::CArchiveTag& tag) override;
	virtual bool Process(QString& value) override;

protected:
	bool ReadStringNode(QString& text);

	// reimplemented (iser::CTextReadArchiveBase)
	virtual bool ReadTextNode(QByteArray& text) override;

private:
	QDomDocument m_document;
	QDomNode m_currentNode;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;
};


} // namespace iqtex


#endif // Qt5

#endif // !iqtex_CXslTransformationReadArchive_included


