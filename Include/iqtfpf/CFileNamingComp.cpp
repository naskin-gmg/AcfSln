#include "iqtfpf/CFileNamingComp.h"


// Qt includes
#include <QFileInfo>
#include <QDir>

// ACF includes
#include "iqt/iqt.h"


namespace iqtfpf
{


// public methods

// reimplemented (ifpf::IFileNamingStrategy)

istd::CString CFileNamingComp::GetFilePath(const istd::CString& fileName, const istd::CString& directoryPath) const
{
	if (!directoryPath.IsEmpty()){
		QDir outputDirectory(iqt::GetQString(directoryPath));

		QFileInfo inputFileInfo(iqt::GetQString(fileName));

		return iqt::GetCString(outputDirectory.absoluteFilePath(inputFileInfo.fileName()));
	}

	return istd::CString();
}


} // namespace iqtfpf


