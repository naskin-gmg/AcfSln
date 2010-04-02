#include "icomp/export.h"

#include "FfMpegPck.h"


namespace FfMpegPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("Acf/Sln/FfMpeg", "Package based on FFMPEG library", "ImagingTools ACF Solutions MPEG Video Audio FFMPEG AvLib");

I_EXPORT_COMPONENT(
			LibAvVideoDecoder,
			"Video decoder based on libAv library",
			"Video Decoder Controller LibAv Bitmap Frame Grab AVI MPEG");


} // namespace FfMpegPck


