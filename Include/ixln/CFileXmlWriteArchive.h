#ifndef ixln_CFileXmlWriteArchive_included
#define ixln_CFileXmlWriteArchive_included


// ACF includes
#include "iser/CFileArchiveInfo.h"

#include "ixln/CXmlWriteArchiveBase.h"


namespace ixln
{


/**
	XERCES-based implementation of archive for writing in XML format.
	Please note that it doesn't create \c counter attribute needed by \c iser::CXmlFileReadArchive.

	\ingroup Persistence
*/
class CFileXmlWriteArchive:
			public CXmlWriteArchiveBase,
			public iser::CFileArchiveInfo
{
public:
	typedef CXmlWriteArchiveBase BaseClass;
	typedef iser::CFileArchiveInfo BaseClass2;

	explicit CFileXmlWriteArchive(
				const istd::CString& filePath,
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace ixln


#endif // !ixln_CFileXmlWriteArchive_included


