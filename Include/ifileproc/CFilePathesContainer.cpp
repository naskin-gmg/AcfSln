// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifileproc/CFilePathesContainer.h>


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include <istd/CChangeNotifier.h>


namespace ifileproc
{


// public methods

void CFilePathesContainer::ResetFiles()
{
	istd::CChangeNotifier changePtr(this);

	m_fileList.clear();
}

bool CFilePathesContainer::InsertFile(const QString& file, int index, bool ignoreNonExistingFiles)
{
	bool addFile = true;

	QFileInfo fileInfo(file);

	if (ignoreNonExistingFiles){
		addFile = fileInfo.exists();
	}
	
	if (addFile){
		istd::CChangeNotifier changePtr(this);
	
		if (index < 0){
			m_fileList.push_back(fileInfo);
		}
		else{
			m_fileList.insert(index, fileInfo);
		}

		return true;
	}

	return false;
}


// reimplemented (ifile::IFileListProvider)

const QFileInfoList& CFilePathesContainer::GetFileList() const
{
	return m_fileList;
}


} // namespace ifileproc


