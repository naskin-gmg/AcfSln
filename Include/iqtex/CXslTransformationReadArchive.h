#ifndef iqtex_CXslTransformationReadArchive_included
#define iqtex_CXslTransformationReadArchive_included


// Qt includes
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>


// ACF includes
#include "istd/ILogger.h"

#include "iser/CXmlDocumentInfoBase.h"
#include "iser/CReadArchiveBase.h"

#include "iqtex/iqtex.h"


namespace iqtex
{


/**
	Qt-based implementation of archive reading from XML file.

	\ingroup Persistence
*/
class CXslTransformationReadArchive: public iser::CReadArchiveBase, public iser::CXmlDocumentInfoBase
{
public:
	CXslTransformationReadArchive(
				const QString& filePath = "",
				const QString& xslFilePath = "",
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	bool OpenDocument(const QString& filePath, const QString& xslFilePath);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(bool& value);
	virtual bool Process(char& value);
	virtual bool Process(quint8& value);
	virtual bool Process(qint8& value);
	virtual bool Process(quint16& value);
	virtual bool Process(qint16& value);
	virtual bool Process(quint32& value);
	virtual bool Process(qint32& value);
	virtual bool Process(quint64& value);
	virtual bool Process(qint64& value);
	virtual bool Process(float& value);
	virtual bool Process(double& value);
	virtual bool Process(QByteArray& value);
	virtual bool Process(QString& value);
	virtual bool ProcessData(void* dataPtr, int size);

protected:
	/**
		Find the next text node and move the current node to the next sibling.
	*/
	QString PullTextNode();

private:
	QDomDocument m_document;
	QDomNode m_currentNode;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;
};


} // namespace iqtex


#endif // !iqtex_CXslTransformationReadArchive_included


