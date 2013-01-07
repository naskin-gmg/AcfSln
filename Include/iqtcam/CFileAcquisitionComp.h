#ifndef iqtcam_CFileAcquisitionComp_included
#define iqtcam_CFileAcquisitionComp_included


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMutex.h>

// ACF includes
#include "iser/IFileLoader.h"
#include "ifile/IFileNameParam.h"
#include "ibase/TLoggerCompWrap.h"
#include "iproc/TSyncProcessorWrap.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"

#include "iqtcam/iqtcam.h"


namespace iqtcam
{


/**
	Bitmap loader component implementing interface \c icam::IBitmapAcquisition over \c iser::IFileLoader.
*/
class CFileAcquisitionComp:
			public ibase::CLoggerComponentBase,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileAcquisitionComp);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_ASSIGN(m_bitmapLoaderCompPtr, "BitmapLoader", "Load bitmap from file", true, "BitmapLoader");
		I_ASSIGN(m_defaultDirAttrPtr, "DefaultDir", "Directory will be used if no parameters are specified", true, ".");
		I_ASSIGN(m_defaultDirParamCompPtr, "DefaultPathParam", "Path of file or directory will be used if no parameters are specified in paramter set", false, "DefaultDirParam");
		I_ASSIGN(m_pathParamIdAttrPtr, "DirParamId", "Id used to get directory parameter (ifile::IFileNameParam)", true, "FileBitmapAcquisition");
		I_ASSIGN(m_maxCachedDirectoriesAttrPtr, "MaxCachedDirs", "Maximum number of cached directories", true, 10);
		I_ASSIGN(m_lastFileNameCompPtr, "LastFileName", "Stores last processed file name here if set", false, "LastFileName");
		I_ASSIGN(m_mirrorXParamIdAttrPtr, "MirrorXParamId", "Id used to get Mirror By X parameter", false, "MirrorXParamId");
		I_ASSIGN(m_mirrorYParamIdAttrPtr, "MirrorYParamId", "Id used to get Mirror By Y parameter", false, "MirrorYParamId");
	I_END_COMPONENT;

	CFileAcquisitionComp();

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

protected:
	struct ParamsInfo
	{
		ParamsInfo();

		QStringList files;
		QStringList::Iterator filesIter;
		quint32 idStamp;
	};

	void DoProcessImage(const iprm::IParamsSet* paramsPtr, istd::IChangeable* outputPtr) const;

private:
	I_REF(iser::IFileLoader, m_bitmapLoaderCompPtr);
	I_ATTR(QString, m_defaultDirAttrPtr);
	I_REF(ifile::IFileNameParam, m_defaultDirParamCompPtr);
	I_ATTR(int, m_maxCachedDirectoriesAttrPtr);
	I_REF(ifile::IFileNameParam, m_lastFileNameCompPtr);

	I_ATTR(QByteArray, m_pathParamIdAttrPtr);
	I_ATTR(QByteArray, m_mirrorXParamIdAttrPtr);
	I_ATTR(QByteArray, m_mirrorYParamIdAttrPtr);

	typedef QMap<QString, ParamsInfo> DirInfos;
	DirInfos m_dirInfos;

	quint32 m_lastIdStamp;
	istd::CIndex2d m_lastImageSize;

	QMutex m_lock;
};


} // namespace iqtcam


#endif // !iqtcam_CFileAcquisitionComp_included


