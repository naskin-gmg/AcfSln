// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iweb_CCgiManager_included
#define iweb_CCgiManager_included


// Qt includes 
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QByteArray>
#include <QtCore/QVariant>
#include <QtCore/QVariantMap>
#include <QtCore/QMap>


namespace iweb
{


class CCgiManager
{
public:
	typedef QMap<QString, QString> KeyValueMap;

	/**
		This property contains the header type for GIF
	*/
	static QString ContentImageGIF;

	/**
		This property contains the header type for JPEG
	*/
	static QString ContentImageJPEG;

	/**
		This property contains the header type for JPG
	*/
	static QString ContentImageJPG;

	/**
		This property contains the header type for PNG
	*/
	static QString ContentImagePNG;

	/**
		Thie property contains the header type for SVG
	*/
	static QString ContentImageSVG;

	/**
		This property contains the header type for TIFF
	*/
	static QString ContentImageTIFF;

	/**
		This property contins the header type for FONT
	*/
	static QString ContentFont;

	/**
		This property contains the header type for EOT
	*/
	static QString ContentFontEOT;

	/**
		This property contains the header type for OTF
	*/
	static QString ContentFontOTF;

	/**
		This property contains the header type for TTF
	*/
	static QString ContentFontTTF;

	/**
		This property contains the header type for WOFF
	*/
	static QString ContentFontWOFF;

	/**
		This property contains the header type for ECMAS
	*/
	static QString ContentScriptECMAS;

	/**
		This property contains the header type for VB
	*/
	static QString ContentScriptVB;

	/**
		This property contains the header type for CSS
	*/
	static QString ContentTypeCSS;

	/**
		This property contain the header type for HTML
	*/
	static QString ContentTypeHTML;

	/**
		This property contains the header type for JS
	*/
	static QString ContentTypeJS;

	/**
		This property contains the header type for JSON
	*/
	static QString ContentTypeJSON;

	/**
		This property contains the header type for TXT
	*/
	static QString ContentTypeText;

	/**
		This property contains the header type for XML
	*/
	static QString ContentTypeXML;

	/**
		This property contains the COMSPEC header name
	*/
	static QString HeaderComSpec;

	/**
		This property contains the DOCUMENT_ROOT header name
	*/
	static QString HeaderDocumentRoot;

	/**
		This proeprty contains the GATEWAY_INTERFACE header name
	*/
	static QString HeaderGatewayInterface;

	/**
		This property contains the HTTP_ACCEPT header name
	*/
	static QString HeaderHttpAccept;

	/**
		This property contains the HTTP_ACCEPT_ENCODING header name
	*/
	static QString HeaderHttpAcceptEncoding;

	/**
		This property contains the HTTP_ACCEPT_LANGUAGE header
	*/
	static QString HeaderHttpAcceptLanguage;

	/**
		This property contains the HTTP_CONNECTION header name
	*/
	static QString HeaderHttpConnection;

	/**
		This property contains the HTTP_COOKIE header name
	*/
	static QString HeaderHttpCookie;

	/**
		This property contains the HTTP_HOST header name
	*/
	static QString HeaderHttpHost;

	/**
		This property contains the HTTP_USER_AGENT header name
	*/
	static QString HeaderHttpUserAgent;

	/**
		This property contains the PATH header name
	*/
	static QString HeaderPath;

	/**
		This property contains the QUERY_STRING header name
	*/
	static QString HeaderQueryString;

	/**
		This property contains the REMOTE_ADDR header name
	*/
	static QString HeaderRemoteAddress;

	/**
		This property contain the REMOTE_PORT header name
	*/
	static QString HeaderRemotePort;

	/**
		This property contains the REQUEST_METHOD header name
	*/
	static QString HeaderRequestMethod;

	/**
		This property contains the REQUEST_URI header name
	*/
	static QString HeaderRequestUri;

	/**
		This paragraph contains the SCRIPT_FILENAME header name
	*/
	static QString HeaderScriptFilename;

	/**
		This property contains the SCRIPT_NAME header namr
	*/
	static QString HeaderScriptName;

	/**
		This property contains the SERVER_ADDR header name
	*/
	static QString HeaderServerAddress;

