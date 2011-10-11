#ifndef ilibav_CLibAvConverter_included
#define ilibav_CLibAvConverter_included


// LIBAV includes
extern "C"{
#include "libavformat/avformat.h"
}

// ACF includes
#include "istd/CIndex2d.h"
#include "iimg/IBitmap.h"


namespace ilibav
{


/**
	Helper class containing static methods to convert between AvLib and ACF classes.
*/
class CLibAvConverter
{
public:
	static bool ConvertBitmap(const AVFrame& avFrame, const istd::CIndex2d& size, int frameFormat, iimg::IBitmap& result);

protected:
	struct Rgba{
		I_BYTE r;
		I_BYTE g;
		I_BYTE b;
		I_BYTE a;
	};
};


} // namespace ilibav


#endif // !ilibav_CLibAvConverter_included


