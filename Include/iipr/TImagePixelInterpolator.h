#ifndef iipr_TImagePixelInterpolator_included
#define iipr_TImagePixelInterpolator_included


// ACF includes
#include "istd/istd.h"


// IACF includes
#include "iipr/IImageInterpolationParams.h"


namespace iimg
{
	class IBitmap;
}


namespace iipr
{


template <typename PixelComponentType>
class TImagePixelInterpolator
{
public:
	typedef PixelComponentType PixelComponent;

	TImagePixelInterpolator(const iimg::IBitmap& image, int interpolationMode);

	PixelComponentType GetInterpolatedValue(double x, double y, int componentIndex) const;

private:
	PixelComponentType GetBilinearInterpolated(double x, double y, int componentIndex) const;
	PixelComponentType GetBitmapPixelValue(int x, int y, int componentIndex) const;

private:
	PixelComponentType* m_imageBufferPtr;
	int m_imageLineDifference;
	int m_imageHeight;
	int m_imageWidth;
	int m_interpolationMode;
	int m_pixelBytesCount;
	int m_maxValue;
};


// public methods

template <typename PixelComponentType>
TImagePixelInterpolator<PixelComponentType>::TImagePixelInterpolator(const iimg::IBitmap& bitmap, int interpolationMode)
{
	m_imageBufferPtr = (PixelComponentType*)bitmap.GetLinePtr(0);
	m_imageLineDifference = bitmap.GetLinesDifference();
	m_imageHeight = bitmap.GetImageSize().GetY();
	m_imageWidth = bitmap.GetImageSize().GetX();
	m_pixelBytesCount = bitmap.GetPixelBitsCount() / 8;
	m_interpolationMode = interpolationMode;
	m_maxValue = int(pow(2.0, double(sizeof(PixelComponentType) * 8)) - 1);
}


template <typename PixelComponentType>
typename TImagePixelInterpolator<PixelComponentType>::PixelComponent TImagePixelInterpolator<PixelComponentType>::GetInterpolatedValue(double x, double y, int componentIndex) const
{
	switch (m_interpolationMode){
		case iipr::IImageInterpolationParams::IM_BILINEAR:
			return GetBilinearInterpolated(x, y, componentIndex);
	}

	return GetBitmapPixelValue(int(x), int(y), componentIndex);
}


// private methods
	
template <typename PixelComponentType>
typename TImagePixelInterpolator<PixelComponentType>::PixelComponent TImagePixelInterpolator<PixelComponentType>::GetBilinearInterpolated(double x, double y, int componentIndex) const
{
	int cx = int(::ceil(x));
	int cy = int(::ceil(y));

	int fx = int(::floor(x));
	int fy = int(::floor(y));

	double dx = cx - x;
	double dy = cy - y;

	double v1 = (1.0 - dx) * (1.0 - dy) * GetBitmapPixelValue(fx, fy, componentIndex);
	double v2 = (dx) * (1.0 - dy) * GetBitmapPixelValue(cx, fy, componentIndex);
	double v3 = (dy) * (1.0 - dx) * GetBitmapPixelValue(fx, cy, componentIndex);
	double v4 = (dy) * (dx) * GetBitmapPixelValue(cx, cy, componentIndex);

	double retVal = v1 + v2 + v3 + v4;
	if (retVal > m_maxValue){
		retVal = m_maxValue;
	}

	return PixelComponentType(retVal);
}


template <typename PixelComponentType>
typename TImagePixelInterpolator<PixelComponentType>::PixelComponent TImagePixelInterpolator<PixelComponentType>::GetBitmapPixelValue(int x, int y, int componentIndex) const
{
	if (x < 0){
		x = 0;
	}

	if (x >= m_imageWidth){
		x = m_imageWidth - 1;
	}

	if (y < 0){
		y = 0;
	}

	if (y >= m_imageHeight){
		y = m_imageHeight - 1;
	}

	return *(m_imageBufferPtr + x * m_pixelBytesCount + componentIndex + m_imageLineDifference * y);
}


} // namespace iipr


#endif // !iipr_TImagePixelInterpolator_included


