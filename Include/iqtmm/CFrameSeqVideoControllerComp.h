#ifndef iqtmm_CFrameSeqVideoControllerComp_included
#define iqtmm_CFrameSeqVideoControllerComp_included


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QFileInfoList>

// ACF includes
#include "ifile/IFilePersistence.h"
#include "imod/CSingleModelObserverBase.h"
#include "icomp/CComponentBase.h"
#include "iproc/TSyncProcessorCompBase.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"
#include "imm/IVideoController.h"


namespace iqtmm
{


/**
	Component to play some image (frame) sequence from a directory.
*/
class CFrameSeqVideoControllerComp:
			public QObject,
			public iproc::TSyncProcessorCompBase<icam::IBitmapAcquisition>,
			virtual public imm::IVideoController,
			protected imod::CSingleModelObserverBase
{
	Q_OBJECT

public:
	typedef iproc::TSyncProcessorCompBase<icam::IBitmapAcquisition> BaseClass;

	I_BEGIN_COMPONENT(CFrameSeqVideoControllerComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(imm::IMediaController);
		I_REGISTER_INTERFACE(imm::IVideoInfo);
		I_REGISTER_INTERFACE(imm::IVideoController);
		I_ASSIGN(m_framesPerSecondAttrPtr, "FramesPerSecond", "Default number of frames per second if this info is unavailable from video", true, 25.0);
		I_ASSIGN(m_frameLoaderCompPtr, "FrameLoader", "Loader for the single frame", true, "FrameLoader");
		I_ASSIGN(m_frameDataCompPtr, "FrameData", "Frame instance will be loaded with loader", true, "FrameData");
	I_END_COMPONENT;

	CFrameSeqVideoControllerComp();

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (imm::IMediaController)
	virtual QString GetOpenedMediumUrl() const;
	virtual bool OpenMediumUrl(const QString& url, bool autoPlay = true);
	virtual void CloseMedium();
	virtual bool IsPlaying() const;
	virtual bool SetPlaying(bool state = true);
	virtual double GetMediumLength() const;
	virtual double GetCurrentPosition() const;
	virtual bool SetCurrentPosition(double position);
	virtual int GetSupportedFeatures() const;

	// reimplemented (imm::IVideoInfo)
	virtual int GetFramesCount() const;
	virtual double GetFrameIntervall() const;
	virtual istd::CIndex2d GetFrameSize() const;
	virtual double GetPixelAspectRatio() const;

	// reimplemented (imm::IVideoController)
	virtual int GetCurrentFrame() const;
	virtual bool SetCurrentFrame(int frameIndex);

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	bool LoadCurrentFrame();

private:
	I_ATTR(QString, m_defaultSequenceDirectoryAttrPtr);
	I_ATTR(double, m_framesPerSecondAttrPtr);
	I_REF(ifile::IFilePersistence, m_frameLoaderCompPtr);
	I_REF(istd::IChangeable, m_frameDataCompPtr);

	int m_currentFrameIndex;

	QString m_mediumUrl;

	QFileInfoList m_fileList;
	bool m_isPlaying;

	bool m_isFrameLoaded;
};


} // namespace iqtmm


#endif // !iqtmm_CFrameSeqVideoControllerComp_included

