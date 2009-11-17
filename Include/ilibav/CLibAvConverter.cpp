#include "ilibav/CLibAvConverter.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace ilibav
{


// static methods

bool CLibAvConverter::ConvertBitmap(const AVFrame& avFrame, const istd::CIndex2d& size, iimg::IBitmap& result)
{
	istd::CChangeNotifier notifier(&result);

	if (result.CreateBitmap(size, 32, 4)){
		const I_BYTE* sourcePtr = avFrame.data[0];
		for (int y = 0; y < size[1]; ++y, sourcePtr += avFrame.linesize[0]){
			::memcpy(result.GetLinePtr(y), sourcePtr, result.GetLineBytesCount());
		}

		return true;
	}

	return false;
}


} // namespace ilibav


