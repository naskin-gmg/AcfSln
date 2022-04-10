#include <iqtex/CXslTransformationReadArchive.h>


#if QT_VERSION < 0x060000

// STL includes
#include <cstring>

// Qt includes
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtXml/QDomNodeList>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/QAbstractMessageHandler>


namespace
{


class ReadArchiveMessageHandler: public QAbstractMessageHandler
{
public:
	explicit ReadArchiveMessageHandler(istd::ILogger* logger);

protected:
	void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation);

	istd::ILogger* m_loggerPtr;
};


ReadArchiveMessageHandler::ReadArchiveMessageHandler(istd::ILogger* logger)
{
	m_loggerPtr = logger;
}


void ReadArchiveMessageHandler::handleMessage(
				QtMsgType /*type*/,
				const QString& description,
				const QUrl& /*identifier*/,
				const QSourceLocation& /*sourceLocation*/)
{
	m_loggerPtr->SendLogMessage(
				istd::IInformationProvider::IC_WARNING,
				0,
				QObject::tr("Transformation message: %1").arg(description),
				"XslTransformationWriteArchive");
}


}


namespace iqtex
{


CXslTransformationReadArchive::CXslTransformationReadArchive(
			const QString& filePath,
			const QString& xslFilePath,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	m_serializeHeader(serializeHeader),
	m_rootTag(rootTag)
{
	if (!filePath.isEmpty()){
		OpenFile(filePath, xslFilePath);
	}
}


bool CXslTransformationReadArchive::OpenFile(const QString& filePath, const QString& xslFilePath)
{
	QFile xmlFile(filePath);
	if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}


	if (xslFilePath.isEmpty()){
		if (!m_document.setContent(&xmlFile)){
			xmlFile.close();

			return false;
		}
	}
	else{
		QFile xslfile(xslFilePath);
		if (!xslfile.open(QIODevice::ReadOnly | QIODevice::Text)){
			return false;
		}

		QString content;

		QXmlQuery query(QXmlQuery::XSLT20);
		ReadArchiveMessageHandler handler(this);
		query.setMessageHandler(&handler);
		query.setFocus(&xmlFile);
		query.setQuery(&xslfile);
		query.evaluateTo(&content); 

		if (!m_document.setContent(content)){
			xmlFile.close();
			xslfile.close();

			return false;
		}
	}

	if (m_currentNode.nodeValue() != m_rootTag.GetId()){
		QDomElement mainElement = m_document.documentElement();

		m_currentNode = mainElement;
	}

	bool retVal = !m_currentNode.isNull();

	if (m_serializeHeader){
		retVal = retVal && SerializeAcfHeader();
	}

	return retVal;
}


// reimplemented (iser::IArchive)

bool CXslTransformationReadArchive::IsTagSkippingSupported() const
{
	return true;
}


bool CXslTransformationReadArchive::BeginTag(const iser::CArchiveTag& tag)
{
	QString tagId(tag.GetId());

	QDomElement element = m_currentNode.firstChildElement(tagId);
	if (!element.isNull()){
		m_currentNode = element;
	}
	return !element.isNull();
}


bool CXslTransformationReadArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	QString tagId(tag.GetId());

	QDomElement element = m_currentNode.firstChildElement(tagId);
	if (!element.isNull()){
		m_currentNode = element;
	}
	else{
		return false;
	}

	int tempCount = 0;
	QDomElement child = element.firstChildElement(QString(subTag.GetId()));
	while (!child.isNull()){
		tempCount++;
		child = child.nextSiblingElement(QString(subTag.GetId()));
	}
	count = tempCount;

	return !element.isNull();
}


bool CXslTransformationReadArchive::EndTag(const iser::CArchiveTag& /*tag*/)
{
	QDomNode parent = m_currentNode.parentNode();
	
	parent.removeChild(m_currentNode);

	m_currentNode = parent;

	return !m_currentNode.isNull();
}


bool CXslTransformationReadArchive::Process(QString& value)
{
	return ReadStringNode(value);
}


// protected methods

bool CXslTransformationReadArchive::ReadStringNode(QString& text)
{
	QDomNode node = m_currentNode.firstChild();
	//Kill separator tags (<br/>)
	while (!node.isText() && !node.isNull()){
		QDomNode brNode = node;
		node = node.nextSibling();
		m_currentNode.removeChild(brNode);
	}
	
	text = node.nodeValue();

	m_currentNode.removeChild(node);

	return !m_currentNode.isNull();
}


// reimplemented (iser::CTextReadArchiveBase)

bool CXslTransformationReadArchive::ReadTextNode(QByteArray& text)
{
	QString stringText;

	if (ReadStringNode(stringText)){
		text = stringText.toLocal8Bit();

		return true;
	}

	return false;
}


} // namespace iqtex


#endif // Qt5


