#include "ilibav/CLibAvVideoDecoderComp.h"


// STL includes
#include <cstring>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TSmartPtr.h"
#include "iprm/IFileNameParam.h"

#include "imeas/CSamplesInfo.h"

#include "ilibav/CLibAvConverter.h"


namespace ilibav
{


// public methods

CLibAvVideoDecoderComp::CLibAvVideoDecoderComp()
:	m_videoStreamId(-1),
	m_audioStreamId(-1),
	m_formatContextPtr(NULL),
	m_videoCodecContextPtr(NULL),
	m_audioCodecContextPtr(NULL),
	m_videoCodecPtr(NULL),
	m_audioCodecPtr(NULL),
	m_framePtr(NULL),
	m_frameRgbPtr(NULL),
	m_bytesRemaining(0),
	m_rawDataPtr(NULL),
	m_currentFrame(0)
{
	av_register_all();

	m_packet.data = NULL;

	m_audioInputBuffer = (uint8_t*)av_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE);
	m_audioOutputBuffer = (int16_t*)av_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);

	m_ignoreFirstAudioFrame = false;
}


CLibAvVideoDecoderComp::~CLibAvVideoDecoderComp()
{
	av_free(m_audioInputBuffer);
	av_free(m_audioOutputBuffer);
}


// reimplemented (iproc::IBitmapAcquisition)

istd::CIndex2d CLibAvVideoDecoderComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return GetFrameSize();
}


// reimplemented (iproc::IProcessor)

int CLibAvVideoDecoderComp::DoProcessing(
			const iprm::IParamsSet* /*paramsPtr*/,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (		(bitmapPtr != NULL) &&
				m_bitmapObjectCompPtr.IsValid()){
		if (bitmapPtr->CopyFrom(*m_bitmapObjectCompPtr)){
			return TS_OK;
		}
	}

	imeas::IDataSequence* audioSequencePtr = dynamic_cast<imeas::IDataSequence*>(outputPtr);
	if (		(audioSequencePtr != NULL) &&
				m_audioSequenceCompPtr.IsValid()){
		if (audioSequencePtr->CopyFrom(*m_audioSequenceCompPtr)){
			return TS_OK;
		}
	}

	return TS_INVALID;
}


// reimplemented (imm::IMediaController)

istd::CString CLibAvVideoDecoderComp::GetOpenedMediumUrl() const
{
	return m_currentUrl;
}


