#ifndef ixln_CFileXmlReadArchive_included
#define ixln_CFileXmlReadArchive_included


#include "iser/CFileArchiveInfo.h"

#include "ixln/CXmlReadArchiveBase.h"


namespace ixln
{


/**
	XERCES-based implementation of archive for reading XML format.
	Please note that it doesn't need \c counter attribute needed by \c iser::CXmlFileReadArchive.

	\ingroup Persistence
*/
class CFileXmlReadArchive:
			public CXmlReadArchiveBase,
			public iser::CFileArchiveInfo
{
public:
	typedef CXmlReadArchiveBase BaseClass;
	typedef iser::CFileArchiveInfo BaseClass2;

	/**	Construct archive for specified file.
	 */
	explicit CFileXmlReadArchive(const istd::CString& filePath, bool serializeHeader = true, const iser::CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace ixln


#endif // !ixln_CFileXmlReadArchive_included


