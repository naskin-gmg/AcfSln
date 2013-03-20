#include "iqtex/CXslTransformationReadArchive.h"


// STL includes
#include <cstring>

// Qt includes
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtXml/QDomNodeList>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/QAbstractMessageHandler>

// ACF includes
#include "istd/CBase64.h"


namespace
{


class ReadArchiveMessageHandler: public QAbstractMessageHandler
{
public:
	ReadArchiveMessageHandler(istd::ILogger* logger);

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
		if (!m_document.setContent(&xmlFile)) {
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

		if (!m_document.setContent(content)) {
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


bool CXslTransformationReadArchive::Process(bool& value)
{
	QString text = PullTextNode();

	if (text == "true"){
		value = true;

		return true;
	}

	if (text == "false"){
		value = false;

		return true;
	}

	return false;
}


bool CXslTransformationReadArchive::Process(char& value)
{
	QString text = PullTextNode();

	if (!text.isEmpty()){
		value = text.at(0).toAscii();

		return true;
	}

	return false;
}


bool CXslTransformationReadArchive::Process(quint8& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = quint8(text.toShort(&retVal));

	return retVal;
}


bool CXslTransformationReadArchive::Process(qint8& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = qint8(text.toShort(&retVal));

	return retVal;
}


bool CXslTransformationReadArchive::Process(quint16& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toUShort(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(qint16& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toShort(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(quint32& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toInt(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(qint32& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toUInt(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(quint64& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toULongLong(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(qint64& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toLongLong(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(float& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toFloat(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(double& value)
{
	QString text = PullTextNode();

	bool retVal;
	value = text.toDouble(&retVal);

	return retVal;
}


bool CXslTransformationReadArchive::Process(QByteArray& value)
{
	QString text = PullTextNode();

	value = text.toLocal8Bit();

	return true;
}


bool CXslTransformationReadArchive::Process(QString& value)
{
	QString text = PullTextNode();

	value = text;

	return !m_currentNode.isNull();
}


bool CXslTransformationReadArchive::ProcessData(void* dataPtr, int size)
{
	QString text = PullTextNode();

	quint8* data = (quint8*)dataPtr;

	QVector<quint8> decodedData = istd::CBase64::ConvertFromBase64(text.toLocal8Bit());

	Q_ASSERT(size == int(decodedData.size()));

	std::memcpy(data, &decodedData[0], size);

	return !m_currentNode.isNull();
}


// protected methods

QString CXslTransformationReadArchive::PullTextNode()
{
	QString text;
	QDomNode node = m_currentNode.firstChild();
	//Kill separator tags (<br/>)
	while (!node.isText() && !node.isNull()){
		QDomNode brNode = node;
		node = node.nextSibling();
		m_currentNode.removeChild(brNode);
	}
	
	text = node.nodeValue();

	m_currentNode.removeChild(node);

	return text;
}


} // namespace iqtex


