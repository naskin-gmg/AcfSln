#ifndef iqtcam_CFileAcquisitionComp_included
#define iqtcam_CFileAcquisitionComp_included


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QStringList>

// ACF includes
#include "iser/IFileLoader.h"
#include "iprm/IFileNameParam.h"
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
		I_ASSIGN(m_parameterIdAttrPtr, "DirParamId", "Id used to get directory parameter (iprm::IFileNameParam)", true, "FileBitmapAcquisition");
		I_ASSIGN(m_maxCachedDirectoriesAttrPtr, "MaxCachedDirs", "Maximum number of cached directories", true, 10);
		I_ASSIGN(m_lastFileNameAttrPtr, "LastFileName", "Stores last processed file name here if set", false, "LastFileName");
	I_END_COMPONENT;

	CFileAcquisitionComp();

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

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

private:
	typedef QMap<QString, ParamsInfo> DirInfos;
	DirInfos m_dirInfos;

	quint32 m_lastIdStamp;

	istd::CIndex2d m_lastImageSize;

	I_REF(iser::IFileLoader, m_bitmapLoaderCompPtr);
	I_ATTR(QString, m_defaultDirAttrPtr);
	I_ATTR(QByteArray, m_parameterIdAttrPtr);
	I_ATTR(int, m_maxCachedDirectoriesAttrPtr);
	I_REF(iprm::IFileNameParam, m_lastFileNameAttrPtr);
};


} // namespace iqtcam


#endif // !iqtcam_CFileAcquisitionComp_included


