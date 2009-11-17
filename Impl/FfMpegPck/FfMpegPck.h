#ifndef FfMpegPck_included
#define FfMpegPck_included


#include "icomp/TModelCompWrap.h"

#include "ilibav/CLibAvVideoDecoderComp.h"


/**
	Package based on FFMPEG library.
*/
namespace FfMpegPck
{


typedef icomp::TModelCompWrap<ilibav::CLibAvVideoDecoderComp> LibAvVideoDecoder;


} // namespace FfMpegPck


#endif // !FfMpegPck_included


