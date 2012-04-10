#ifndef iqtcam_CFileAcquisitionComp_included
#define iqtcam_CFileAcquisitionComp_included


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QStringList>

// ACF includes
#include "iser/IFileLoader.h"
#include "icomp/CComponentBase.h"
#include "iprm/IFileNameParam.h"

#include "iproc/TSyncProcessorWrap.h"

#include "iproc/IBitmapAcquisition.h"

#include "iqtcam/iqtcam.h"


namespace iqtcam
{


/**
	Bitmap loader component implementing interface \c iproc::IBitmapAcquisition over \c iser::IFileLoader.
*/
class CFileAcquisitionComp:
			public icomp::CComponentBase,
			virtual public iproc::TSyncProcessorWrap<iproc::IBitmapAcquisition>
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileAcquisitionComp);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_REGISTER_INTERFACE(iproc::IBitmapAcquisition);
		I_ASSIGN(m_bitmapLoaderCompPtr, "BitmapLoader", "Load bitmap from file", true, "BitmapLoader");
		I_ASSIGN(m_defaultDirAttrPtr, "DefaultDir", "Directory will be used if no parameters are specified", true, ".");
		I_ASSIGN(m_parameterIdAttrPtr, "DirParamId", "Id used to get directory parameter (iprm::IFileNameParam)", true, "FileBitmapAcquisition");
		I_ASSIGN(m_maxCachedDirectoriesAttrPtr, "MaxCachedDirs", "Maximum number of cached directories", true, 10);
	I_END_COMPONENT;

	CFileAcquisitionComp();

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (iproc::IBitmapAcquisition)
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
};


} // namespace iqtcam


#endif // !iqtcam_CFileAcquisitionComp_included


