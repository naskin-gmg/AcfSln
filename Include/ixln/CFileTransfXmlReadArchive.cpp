#include "ixln/CFileTransfXmlReadArchive.h"


#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>


XALAN_USING_XERCES(DOMDocument)
XALAN_USING_XERCES(DOMImplementation)

XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(FormatterToXercesDOM)


namespace ixln
{


CFileTransfXmlReadArchive::CFileTransfXmlReadArchive(
			const istd::CString& filePath,
			const istd::CString& transfFilePath,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(rootTag)
{
	DOMDocument* documentPtr = DOMImplementation::getImplementation()->createDocument();    
	if (documentPtr != NULL){
		FormatterToXercesDOM formatter(documentPtr, NULL);

		// Now do the transform as normal
		XalanTransformer transformer;

		if (transformer.transform(
					filePath.ToString().c_str(), 
					transfFilePath.ToString().c_str(), 
					formatter) == 0){
			m_documentPtr = documentPtr;

			m_nodePtr = m_documentPtr->getFirstChild();

			if (!CheckRootNode()){
				m_documentPtr->release();
				m_documentPtr = NULL;
				m_nodePtr = NULL;
			}

			if (serializeHeader){
				SerializeAcfHeader();
			}
		}
	}
}


} // namespace ixln


