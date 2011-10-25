#include "FfMpegPck.h"


#include "icomp/export.h"


namespace FfMpegPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE(
			"Acf/Sln/FfMpeg",
			"Package based on FFMPEG library",
			"ACF Solutions MPEG Video Audio FFMPEG AvLib" IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			LibAvVideoDecoder,
			"Video decoder based on libAv library",
			"Video Decoder Controller LibAv Bitmap Frame Grab AVI MPEG" IM_TAG("Multimedia"));


} // namespace FfMpegPck