bool CLibAvVideoDecoderComp::OpenMediumUrl(const istd::CString& url, bool /*autoPlay*/)
{
	istd::CChangeNotifier notifier(this, CF_STATUS | CF_MEDIA_POSITION);

	CloseMedium();

	if (		(av_open_input_file(&m_formatContextPtr, url.ToString().c_str(), NULL, 0, NULL) != 0) ||
				(m_formatContextPtr == NULL)){
		SendInfoMessage(MI_CANNOT_OPEN, istd::CString("Cannot open media file ") + url);

		return false;
	}

	if (av_find_stream_info(m_formatContextPtr) < 0){
		SendInfoMessage(MI_FORMAT_PROBLEM, istd::CString("No stream info for file ") + url);

		return false;
	}

	m_currentUrl = url;

	// Find the first video stream
	for (unsigned int i = 0; i < m_formatContextPtr->nb_streams; i++){
		AVCodecContext* codecContextPtr = m_formatContextPtr->streams[i]->codec;
		if (codecContextPtr == NULL){
			continue;
		}

		if (m_bitmapObjectCompPtr.IsValid() && (m_videoCodecPtr == NULL) && (codecContextPtr->codec_type == CODEC_TYPE_VIDEO)){
			m_videoStreamId = i;
			m_videoCodecContextPtr = codecContextPtr;

			// Determine required buffer size and allocate buffer
			int numBytes = avpicture_get_size(
						PIX_FMT_RGB32,
						m_videoCodecContextPtr->width,
						m_videoCodecContextPtr->height);
			m_imageBuffer.resize(numBytes);

			// Assign appropriate parts of buffer to image planes in pFrameRGB
			avpicture_fill(
						(AVPicture*)m_frameRgbPtr,
						&m_imageBuffer[0],
						PIX_FMT_RGB32,
						m_videoCodecContextPtr->width,
						m_videoCodecContextPtr->height);

			// Find the decoder for the video stream
			m_videoCodecPtr = avcodec_find_decoder(m_videoCodecContextPtr->codec_id);
			if (m_videoCodecPtr == NULL){
				continue;
			}

			// Inform the codec that we can handle truncated bitstreams -- i.e.,
			// bitstreams where frame boundaries can fall in the middle of packets
			if (m_videoCodecPtr->capabilities & CODEC_CAP_TRUNCATED){
				m_videoCodecContextPtr->flags |= CODEC_FLAG_TRUNCATED;
			}

			// Open codec
			if (avcodec_open(m_videoCodecContextPtr, m_videoCodecPtr) < 0){
				SendInfoMessage(MI_FORMAT_PROBLEM, istd::CString("Cannot open codec for file ") + url);

				m_videoCodecPtr = NULL;
			}
		}
		else if (m_audioSequenceCompPtr.IsValid() && (m_audioCodecPtr == NULL) && (codecContextPtr->codec_type == CODEC_TYPE_AUDIO)){
			m_audioStreamId = i;

			// Find the decoder for the video stream
			m_audioCodecPtr = avcodec_find_decoder(codecContextPtr->codec_id);
			if (m_audioCodecPtr == NULL){
				continue;
			}

			// Inform the codec that we can handle truncated bitstreams -- i.e.,
			// bitstreams where frame boundaries can fall in the middle of packets
			if (m_audioCodecPtr->capabilities & CODEC_CAP_TRUNCATED){
				codecContextPtr->flags |= CODEC_FLAG_TRUNCATED;
			}

			// Open codec
			if (avcodec_open(codecContextPtr, m_audioCodecPtr) >= 0){
				m_audioCodecContextPtr = codecContextPtr;
				m_ignoreFirstAudioFrame = false;

				if (m_audioSequenceCompPtr.IsValid() && m_autoAudioGrabLengthAttrPtr.IsValid()){
					if (m_audioCodecContextPtr->sample_rate > 0){
						int samplesCount = int(*m_autoAudioGrabLengthAttrPtr * m_audioCodecContextPtr->sample_rate);
						int channelsCount = m_audioCodecContextPtr->channels;
						istd::CRange resultProportionRange(0, double(samplesCount) / m_audioCodecContextPtr->sample_rate);
						m_audioSequenceCompPtr->CreateSequenceWithInfo(
									istd::TSmartPtr<const imeas::IDataSequenceInfo>(new imeas::CSamplesInfo(resultProportionRange)),
									samplesCount,
									channelsCount);
					}
				}
			}
			else{
				SendInfoMessage(MI_FORMAT_PROBLEM, istd::CString("Cannot open codec for file ") + url);

				m_audioCodecPtr = NULL;
			}
		}
	}

	if (m_bitmapObjectCompPtr.IsValid() && (m_videoCodecPtr == NULL)){
		SendInfoMessage(MI_FORMAT_PROBLEM, istd::CString("No video decoder found for file ") + url);
	}

	if (m_audioSequenceCompPtr.IsValid() && (m_audioCodecPtr == NULL)){
		SendInfoMessage(MI_FORMAT_PROBLEM, istd::CString("No audio decoder found for file ") + url);
	}

	ReadNextFrame();

	return true;
}


void CLibAvVideoDecoderComp::CloseMedium()
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	if (m_formatContextPtr != NULL){
		av_close_input_file(m_formatContextPtr);

		m_formatContextPtr = NULL;
	}

	m_videoCodecContextPtr = NULL;
	m_audioCodecContextPtr = NULL;
	m_videoCodecPtr = NULL;
	m_audioCodecPtr = NULL;

	m_packet.data = NULL;
	m_bytesRemaining = 0;
	m_rawDataPtr = NULL;

	m_currentFrame = 0;

	m_imageBuffer.clear();
}


bool CLibAvVideoDecoderComp::IsPlaying() const
{
	return false;
}


bool CLibAvVideoDecoderComp::SetPlaying(bool /*state*/)
{
	return false;
}


double CLibAvVideoDecoderComp::GetMediumLength() const
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if (videoStreamPtr != NULL){
			return double(videoStreamPtr->duration * videoStreamPtr->time_base.num) / videoStreamPtr->time_base.den;
		}
	}

	return 0;
}


double CLibAvVideoDecoderComp::GetCurrentPosition() const
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if (videoStreamPtr != NULL){
			return double(GetCurrentFrame() * videoStreamPtr->time_base.num) / videoStreamPtr->time_base.den;
		}
	}

	return 0;
}


bool CLibAvVideoDecoderComp::SetCurrentPosition(double position)
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if (videoStreamPtr != NULL){
			return SetCurrentFrame(int(position * videoStreamPtr->time_base.den / videoStreamPtr->time_base.num));
		}
	}

	return false;
}


int	CLibAvVideoDecoderComp::GetSupportedFeatures() const
{
	return SF_OPEN_MEDIA | SF_SEEK;
}


// reimplemented (imm::IVideoInfo)

int CLibAvVideoDecoderComp::GetFramesCount() const
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if (videoStreamPtr != NULL){
			return int(videoStreamPtr->duration);
		}
	}

	return 0;
}


