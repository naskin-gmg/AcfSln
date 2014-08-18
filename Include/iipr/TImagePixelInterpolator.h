#ifndef iipr_TImagePixelInterpolator_included
#define iipr_TImagePixelInterpolator_included


// Qt includes
#include <QtCore/qmath.h>

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

	PixelComponentType GetInterpolatedValue(float x, float y, int componentIndex) const;

private:
	PixelComponentType GetBilinearInterpolated(float x, float y, int componentIndex) const;
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
	m_maxValue = int(qPow(2.0, double(sizeof(PixelComponentType) * 8)) - 1);
}


template <typename PixelComponentType>
typename TImagePixelInterpolator<PixelComponentType>::PixelComponent TImagePixelInterpolator<PixelComponentType>::GetInterpolatedValue(float x, float y, int componentIndex) const
{
	switch (m_interpolationMode){
		case iipr::IImageInterpolationParams::IM_NO_INTERPOLATION:
			return GetBitmapPixelValue(int(x), int(y), componentIndex);

		case iipr::IImageInterpolationParams::IM_BILINEAR:
			return GetBilinearInterpolated(x, y, componentIndex);

		default:
			I_CRITICAL();
	}

	return GetBitmapPixelValue(int(x), int(y), componentIndex);
}


// private methods
	
template <typename PixelComponentType>
typename TImagePixelInterpolator<PixelComponentType>::PixelComponent TImagePixelInterpolator<PixelComponentType>::GetBilinearInterpolated(float x, float y, int componentIndex) const
{
	int px = qFloor(x);
	int py = qFloor(y);

	// Calculate the weights for each pixel:
	float fx = x - px;
	float fy = y - py;
	float fx1 = 1.0f - fx;
	float fy1 = 1.0f - fy;

	float w1 = fx1 * fy1;
	float w2 = fx * fy1;
	float w3 = fx1 * fy;
	float w4 = fx * fy;

	float v1 = w1 * GetBitmapPixelValue(px, py, componentIndex);
	float v2 = w2 * GetBitmapPixelValue(px + 1, py, componentIndex);
	float v3 = w3 * GetBitmapPixelValue(px, py + 1, componentIndex);
	float v4 = w4 * GetBitmapPixelValue(px + 1, py + 1, componentIndex);

	float retVal = v1 + v2 + v3 + v4;
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


