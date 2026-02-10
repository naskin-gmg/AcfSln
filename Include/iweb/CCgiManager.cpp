// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iweb/CCgiManager.h>


// STL includes
#include <iostream>
#include <string>

// Qt includes
#include <QtCore/QUrl>


namespace iweb
{


QString CCgiManager::ContentImageGIF = "image/gif";
QString CCgiManager::ContentImageJPEG = "image/jpeg";
QString CCgiManager::ContentImageJPG = "image/jpg";
QString CCgiManager::ContentImagePNG = "image/png";
QString CCgiManager::ContentImageSVG = "image/svg+xml";
QString CCgiManager::ContentImageTIFF = "image/tiff";
QString CCgiManager::ContentFont = "application/font";
QString CCgiManager::ContentFontEOT = "application/vnd.ms-fontobject";
QString CCgiManager::ContentFontOTF = "font/opentype";
QString CCgiManager::ContentFontTTF = "font/ttf";
QString CCgiManager::ContentFontWOFF = "font/woff";
QString CCgiManager::ContentScriptECMAS = "text/ecmascript";
QString CCgiManager::ContentScriptVB = "text/vbscript";
QString CCgiManager::ContentTypeCSS = "text/css";
QString CCgiManager::ContentTypeHTML = "text/html";
QString CCgiManager::ContentTypeJS = "text/javascript";
QString CCgiManager::ContentTypeJSON = "application/json";
QString CCgiManager::ContentTypeText = "text/plain";
QString CCgiManager::ContentTypeXML = "text/xml";
QString CCgiManager::HeaderComSpec = "COMSPEC";
QString CCgiManager::HeaderDocumentRoot = "DOCUMENT_ROOT";
QString CCgiManager::HeaderGatewayInterface = "GATEWAY_INTERFACE";
QString CCgiManager::HeaderHttpAccept = "HTTP_ACCEPT";
QString CCgiManager::HeaderHttpAcceptEncoding = "HTTP_ACCEPT_ENCODING";
QString CCgiManager::HeaderHttpAcceptLanguage = "HTTP_ACCEPT_LANGUAGE";
QString CCgiManager::HeaderHttpConnection= "HTTP_CONNECTION";
QString CCgiManager::HeaderHttpCookie = "HTTP_COOKIE";
QString CCgiManager::HeaderHttpHost= "HTTP_HOST";
QString CCgiManager::HeaderHttpUserAgent = "HTTP_USER_AGENT";
QString CCgiManager::HeaderPath = "PATH";
QString CCgiManager::HeaderQueryString = "QUERY_STRING";
QString CCgiManager::HeaderRemoteAddress = "REMOTE_ADDR";
QString CCgiManager::HeaderRemotePort = "REMOTE_PORT";
QString CCgiManager::HeaderRequestMethod = "REQUEST_METHOD";
QString CCgiManager::HeaderRequestUri = "REQUEST_URI";
QString CCgiManager::HeaderScriptFilename= "SCRIPT_FILENAME";
QString CCgiManager::HeaderScriptName = "SCRIPT_NAME";
QString CCgiManager::HeaderServerAddress = "SERVER_ADDR";
QString CCgiManager::HeaderServerAdministrator = "SERVER_ADMIN";
QString CCgiManager::HeaderServerName = "SERVER_NAME";
QString CCgiManager::HeaderServerPort = "SERVER_PORT";
QString CCgiManager::HeaderServerProtocol= "SERVER_PROTOCOL";
QString CCgiManager::HeaderServerSignature = "SERVER_SIGNATURE";
QString CCgiManager::HeaderServerSoftware= "SERVER_SOFTWARE";
QString CCgiManager::MethodDelete = "DELETE";
QString CCgiManager::MethodGet = "GET";
QString CCgiManager::MethodHead = "HEAD";
QString CCgiManager::MethodPost = "POST";
QString CCgiManager::MethodPut = "PUT";


// public methods

CCgiManager::CCgiManager()
{
	ProcessHeaders();
	ProcessCookies();
	ProcessGet();
	ProcessPost();
}

void CCgiManager::AddResponseHeader(const QString& header, const QString& value)
{
	// Set the response header into the instance
	m_responseHeaders.insert(header, value);
}


QString CCgiManager::EncodeQuery(const KeyValueMap& queryMap, const QString& pairGlue)
{
	QStringList queryList;
	foreach (QString key, queryMap){
		QStringList pairList;
		pairList.append(QString(QUrl::toPercentEncoding(key)).replace(" ", "+"));
		pairList.append(QString(QUrl::toPercentEncoding(queryMap.value(key))).replace(" ", "+"));

		queryList.append(pairList.join("="));
	}

	return queryList.join(pairGlue);
}


QString CCgiManager::EncodeQuery(const QVariantMap& queryMap, const QString& pairGlue)
{
	QStringList queryList;
	foreach (QString key, queryMap.keys()){
		QStringList pairList;
		pairList.append(QString(QUrl::toPercentEncoding(key)).replace(" ", "+"));
		pairList.append(QString(QUrl::toPercentEncoding(queryMap.value(key).toString())).replace(" ", "+"));

		queryList.append(pairList.join("="));
	}

	return queryList.join(pairGlue);
}


void CCgiManager::WriteResponse()
{
	SendHeaders();

	SendResponseContent();
}


QString CCgiManager::GetResponseContent() const
{
	return m_responseContent;
}


QString CCgiManager::GetCookie(const QString& cookieName) const
{
	if (m_cookies.contains(cookieName)){
		return m_cookies.value(cookieName);
	}

	return NULL;
}


const CCgiManager::KeyValueMap& CCgiManager::GetCookies() const
{
	return m_cookies;
}


QString CCgiManager::GetParameter(const QString& parameterName) const
{
	if (m_postParameters.contains(parameterName)){
		return m_postParameters.value(parameterName);
	}

	if (m_getParameters.contains(parameterName)){
		return m_getParameters.value(parameterName);
	}

	return NULL;
}


const CCgiManager::KeyValueMap& CCgiManager::GetPostData() const
{
	return m_postParameters;
}


const CCgiManager::KeyValueMap& CCgiManager::GetQueryData() const
{
	return m_getParameters;
}


QString CCgiManager::GetRequestHeader(const QString& requestHeaderType) const
{
	if (m_requestHeaders.contains(requestHeaderType)){
		return m_requestHeaders.value(requestHeaderType);
	}

	return NULL;
}


const CCgiManager::KeyValueMap& CCgiManager::GetRequestHeaders() const
{
	return m_requestHeaders;
}


void CCgiManager::SetResponseContent(const QString& responseContent)
{
	m_responseContent = responseContent;
}


void CCgiManager::SetResponseContentType(const QString& contentType)
{
	AddResponseHeader("Content-Type", contentType);
}


// protected methods

void CCgiManager::ProcessCookies()
{
	QString cookies = m_requestHeaders.value(HeaderHttpCookie);
	if (!cookies.isEmpty()){
		m_cookies = DecodeQuery(cookies, ";");
	}
}


void CCgiManager::ProcessGet()
{	
	QString query = m_requestHeaders.value(CCgiManager::HeaderQueryString);
	if (!query.isEmpty()){
		m_getParameters = DecodeQuery(query);
	}
}


void CCgiManager::ProcessHeaders()
{
	m_requestHeaders.insert(HeaderDocumentRoot, QString(qgetenv(HeaderDocumentRoot.toLatin1().data())));
	m_requestHeaders.insert(HeaderGatewayInterface, QString(qgetenv(HeaderGatewayInterface.toLatin1().data())));
	m_requestHeaders.insert(HeaderHttpAccept, QString(qgetenv(HeaderHttpAccept.toLatin1().data())));
	m_requestHeaders.insert(HeaderHttpAcceptEncoding, QString(qgetenv(HeaderHttpAcceptEncoding.toLatin1().data())));
	m_requestHeaders.insert(HeaderHttpAcceptLanguage, QString(qgetenv(HeaderHttpAcceptLanguage.toLatin1().data())));
	m_requestHeaders.insert(HeaderHttpConnection, QString(qgetenv(HeaderHttpConnection.toLatin1().data())));
	m_requestHeaders.insert(HeaderHttpCookie, QString(qgetenv(HeaderHttpCookie.toLatin1().data())));
	m_requestHeaders.insert(HeaderHttpHost, QString(qgetenv(HeaderHttpHost.toLatin1().data())));
	m_requestHeaders.insert(HeaderHttpUserAgent, QString(qgetenv(HeaderHttpUserAgent.toLatin1().data())));
	m_requestHeaders.insert(HeaderPath, QString(qgetenv(HeaderPath.toLatin1().data())));
	m_requestHeaders.insert(HeaderQueryString, QString(qgetenv(HeaderQueryString.toLatin1().data())));
	m_requestHeaders.insert(HeaderRemoteAddress, QString(getenv(HeaderRemoteAddress.toLatin1().data())));
	m_requestHeaders.insert(HeaderRemotePort, QString(qgetenv(HeaderRemotePort.toLatin1().data())));
	m_requestHeaders.insert(HeaderRequestMethod, QString(qgetenv(HeaderRequestMethod.toUpper().toLatin1().data())));
	m_requestHeaders.insert(HeaderRequestUri, QString(qgetenv(HeaderRequestUri.toLatin1().data())));
	m_requestHeaders.insert(HeaderScriptFilename, QString(qgetenv(HeaderScriptFilename.toLatin1().data())));
	m_requestHeaders.insert(HeaderScriptName, QString(qgetenv(HeaderScriptName.toLatin1().data())));
	m_requestHeaders.insert(HeaderServerAddress, QString(qgetenv(HeaderServerAddress.toLatin1().data())));
	m_requestHeaders.insert(HeaderServerAdministrator, QString(qgetenv(HeaderServerAdministrator.toLatin1().data())));
	m_requestHeaders.insert(HeaderServerName, QString(qgetenv(HeaderServerName.toLatin1().data())));
	m_requestHeaders.insert(HeaderServerPort, QString(qgetenv(HeaderServerPort.toLatin1().data())));
	m_requestHeaders.insert(HeaderServerProtocol, QString(qgetenv(HeaderServerProtocol.toLatin1().data())));
	m_requestHeaders.insert(HeaderServerSignature, QString(qgetenv(HeaderServerSignature.toLatin1().data())));
	m_requestHeaders.insert(HeaderServerSoftware, QString(qgetenv(HeaderServerSoftware.toLatin1().data())));
}


void CCgiManager::ProcessPost()
{
	if (m_requestHeaders.value(CCgiManager::HeaderRequestMethod).toUpper() == CCgiManager::MethodPost){
		std::string postData;
		std::getline(std::cin, postData);
		if (!QString::fromStdString(postData).isEmpty()){
			m_postParameters = DecodeQuery(QString().fromStdString(postData));
		}
	}
}


void CCgiManager::SendResponseContent()
{
	std::cout << m_responseContent.toStdString() << std::endl;
}


void CCgiManager::SendHeaders()
{
	// Add the content length header
	AddResponseHeader("Content-Length", QString::number(m_responseContent.length()));

	// Create the iterator
	KeyValueMap::iterator headersIter;

	// Loop through the headers
	for (headersIter = m_responseHeaders.begin(); headersIter != m_responseHeaders.end(); ++headersIter){
		// Print the header
		std::cout << headersIter.key().toStdString() << ": " << headersIter.value().toStdString() << "\r\n";
	}

	// Send an extra newline
	std::cout << "\r\n";
}


CCgiManager::KeyValueMap CCgiManager::DecodeQuery(const QString& query, const QString& pairSeparator)
{
	KeyValueMap queryMap;

	QStringList parameterList = query.split(pairSeparator);
	foreach (QString pair, parameterList){
		QStringList pairList = pair.split("=");
		queryMap.insert(
					QUrl::fromPercentEncoding(pairList.at(0).toLatin1()).replace("+", " "),
					QUrl::fromPercentEncoding(pairList.at(1).toLatin1()).replace("+", " "));
	}

	return queryMap;
}



} // namespace iweb


