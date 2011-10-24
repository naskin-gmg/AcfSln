#include "iqtcam/CFileAcquisitionComp.h"


// Qt includes
#include <QFileInfo>
#include <QByteArray>
#include <QImageReader>
#include <QStringList>

#include "istd/TChangeNotifier.h"

#include "iprm/IParamsSet.h"

#include "iqt/IQImageProvider.h"


namespace iqtcam
{


CFileAcquisitionComp::CFileAcquisitionComp()
:	m_lastIdStamp(0),
	m_lastImageSize(-1, -1)
{
}


// reimplemented (iproc::IProcessor)

int CFileAcquisitionComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	I_ASSERT(m_defaultDirAttrPtr.IsValid());	// obligatory attribute
	I_ASSERT(m_parameterIdAttrPtr.IsValid());	// obligatory attribute
	I_ASSERT(m_maxCachedDirectoriesAttrPtr.IsValid());	// obligatory attribute

	if (!m_bitmapLoaderCompPtr.IsValid()){
		return TS_INVALID;
	}

	istd::CString filesPath = *m_defaultDirAttrPtr;
	const iprm::IFileNameParam* loaderParamsPtr = NULL;
	if (paramsPtr != NULL){
		loaderParamsPtr = dynamic_cast<const iprm::IFileNameParam*>(paramsPtr->GetParameter((*m_parameterIdAttrPtr).ToString()));
		if (loaderParamsPtr != NULL){
			filesPath = loaderParamsPtr->GetPath();
		}
	}
	QDir directory(iqt::GetQString(filesPath));

	istd::CStringList extensions;
	m_bitmapLoaderCompPtr->GetFileExtensions(extensions, iser::IFileLoader::QF_LOAD);

	ParamsInfo& info = m_dirInfos[filesPath];
	if (info.filesIter == info.files.end()){
		QStringList nameFilters;

		if (!extensions.empty()){
			for (		istd::CStringList::iterator iter = extensions.begin();
						iter != extensions.end();
						++iter){
				nameFilters << (QString("*.") + iqt::GetQString(*iter));
			}
		}
		else{
			nameFilters << "*.bmp";
			nameFilters << "*.png";
			nameFilters << "*.jpg";
		}

		info.files = directory.entryList(nameFilters, QDir::Files | QDir::Readable);
		info.filesIter = info.files.begin();
	}

	info.idStamp = ++m_lastIdStamp;

	int retVal = TS_INVALID;
	if (info.filesIter != info.files.end()){
		istd::CString fileName = iqt::GetCString(directory.absoluteFilePath(*info.filesIter));
		info.filesIter++;

		if (outputPtr != NULL){
			int loadState = m_bitmapLoaderCompPtr->LoadFromFile(*outputPtr, fileName);
			retVal = (loadState == iser::IFileLoader::StateOk)? TS_OK: TS_INVALID;

			iimg::IRasterImage* imagePtr = dynamic_cast<iimg::IRasterImage*>(outputPtr);
			if (imagePtr != NULL){
				m_lastImageSize = imagePtr->GetImageSize();
			}
		}
		else{
			retVal = TS_OK;
		}
	}

	if (int(m_dirInfos.size()) > *m_maxCachedDirectoriesAttrPtr){
		DirInfos::iterator maxIdStampDiffIter = m_dirInfos.end();
		I_DWORD maxIdStampDiff = 0;
		for (		DirInfos::iterator iter = m_dirInfos.begin();
					iter != m_dirInfos.end();
					++iter){
			I_DWORD idStampDiff = m_lastIdStamp - iter->second.idStamp;
			if (idStampDiff >= maxIdStampDiff){
				maxIdStampDiff = idStampDiff;
				maxIdStampDiffIter = iter;
			}
		}

		if (maxIdStampDiffIter != m_dirInfos.end()){
			m_dirInfos.erase(maxIdStampDiffIter);	// remove element recently not used
		}
	}

	return retVal;
}


// reimplemented (iproc::IBitmapAcquisition)

istd::CIndex2d CFileAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return m_lastImageSize;
}


// public methods of embedded class ParamsInfo

CFileAcquisitionComp::ParamsInfo::ParamsInfo()
{
	filesIter = files.end();
}


} // namespace iqtcam


