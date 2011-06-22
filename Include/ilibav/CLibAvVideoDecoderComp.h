#ifndef ilibav_CLibAvVideoDecoderComp_included
#define ilibav_CLibAvVideoDecoderComp_included


// STL includes
#include <vector>

// LIBAV includes
extern "C"{
#define inline _inline
#define __STDC_CONSTANT_MACROS
#include "libavformat/avformat.h"
#undef inline
}

// ACF includes
#include "icomp/CComponentBase.h"
#include "iproc/IBitmapAcquisition.h"
#include "iproc/TSyncProcessorCompBase.h"
#include "imm/IVideoController.h"

#include "imeas/IDataSequence.h"


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
		I_ASSIGN(m_bitmapObjectCompPtr, "BitmapObject", "Bitmap object where current bitmap is stored", false, "Bitmap");
		I_ASSIGN(m_audioSampleObjectCompPtr, "AudioSampleObject", "Sample sequence object where current audio sample is stored", false, "AudioSequence");
		I_ASSIGN(m_tracePositionAttrPtr, "TracePosition", "If enabled, all position changes will be updated in preview objects", true, true);
		I_ASSIGN(m_grabProgressiveAttrPtr, "GrabProgressive", "If enabled the next frame will be returned by image and sample grab, if false only current frame will be returned", true, false);
		I_ASSIGN(m_minimalImageDistanceAttrPtr, "MinimalImageDistance", "Minimal distance between two grabbed images if progressive grabbing enabled", false, 0.1);
	I_END_COMPONENT();

	CLibAvVideoDecoderComp();
	virtual ~CLibAvVideoDecoderComp();

	// reimplemented (iproc::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

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
	enum FrameType
	{
		FT_ERROR = -1,
		FT_END,
		FT_AUDIO_SAMPLE,
		FT_IMAGE,
		FT_SKIPED_AUDIO_SAMPLE,
		FT_SKIPPED_IMAGE
	};

	FrameType ReadNextFrame(
				iimg::IBitmap* bitmapPtr,
				imeas::IDataSequence* audioSequencePtr,
				double minimalImagePos = -1,
				double minimalAudioPos = -1);

	bool TryTracePosition();

	// reimplemented (icomp::CComponentBase)
	void OnComponentCreated();
	void OnComponentDestroyed();

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

	std::vector<I_BYTE> m_imageBuffer;

	AVPacket m_packet;
	int m_bytesRemaining;
	uint8_t* m_rawDataPtr;

	istd::CString m_currentUrl;

	I_REF(iimg::IBitmap, m_bitmapObjectCompPtr);
	I_REF(imeas::IDataSequence, m_audioSampleObjectCompPtr);
	I_ATTR(bool, m_tracePositionAttrPtr);
	I_ATTR(bool, m_grabProgressiveAttrPtr);
	I_ATTR(double, m_minimalImageDistanceAttrPtr);

	int m_currentFrame;

	bool m_isCurrentImageValid;
	bool m_isCurrentSampleValid;
};


} // namespace ilibav


#endif // !ilibav_CLibAvVideoDecoderComp_included

