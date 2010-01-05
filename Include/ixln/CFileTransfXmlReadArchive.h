#ifndef ixln_CFileTransfXmlReadArchive_included
#define ixln_CFileTransfXmlReadArchive_included


#include "ixln/CXmlReadArchiveBase.h"


namespace ixln
{


/**
	XALAN and XERCES-based implementation of archive for reading XML format with XSL transformation.

	\ingroup Persistence
*/
class CFileTransfXmlReadArchive: public CXmlReadArchiveBase
{
public:
	typedef CXmlReadArchiveBase BaseClass;

	/**	Construct archive for specified file.
	 */
	explicit CFileTransfXmlReadArchive(
			const istd::CString& filePath,
			const istd::CString& transfFilePath,
			bool serializeHeader = true,
			const iser::CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace ixln


#endif // !ixln_CFileTransfXmlReadArchive_included


