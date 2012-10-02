#ifndef ifpf_CFileContainer_included
#define ifpf_CFileContainer_included


// Qt includes
#include <QFileInfoList>

// ACF includes
#include "ibase/IFileListProvider.h"

#include "ifpf/ifpf.h"


namespace ifpf
{


class CFileContainer: virtual public ibase::IFileListProvider
{
public:
	void ResetFiles();

	/**
		Insert a new file into the list.
	*/
	virtual bool InsertFile(const QString& file, int index = -1);

	// reimplemented (ibase::IFileListProvider)
	virtual QStringList GetFileList() const;

private:
	QFileInfoList m_fileList;
};


} // namespace ifpf


#endif //!ifpf_CFileContainer_included