	/**
		This property contians the SERVER_ADMIN header name
	*/
	static QString HeaderServerAdministrator;

	/**
		This property contains the SERVER_NAME header name
	*/
	static QString HeaderServerName;

	/**
		This property contains the SERVER_PORT header name
	*/
	static QString HeaderServerPort;

	/**
		This property contains the SERVER_PROTOCOL header name
	*/
	static QString HeaderServerProtocol;

	/**
		This property contains the SERVER_SIGNATURE header name
	*/
	static QString HeaderServerSignature;

	/**
		This property contains the SERVER_SOFTWARE header name
	*/
	static QString HeaderServerSoftware;

	/**
		This property contains the DELETE data request method name
	*/
	static QString MethodDelete;

	/**
		This property contains the GET data request method name
	*/
	static QString MethodGet;

	/**
		This property contains the HEAD data request method name
	*/
	static QString MethodHead;

	/**
		This property contains the POST data request method name
	*/
	static QString MethodPost;

	/**
		This property contains the PUT data request method name
	*/
	static QString MethodPut;

	/**
		This constructor automatically parses the HTTP headers and data
	*/
	CCgiManager();

	/**
		This method adds a response header to the instance
	*/
	void AddResponseHeader(const QString& header, const QString& value);

	/**
		This method encodes a query map into a query string
	*/
	QString EncodeQuery(const KeyValueMap& queryMap, const QString& pairGlue = "&");

	/**
		This method encodes a query variant map into a query string
	*/
	QString EncodeQuery(const QVariantMap& queryMap, const QString& pairGlue = "&");

	/**
		This method writes the HTTP response to the browser/socket
	*/
	void WriteResponse();

	/**
		This method returns the current response content in the instance
	*/
	QString GetResponseContent() const;

	/**
		This method returns a cookie from the instance if one exists
	*/
	QString GetCookie(const QString& cookieName) const;

	/**
		This method returns the current cookie map in the instance
	*/
	const KeyValueMap& GetCookies() const;

	/**
		This method returns a GET/POST parameter from the instance if one exists
	*/
	QString GetParameter(const QString& parameterName) const;

	/**
		This method returns the current POST map from the instance
	*/
	const KeyValueMap& GetPostData() const;

	/**
		This method returns the current GET map from the instance
	*/
	const KeyValueMap& GetQueryData() const;

	/**
		This method returns a request header from the instance if one exists
	*/
	QString GetRequestHeader(const QString& requestHeaderType) const;

	/**
		This method returns the current request header map in the instance
	*/
	const KeyValueMap& GetRequestHeaders() const;

	/**
		This method sets the response content into the instance
	*/
	void SetResponseContent(const QString& responseContent);

	/**
		This method sets the content type header into the instance
	*/
	void SetResponseContentType(const QString& contentType);

protected:
	/**
		This method processes the HTTP_COOKIE from the header
	*/
	void ProcessCookies();

	/**
		This method processes the QUERY_STRING from the header
	*/
	void ProcessGet();

	/**
		This method processes the request headers into the instance
	*/
	void ProcessHeaders();

	/**
		This method processes the POST data into the instance
	*/
	void ProcessPost();

	/**
		This method writes the content to the browser/socket/stream
	*/
	void SendResponseContent();

	/**
		This method writes the response headers to the browser/socket/stream
	*/
	void SendHeaders();

	/**
		This method decodes a query string into a query map
	*/
	KeyValueMap DecodeQuery(const QString& query, const QString& pairSeparator = "&");

private:
	/**
		This property contains the response content
	*/
	QString m_responseContent;

	/**
		This property contains the HTTP cookies
	*/
	KeyValueMap m_cookies;

	/**
		This property contains the GET data
	*/
	KeyValueMap m_getParameters;

	/**
		This property contains the POST data
	*/
	KeyValueMap m_postParameters;

	/**
		This property contains the request headers from the client
	*/
	KeyValueMap m_requestHeaders;

	/**
		This property contains the response headers to send to the client
	*/
	KeyValueMap m_responseHeaders;
};


} // namespace iweb


#endif // iweb_CCgiManager_included


