#ifndef ifileproc_CFilePathesContainer_included
#define ifileproc_CFilePathesContainer_included


// Qt includes
#include <QFileInfoList>

// ACF includes
#include "ifile/IFileListProvider.h"

#include "ifileproc/ifileproc.h"


namespace ifileproc
{


class CFilePathesContainer: virtual public ifile::IFileListProvider
{
public:
	void ResetFiles();

	/**
		Insert a new file into the list.
		If \ignoreNonExistingFiles is set, only existing files will be added to the list.
	*/
	virtual bool InsertFile(const QString& file, int index = -1, bool ignoreNonExistingFiles = false);

	// reimplemented (ifile::IFileListProvider)
	virtual QStringList GetFileList() const;

private:
	QFileInfoList m_fileList;
};


} // namespace ifileproc


#endif //!ifileproc_CFilePathesContainer_included


