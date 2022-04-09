#include <iqtex/CXslTransformationWriteArchive.h>


#if QT_VERSION < 0x060000

// Qt includes
#include <QtCore/QTextStream>
#include <QtXml/QDomNodeList>

#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/QAbstractMessageHandler>

namespace iqtex
{

class WriteArchiveMessageHandler: public QAbstractMessageHandler
{
public:
	explicit WriteArchiveMessageHandler(CXslTransformationWriteArchive* logger);

protected:
	void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation);

	CXslTransformationWriteArchive* m_loggerPtr;
};


WriteArchiveMessageHandler::WriteArchiveMessageHandler(CXslTransformationWriteArchive* logger)
{
	m_loggerPtr = logger;
}


void WriteArchiveMessageHandler::handleMessage(
			QtMsgType /*type*/,
			const QString& description,
			const QUrl& /*identifier*/,
			const QSourceLocation& /*sourceLocation*/)
{
	m_loggerPtr->SendLogMessage(
					istd::IInformationProvider::IC_WARNING,
					0,
					tr("Transformation message: ").append(description),
					"XslTransformationWriteArchive");
}


// public methods

CXslTransformationWriteArchive::CXslTransformationWriteArchive(
			const QString& filePath,
			const QString& xslFilePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr),
	BaseClass2(filePath),
	m_serializeHeader(serializeHeader),
	m_rootTag(rootTag),
	m_isSeparatorNeeded(false)
{
	if (!filePath.isEmpty()){
		OpenFile(filePath, xslFilePath);
	}
}


CXslTransformationWriteArchive::~CXslTransformationWriteArchive()
{
	Flush();
}


bool CXslTransformationWriteArchive::Flush()
{
	if (m_file.isOpen()){
		if (!m_xslFilePath.isEmpty()){
			QFile xslFile(m_xslFilePath);
			if (!xslFile.open(QIODevice::ReadOnly | QIODevice::Text)){
				return false;
			}
			QXmlQuery query(QXmlQuery::XSLT20);
			query.setMessageHandler(new WriteArchiveMessageHandler(this));
			QByteArray byteArray = m_document.toByteArray();
			QBuffer buffer(&byteArray);
			if (!buffer.open(QBuffer::WriteOnly)){
				return false;
			}
			query.setFocus(&buffer);
			query.setQuery(&xslFile);
			query.evaluateTo(&m_file); 
			xslFile.close();

			return true;
		}
		else{
			QTextStream stream(&m_file);

			m_document.save(stream, 4);

			m_file.close();
			return true;
		}
	}

	return false;
}


bool CXslTransformationWriteArchive::OpenFile(const QString& filePath, const QString& xslFilePath)
{
	bool retVal = true;

	m_file.setFileName(filePath);
	m_file.open(QIODevice::WriteOnly | QIODevice::Text);

	m_xslFilePath = xslFilePath;

	m_document.clear();

	m_currentParent = m_document.createElement(m_rootTag.GetId());

	m_document.appendChild(m_currentParent);

	if (m_serializeHeader){
		retVal = retVal && SerializeAcfHeader();
	}

	return retVal;
}


// reimplemented (iser::IArchive)

bool CXslTransformationWriteArchive::IsTagSkippingSupported() const
{
	return true;
}


bool CXslTransformationWriteArchive::BeginTag(const iser::CArchiveTag& tag)
{
	QDomElement newElement = m_document.createElement(tag.GetId());

	m_currentParent.appendChild(newElement);

	m_currentParent = newElement;

	m_isSeparatorNeeded = false;

	return true;
}


bool CXslTransformationWriteArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& count)
{
	QDomElement newElement = m_document.createElement(tag.GetId());

	newElement.setAttribute("count", count);
	m_currentParent.appendChild(newElement);

	m_currentParent = newElement;
	m_isSeparatorNeeded = false;

	return true;
}


bool CXslTransformationWriteArchive::EndTag(const iser::CArchiveTag& /*tag*/)
{
	m_currentParent = m_currentParent.parentNode().toElement();

	m_isSeparatorNeeded = false;

	return !m_currentParent.isNull();
}


bool CXslTransformationWriteArchive::Process(QString& value)
{
	return WriteStringNode(value);
}


// protected methods

bool CXslTransformationWriteArchive::WriteStringNode(const QString& text)
{
	if (m_isSeparatorNeeded){
		QDomElement separator = m_document.createElement(GetElementSeparator());

		m_currentParent.appendChild(separator);
	}

	QDomText newNode = m_document.createTextNode(text);
	m_currentParent.appendChild(newNode);
	m_isSeparatorNeeded = true;

	return true;
}


// reimplemented (iser::CTextWriteArchiveBase)

bool CXslTransformationWriteArchive::WriteTextNode(const QByteArray& text)
{
	return WriteStringNode(text);
}


} // namespace iqtex


#endif // Qt5

