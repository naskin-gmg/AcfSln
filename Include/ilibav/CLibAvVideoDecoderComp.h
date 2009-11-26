#ifndef ilibav_CLibAvVideoDecoderComp_included
#define ilibav_CLibAvVideoDecoderComp_included


// LIBAV includes
extern "C"{
#define inline _inline
#include "libavformat/avformat.h"
#undef inline
}

// ACF includes
#include "icomp/CComponentBase.h"
#include "iproc/IBitmapAcquisition.h"
#include "iproc/TSyncProcessorCompBase.h"
#include "imm/IVideoController.h"

#include "imeas/ISamplesSequence.h"


namespace ilibav
{


/**
	Implementation of imm::IVideoController and iproc::IBitmapAcquisition interfaces using LibAv library.
*/
class CLibAvVideoDecoderComp:
			public iproc::TSyncProcessorCompBase<iproc::IBitmapAcquisition>,
			virtual public imm::IVideoController
{
public:
	typedef iproc::TSyncProcessorCompBase<iproc::IBitmapAcquisition> BaseClass;

	enum MessageId
	{
		MI_CANNOT_OPEN = 0x56a20,
		MI_FORMAT_PROBLEM
	};

	I_BEGIN_COMPONENT(CLibAvVideoDecoderComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(imm::IMediaController);
		I_REGISTER_INTERFACE(imm::IVideoInfo);
		I_REGISTER_INTERFACE(imm::IVideoController);
		I_ASSIGN(m_bitmapObjectCompPtr, "Bitmap", "Bitmap object where current bitmap is stored", false, "Bitmap");
		I_ASSIGN(m_audioSequenceCompPtr, "AudioSequence", "Sample sequence object where current audio sample is stored", false, "AudioSequence");
		I_ASSIGN(m_autoAudioGrabLengthAttrPtr, "AutoAudioGrabLength", "If enabled, audio will be automatically grabbed", false, 1.0);
	I_END_COMPONENT();

	CLibAvVideoDecoderComp();
	virtual ~CLibAvVideoDecoderComp();

	// reimplemented (icomp::IComponent)
	void OnComponentCreated();
	void OnComponentDestroyed();

	// reimplemented (iproc::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

	// reimplemented (imm::IMediaController)
	virtual istd::CString GetOpenedMediumUrl() const;
	virtual bool OpenMediumUrl(const istd::CString& url, bool autoPlay = true);
	virtual void CloseMedium();
	virtual bool IsPlaying() const;
	virtual bool SetPlaying(bool state = true);
	virtual double GetMediumLength() const;
	virtual double GetCurrentPosition() const;
	virtual bool SetCurrentPosition(double position);
	virtual int	GetSupportedFeatures() const;

	// reimplemented (imm::IVideoInfo)
	virtual int GetFramesCount() const;
	virtual double GetFrameIntervall() const;
	virtual istd::CIndex2d GetFrameSize() const;
	virtual double GetPixelAspectRatio() const;

	// reimplemented (imm::IVideoController)
	virtual int GetCurrentFrame() const;
	virtual bool SetCurrentFrame(int frameIndex);

protected:
	bool ReadNextFrame();

private:
	int m_videoStreamId;
	int m_audioStreamId;
	AVFormatContext* m_formatContextPtr;
	AVCodecContext* m_videoCodecContextPtr;
	AVCodecContext* m_audioCodecContextPtr;
	AVCodec* m_videoCodecPtr;
	AVCodec* m_audioCodecPtr;

	AVFrame* m_framePtr;
	AVFrame* m_frameRgbPtr;

	uint8_t* m_audioInputBuffer;
	int16_t* m_audioOutputBuffer;
	bool m_ignoreFirstAudioFrame;

	istd::TDelPtr<I_BYTE, true> m_imageBufferPtr;

	AVPacket m_packet;
	int m_bytesRemaining;
	uint8_t* m_rawDataPtr;

	istd::CString m_currentUrl;

	I_REF(iimg::IBitmap, m_bitmapObjectCompPtr);
	I_REF(imeas::ISamplesSequence, m_audioSequenceCompPtr);
	I_ATTR(double, m_autoAudioGrabLengthAttrPtr);

	int m_currentFrame;
};


} // namespace ilibav


#endif // !ilibav_CLibAvVideoDecoderComp_included