double CLibAvVideoDecoderComp::GetFrameIntervall() const
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if ((videoStreamPtr != NULL) && (videoStreamPtr->time_base.den != 0)){
			return double(videoStreamPtr->time_base.num) / videoStreamPtr->time_base.den;
		}
	}

	return -1;
}


istd::CIndex2d CLibAvVideoDecoderComp::GetFrameSize() const
{
	if (m_videoCodecContextPtr != NULL){
		return istd::CIndex2d(m_videoCodecContextPtr->width, m_videoCodecContextPtr->height);
	}

	return istd::CIndex2d::GetInvalid();
}


double CLibAvVideoDecoderComp::GetPixelAspectRatio() const
{
	if ((m_videoCodecContextPtr != NULL) && (m_videoCodecContextPtr->sample_aspect_ratio.num != 0)){
		AVRational ratio = m_videoCodecContextPtr->sample_aspect_ratio;
		if (ratio.den != 0){
			return double(ratio.num) / ratio.den;
		}
	}

	return -1;
}


// reimplemented (imm::IVideoController)

int CLibAvVideoDecoderComp::GetCurrentFrame() const
{
	return m_currentFrame;
}


bool CLibAvVideoDecoderComp::SetCurrentFrame(int frameIndex)
{
	if (frameIndex == m_currentFrame){
		return true;
	}

	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		istd::CChangeNotifier notifier(this, CF_MEDIA_POSITION);

		if (av_seek_frame(
					m_formatContextPtr,
					m_videoStreamId,
					frameIndex,
					0) >= 0){
			ReadNextFrame();

			m_currentFrame = frameIndex;
			m_ignoreFirstAudioFrame = true;

			return true;
		}
	}

	return false;
}


// protected methods

