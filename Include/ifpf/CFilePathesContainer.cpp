#include "ifpf/CFilePathesContainer.h"


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include "istd/TChangeNotifier.h"


namespace ifpf
{


// public methods

void CFilePathesContainer::ResetFiles()
{
	istd::CChangeNotifier changePtr(this);

	m_fileList.clear();
}

bool CFilePathesContainer::InsertFile(const QString& file, int index)
{
	QFileInfo fileInfo(file);
	if (fileInfo.exists()){
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


// reimplemented (ibase::IFileListProvider)

QStringList CFilePathesContainer::GetFileList() const
{
	QStringList fileList;

	for (		QFileInfoList::ConstIterator index = m_fileList.constBegin(); 
				index != m_fileList.constEnd();
				index++){
		fileList.push_back((*index).absoluteFilePath());
	}

	return fileList;
}


} // namespace ifpf


