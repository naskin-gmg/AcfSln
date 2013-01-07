#include "iqtcam/CFileAcquisitionComp.h"


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QByteArray>
#include <QtGui/QImageReader>
#include <QtCore/QStringList>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "iprm/IParamsSet.h"
#include "iimg/IRasterImage.h"
#include "iimg/IQImageProvider.h"
#include "iimg/CBitmap.h"
#include "iprm/TParamsPtr.h"
#include "iprm/IEnableableParam.h"


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
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	QMutexLocker lock(&m_lock);

	I_ASSERT(m_defaultDirAttrPtr.IsValid());	// obligatory attribute
	I_ASSERT(m_pathParamIdAttrPtr.IsValid());	// obligatory attribute
	I_ASSERT(m_maxCachedDirectoriesAttrPtr.IsValid());	// obligatory attribute

	if (!m_bitmapLoaderCompPtr.IsValid()){
		SendVerboseMessage("Bitmap loader was not set");

		return TS_INVALID;
	}

	QString inputPath = *m_defaultDirAttrPtr;
	iprm::TParamsPtr<ifile::IFileNameParam> loaderParamsPtr(paramsPtr, m_pathParamIdAttrPtr, m_defaultDirParamCompPtr, false);
	if (loaderParamsPtr.IsValid()){
		inputPath = loaderParamsPtr->GetPath();
	}

	QString imageFileName;

	QFileInfo inputPathInfo(inputPath);
	if (inputPathInfo.isDir()){
		QDir directory(inputPath);

		QStringList extensions;
		m_bitmapLoaderCompPtr->GetFileExtensions(extensions, iser::IFileLoader::QF_LOAD);

		ParamsInfo& info = m_dirInfos[inputPath];
		if (info.filesIter == info.files.end()){
			QStringList nameFilters;

			if (!extensions.isEmpty()){
				for (		QStringList::iterator iter = extensions.begin();
							iter != extensions.end();
							++iter){
					nameFilters << (QString("*.") + *iter);
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

		if (info.filesIter != info.files.end()){
			imageFileName = directory.absoluteFilePath(*info.filesIter);
			info.filesIter++;
		}
	}
	else if (inputPathInfo.isFile()){
		imageFileName = inputPath;
	}

	int retVal = TS_INVALID;

	if (!imageFileName.isEmpty()){
		if (outputPtr != NULL){
			int loadState = m_bitmapLoaderCompPtr->LoadFromFile(*outputPtr, imageFileName);
			retVal = (loadState == iser::IFileLoader::StateOk)? TS_OK: TS_INVALID;

			iimg::IRasterImage* imagePtr = dynamic_cast<iimg::IRasterImage*>(outputPtr);
			if (imagePtr != NULL){
				m_lastImageSize = imagePtr->GetImageSize();
			}

			if (m_lastFileNameCompPtr.IsValid()){
				m_lastFileNameCompPtr->SetPath(imageFileName);
			}

			DoProcessImage(paramsPtr, outputPtr);
		}
		else{
			retVal = TS_OK;
		}
	}

	if (int(m_dirInfos.size()) > *m_maxCachedDirectoriesAttrPtr){
		DirInfos::iterator maxIdStampDiffIter = m_dirInfos.end();
		quint32 maxIdStampDiff = 0;

		for (		DirInfos::iterator iter = m_dirInfos.begin();
					iter != m_dirInfos.end();
					++iter){
			quint32 idStampDiff = m_lastIdStamp - iter.value().idStamp;
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


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d CFileAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return m_lastImageSize;
}


// public methods of embedded class ParamsInfo

CFileAcquisitionComp::ParamsInfo::ParamsInfo()
{
	filesIter = files.end();
}


// protected methods

void CFileAcquisitionComp::DoProcessImage(const iprm::IParamsSet* paramsPtr, istd::IChangeable* outputPtr) const
{
	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (bitmapPtr == NULL || paramsPtr == NULL){
		return;
	}

	// check if postprocessing needed
	bool mirrorX = false, mirrorY = false;

	if (m_mirrorXParamIdAttrPtr.IsValid()){
		iprm::TParamsPtr<iprm::IEnableableParam> mirrorXParam(paramsPtr, *m_mirrorXParamIdAttrPtr);
		mirrorX = mirrorXParam->IsEnabled();
	}

	if (m_mirrorYParamIdAttrPtr.IsValid()){
		iprm::TParamsPtr<iprm::IEnableableParam> mirrorYParam(paramsPtr, *m_mirrorYParamIdAttrPtr);
		mirrorY = mirrorYParam->IsEnabled();
	}

	if (mirrorX || mirrorY){
		iimg::CBitmap tempBitmap;
		if (tempBitmap.CopyFrom(*bitmapPtr)){
			QImage tempImage(tempBitmap.GetQImageRef().mirrored(mirrorX, mirrorY));
			if (tempBitmap.CopyImageFrom(tempImage)){
				bitmapPtr->CopyFrom(tempBitmap);
			}
		}
	}
}


} // namespace iqtcam


