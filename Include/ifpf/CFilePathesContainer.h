#ifndef ifpf_CFilePathesContainer_included
#define ifpf_CFilePathesContainer_included


// Qt includes
#include <QFileInfoList>

// ACF includes
#include "ibase/IFileListProvider.h"

#include "ifpf/ifpf.h"


namespace ifpf
{


class CFilePathesContainer: virtual public ibase::IFileListProvider
{
public:
	void ResetFiles();

	/**
		Insert a new file into the list.
		If \ignoreNonExistingFiles is set, only existing files will be added to the list.
	*/
	virtual bool InsertFile(const QString& file, int index = -1, bool ignoreNonExistingFiles = false);

	// reimplemented (ibase::IFileListProvider)
	virtual QStringList GetFileList() const;

private:
	QFileInfoList m_fileList;
};


} // namespace ifpf


#endif //!ifpf_CFilePathesContainer_included