bool CLibAvVideoDecoderComp::ReadNextFrame()
{
	if (		(m_formatContextPtr == NULL) ||
				(m_videoCodecContextPtr == NULL) ||
				(m_framePtr == NULL) ||
				(m_frameRgbPtr == NULL)){
		return false;
	}

	bool retVal = true;

	bool needVideoFrame = (m_videoStreamId >= 0) && (m_videoCodecContextPtr != NULL);
	bool needAudioFrame = (m_audioStreamId >= 0) && (m_audioCodecContextPtr != NULL);

	int samplesOffset = 0;

	istd::CChangeNotifier notifier(m_audioSequenceCompPtr.GetPtr());

	// Decode packets until we have decoded a complete frame
	while (retVal && (needVideoFrame || needAudioFrame)){
		while ((m_rawDataPtr != NULL) && (m_bytesRemaining > 0)){
			if (needVideoFrame && (m_packet.stream_index == m_videoStreamId)){
				I_ASSERT(m_videoCodecContextPtr != NULL);

				int bytesToCopy = istd::Min(m_bytesRemaining, AVCODEC_MAX_AUDIO_FRAME_SIZE);
				std::memset(m_audioInputBuffer + bytesToCopy, 0, FF_INPUT_BUFFER_PADDING_SIZE);
				std::memcpy(m_audioInputBuffer, m_rawDataPtr, bytesToCopy);

				// Decode the next chunk of data
				int frameFinished = 0;
				int bytesDecoded = avcodec_decode_video(
							m_videoCodecContextPtr,
							m_framePtr,
							&frameFinished,
							m_audioInputBuffer,
							bytesToCopy);

				// Was there an error?
				if (bytesDecoded > 0){
					m_bytesRemaining -= bytesDecoded;
					m_rawDataPtr += bytesDecoded;

					// Did we finish the current frame? Then we can return
					if (frameFinished != 0){
						I_ASSERT(m_bitmapObjectCompPtr.IsValid());
						if (CLibAvConverter::ConvertBitmap(
									*m_framePtr,
									GetFrameSize(),
									m_videoCodecContextPtr->pix_fmt,
									*m_bitmapObjectCompPtr)){
							needVideoFrame = false;
						}
					}

					continue;
				}
				else{
//					retVal = false;
					break;
				}
			}
			else if (needAudioFrame && (m_packet.stream_index == m_audioStreamId)){
				I_ASSERT(m_audioSequenceCompPtr.IsValid());
				I_ASSERT(m_audioCodecContextPtr != NULL);

				int sampleRawSize = 0;

				switch (m_audioCodecContextPtr->sample_fmt){
				case SAMPLE_FMT_U8:
					sampleRawSize = sizeof(I_BYTE);
					break;

				case SAMPLE_FMT_S16:
					sampleRawSize = sizeof(I_SWORD);
					break;

				case SAMPLE_FMT_S32:
					sampleRawSize = sizeof(I_SDWORD);
					break;

				case SAMPLE_FMT_FLT:
					sampleRawSize = sizeof(float);
					break;

				case SAMPLE_FMT_DBL:
					sampleRawSize = sizeof(double);
					break;
				}

				int bytesToCopy = istd::Min(m_bytesRemaining, AVCODEC_MAX_AUDIO_FRAME_SIZE);
				std::memset(m_audioInputBuffer + bytesToCopy, 0, FF_INPUT_BUFFER_PADDING_SIZE);
				std::memcpy(m_audioInputBuffer, m_rawDataPtr, bytesToCopy);

				// Decode the next chunk of data
				int audioBufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;

				int bytesDecoded = avcodec_decode_audio2(
							m_audioCodecContextPtr,
							m_audioOutputBuffer,
							&audioBufferSize,
							m_audioInputBuffer,
							bytesToCopy);

				// Was there an error?
				if (bytesDecoded > 0){
					m_bytesRemaining -= bytesDecoded;
					m_rawDataPtr += bytesDecoded;

					if (!m_ignoreFirstAudioFrame){
						int channelsCount = m_audioCodecContextPtr->channels;

						int remainSamples = m_audioSequenceCompPtr->GetSamplesCount() - samplesOffset;
						int samplesToCopy = istd::Min(remainSamples, audioBufferSize / sampleRawSize);

						switch (m_audioCodecContextPtr->sample_fmt){
						case SAMPLE_FMT_U8:
							{
								I_BYTE* samplesPtr = (I_BYTE*)m_audioOutputBuffer;
								for (int i = 0; i < samplesToCopy; ++i){
									m_audioSequenceCompPtr->SetSample(samplesOffset + i / channelsCount, i % channelsCount, samplesPtr[i] / 127.5 - 1);
								}
								samplesOffset += samplesToCopy / channelsCount;
							}
							break;

						case SAMPLE_FMT_S16:
							{
								I_SWORD* samplesPtr = (I_SWORD*)m_audioOutputBuffer;
								for (int i = 0; i < samplesToCopy; ++i){
									m_audioSequenceCompPtr->SetSample(samplesOffset + i / channelsCount, i % channelsCount, samplesPtr[i] / 32768.0);
								}
								samplesOffset += samplesToCopy / channelsCount;
							}
							break;

						case SAMPLE_FMT_S32:
							{
								I_SDWORD* samplesPtr = (I_SDWORD*)m_audioOutputBuffer;
								for (int i = 0; i < samplesToCopy; ++i){
									m_audioSequenceCompPtr->SetSample(samplesOffset + i / channelsCount, i % channelsCount, double(samplesPtr[i]) / 0x80000000);
								}
								samplesOffset += samplesToCopy / channelsCount;
							}
							break;

						case SAMPLE_FMT_FLT:
							{
								float* samplesPtr = (float*)m_audioOutputBuffer;
								for (int i = 0; i < samplesToCopy; ++i){
									m_audioSequenceCompPtr->SetSample(samplesOffset + i / channelsCount, i % channelsCount, samplesPtr[i]);
								}
								samplesOffset += samplesToCopy / channelsCount;
							}
							break;

						case SAMPLE_FMT_DBL:
							{
								double* samplesPtr = (double*)m_audioOutputBuffer;
								for (int i = 0; i < samplesToCopy; ++i){
									m_audioSequenceCompPtr->SetSample(samplesOffset + i / channelsCount, i % channelsCount, samplesPtr[i]);
								}
								samplesOffset += samplesToCopy / channelsCount;
							}
							break;
						}

						if (samplesOffset >= m_audioSequenceCompPtr->GetSamplesCount()){
							needAudioFrame = false;
						}
					}
					else{
						m_ignoreFirstAudioFrame = false;
					}

					continue;
				}
				else{
					break;
//					retVal = false;
				}
			}
			else{
				break;
			}
		}

		if (m_packet.data != NULL){
			av_free_packet(&m_packet);
			m_packet.data = NULL;
			m_rawDataPtr = NULL;
		}

		// Read new packet
		if ((av_read_frame(m_formatContextPtr, &m_packet) < 0) || (m_packet.data == NULL)){
			return false;
		}

		m_bytesRemaining = m_packet.size;
		m_rawDataPtr = m_packet.data;
	}

	return retVal;
}


// reimplemented (icomp::CComponentBase)

void CLibAvVideoDecoderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_framePtr = avcodec_alloc_frame();
	m_frameRgbPtr = avcodec_alloc_frame();
}


void CLibAvVideoDecoderComp::OnComponentDestroyed()
{
	CloseMedium();

	if (m_framePtr != NULL){
		av_free(m_framePtr);

		m_framePtr = NULL;
	}

	if (m_frameRgbPtr != NULL){
		av_free(m_frameRgbPtr);

		m_frameRgbPtr = NULL;
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ilibav


