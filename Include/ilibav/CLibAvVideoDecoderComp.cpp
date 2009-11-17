#include "ilibav/CLibAvVideoDecoderComp.h"


#include "ilibav/CLibAvConverter.h"


// LIBAV includes
extern "C"{
#include "libavcodec/imgconvert.h"
}

// ACF includes
#include "istd/TChangeNotifier.h"
#include "iprm/IFileNameParam.h"


namespace ilibav
{


// public methods

CLibAvVideoDecoderComp::CLibAvVideoDecoderComp()
:	m_videoStreamId(-1),
	m_formatContextPtr(NULL),
	m_codecContextPtr(NULL),
	m_codecPtr(NULL),
	m_framePtr(NULL),
	m_frameRgbPtr(NULL),
	m_bytesRemaining(0),
	m_rawDataPtr(NULL)

{
	av_register_all();

	m_packet.data = NULL;
}


// reimplemented (icomp::IComponent)

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


// reimplemented (iproc::IBitmapAcquisition)

istd::CIndex2d CLibAvVideoDecoderComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return GetFrameSize();
}


// reimplemented (iproc::IProcessor)

int CLibAvVideoDecoderComp::DoProcessing(
			const iprm::IParamsSet* /*paramsPtr*/,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (		(bitmapPtr != NULL) &&
				m_imageBufferPtr.IsValid() &&
				ReadNextFrame()){
		if (CLibAvConverter::ConvertBitmap(*m_frameRgbPtr, GetFrameSize(), *bitmapPtr)){
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
	istd::CChangeNotifier notifier(this, CF_STATUS);

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
		if ((codecContextPtr != NULL) && (codecContextPtr->codec_type == CODEC_TYPE_VIDEO)){
			m_videoStreamId = i;
			m_codecContextPtr = codecContextPtr;

			// Determine required buffer size and allocate buffer
			int numBytes = avpicture_get_size(
						PIX_FMT_RGB32,
						m_codecContextPtr->width,
						m_codecContextPtr->height);
			m_imageBufferPtr.SetPtr(new I_BYTE[numBytes]);

			// Assign appropriate parts of buffer to image planes in pFrameRGB
			avpicture_fill(
						(AVPicture*)m_frameRgbPtr,
						m_imageBufferPtr.GetPtr(),
						PIX_FMT_RGB32,
						m_codecContextPtr->width,
						m_codecContextPtr->height);

			// Find the decoder for the video stream
			m_codecPtr = avcodec_find_decoder(m_codecContextPtr->codec_id);
			if (m_codecPtr==NULL){
				SendInfoMessage(MI_FORMAT_PROBLEM, istd::CString("No decoder found for file ") + url);

				return false;
			}

			// Inform the codec that we can handle truncated bitstreams -- i.e.,
			// bitstreams where frame boundaries can fall in the middle of packets
			if (m_codecPtr->capabilities & CODEC_CAP_TRUNCATED){
				m_codecContextPtr->flags |= CODEC_FLAG_TRUNCATED;
			}

			// Open codec
			if (avcodec_open(m_codecContextPtr, m_codecPtr) < 0){
				SendInfoMessage(MI_FORMAT_PROBLEM, istd::CString("Cannot open codec for file ") + url);

				return false;
			}

			return true;
		}
	}

	SendInfoMessage(MI_FORMAT_PROBLEM, istd::CString("No video decoder found for file ") + url);

	return false;
}


void CLibAvVideoDecoderComp::CloseMedium()
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	if (m_formatContextPtr != NULL){
		av_close_input_file(m_formatContextPtr);

		m_formatContextPtr = NULL;
	}

	m_codecContextPtr = NULL;

	m_packet.data = NULL;
	m_bytesRemaining = 0;
	m_rawDataPtr = NULL;

	m_imageBufferPtr.Reset();
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
			return double(videoStreamPtr->cur_dts * videoStreamPtr->time_base.num) / videoStreamPtr->time_base.den;
		}
	}

	return 0;
}


bool CLibAvVideoDecoderComp::SetCurrentPosition(double position)
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if (videoStreamPtr != NULL){
			return av_seek_frame(
						m_formatContextPtr,
						m_videoStreamId,
						int64_t(position * videoStreamPtr->time_base.den / videoStreamPtr->time_base.num),
						0) >= 0;
		}
	}

	return false;
}


int	CLibAvVideoDecoderComp::GetSupportedFeatures() const
{
	return SF_SEEK | SF_GRAB_CURRENT;
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
	if (m_codecContextPtr != NULL){
		return istd::CIndex2d(m_codecContextPtr->width, m_codecContextPtr->height);
	}

	return istd::CIndex2d::GetInvalid();
}


double CLibAvVideoDecoderComp::GetPixelAspectRatio() const
{
	if ((m_codecContextPtr != NULL) && (m_codecContextPtr->sample_aspect_ratio.num != 0)){
		AVRational ratio = m_codecContextPtr->sample_aspect_ratio;
		if (ratio.den != 0){
			return double(ratio.num) / ratio.den;
		}
	}

	return -1;
}


// reimplemented (imm::IVideoController)

int CLibAvVideoDecoderComp::GetCurrentFrame() const
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		AVStream* videoStreamPtr = m_formatContextPtr->streams[m_videoStreamId];
		if (videoStreamPtr != NULL){
			return int(videoStreamPtr->cur_dts);
		}
	}

	return 0;
}


bool CLibAvVideoDecoderComp::SetCurrentFrame(int frameIndex)
{
	if ((m_formatContextPtr != 0) && (m_videoStreamId >= 0)){
		return av_seek_frame(
					m_formatContextPtr,
					m_videoStreamId,
					frameIndex,
					0) >= 0;
	}

	return false;
}


// protected methods

bool CLibAvVideoDecoderComp::ReadNextFrame()
{
	if (		(m_formatContextPtr == NULL) ||
				(m_codecContextPtr == NULL) ||
				(m_framePtr == NULL) ||
				(m_frameRgbPtr == NULL)){
		return false;
	}

	// Decode packets until we have decoded a complete frame
	while (true){
		// Work on the current packet until we have decoded all of it
		while (m_bytesRemaining > 0){
			// Decode the next chunk of data
			int frameFinished = 0;
			int bytesDecoded = avcodec_decode_video(m_codecContextPtr, m_framePtr, &frameFinished, m_rawDataPtr, m_bytesRemaining);

			// Was there an error?
			if (bytesDecoded < 0){
				return false;
			}

			m_bytesRemaining -= bytesDecoded;
			m_rawDataPtr += bytesDecoded;

			// Did we finish the current frame? Then we can return
			if (frameFinished != 0){
				img_convert((AVPicture*)m_frameRgbPtr,
							PIX_FMT_RGB32,
							(AVPicture*)m_framePtr,
							m_codecContextPtr->pix_fmt,
							m_codecContextPtr->width,
							m_codecContextPtr->height);

				istd::CChangeNotifier notifier(this, CF_MEDIA_POSITION);

				return true;
			}
		}

		// Read the next packet, skipping all packets that aren't for this
		// stream
		do{
			// Free old packet
			if (m_packet.data != NULL){
				av_free_packet(&m_packet);
			}

			// Read new packet
			if (av_read_frame(m_formatContextPtr, &m_packet) < 0){
				return false;
			}
		} while (m_packet.stream_index != m_videoStreamId);

		m_bytesRemaining = m_packet.size;
		m_rawDataPtr = m_packet.data;
	}
}


} // namespace ilibav


