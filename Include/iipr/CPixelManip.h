#ifndef iipr_CPixelManip_included
#define iipr_CPixelManip_included


// Qt includes
#include <QtCore/QtGlobal>


namespace iipr
{


#pragma pack(push, 1)


/**
	Set pixel performance optimized classes designed to be used for pixel manipulation in template processing methods
*/
class CPixelManip
{
public:
	class Rgba
	{
	public:
		Rgba()
		{
		}

		Rgba(quint8 r, quint8 g, quint8 b, quint8 a = 255)
		:	m_r(r), m_g(g), m_b(b), m_a(a)
		{
		}

		quint8 m_r;
		quint8 m_g;
		quint8 m_b;
		quint8 m_a;
	};

	template <int Shift>
	class GrayCropAccum32
	{
	public:
		GrayCropAccum32()
		{
		}

		GrayCropAccum32(const GrayCropAccum32& value)
		:	m_gray(value.m_gray)
		{
		}

		GrayCropAccum32(int value)
		:	m_gray(qint32(value) << Shift)
		{
		}

		GrayCropAccum32(double value)
		:	m_gray(qint32(value * (1 << Shift)))
		{
		}

		bool operator==(const GrayCropAccum32& value)
		{
			return m_gray == value.m_gray;
		}

		bool operator==(int value)
		{
			return m_gray == (qint32(value) << Shift);
		}

		bool operator==(double value)
		{
			return m_gray == qint32(value * (1 << Shift));
		}

		bool operator!=(const GrayCropAccum32& value)
		{
			return m_gray != value.m_gray;
		}

		bool operator!=(int value)
		{
			return m_gray != (qint32(value) << Shift);
		}

		bool operator!=(double value)
		{
			return m_gray != qint32(value * (1 << Shift));
		}

		GrayCropAccum32& operator=(const GrayCropAccum32& value)
		{
			m_gray = value.m_gray;

			return *this;
		}

		GrayCropAccum32& operator=(int value)
		{
			m_gray = qint32(value) << Shift;

			return *this;
		}

		GrayCropAccum32& operator=(double value)
		{
			m_gray = qint32(value * (1 << Shift));

			return *this;
		}

		GrayCropAccum32& operator+=(const GrayCropAccum32& value)
		{
			m_gray += value.m_gray;

			return *this;
		}

		GrayCropAccum32& operator+=(qint32 value)
		{
			m_gray += qint32(value) << Shift;

			return *this;
		}

		GrayCropAccum32& operator+=(double value)
		{
			m_gray += qint32(value * (1 << Shift));

			return *this;
		}

		GrayCropAccum32& operator-=(const GrayCropAccum32& value)
		{
			m_gray -= value.m_gray;

			return *this;
		}

		GrayCropAccum32& operator-=(qint32 value)
		{
			m_gray -= qint32(value) << Shift;

			return *this;
		}

		GrayCropAccum32& operator-=(double value)
		{
			m_gray -= qint32(value * (1 << Shift));

			return *this;
		}

		GrayCropAccum32& operator*=(const GrayCropAccum32& value)
		{
			m_gray = qint32((quint64(m_gray) * quint64(value.m_gray)) >> Shift);

			return *this;
		}

		GrayCropAccum32& operator*=(qint32 value)
		{
			m_gray *= qint32(value);

			return *this;
		}

		GrayCropAccum32& operator*=(double value)
		{
			m_gray = qint32(m_gray * value);

			return *this;
		}

		GrayCropAccum32 operator/(qint32 value)
		{
			GrayCropAccum32 retVal;

			retVal.m_gray = m_gray / value;

			return retVal;
		}

		GrayCropAccum32 operator/(double value)
		{
			GrayCropAccum32 retVal;

			retVal.m_gray = qint32(m_gray / value);

			return retVal;
		}

		operator quint8()
		{
			if (m_gray < 0){
				return 0;
			}
			if (m_gray > (0xff << Shift)){
				return 0xff;
			}

			return quint8(m_gray >> Shift);
		}

		operator quint16()
		{
			if (m_gray < 0){
				return 0;
			}
			if (m_gray > (0xffff << Shift)){
				return 0xffff;
			}

			return quint16(m_gray >> Shift);
		}

		operator quint32()
		{
			if (m_gray < 0){
				return 0;
			}

			return quint32(m_gray >> Shift);
		}

		operator Rgba()
		{
			if (m_gray < 0){
				return 0;
			}
			if (m_gray > (0xffff << Shift)){
				return 0xffff;
			}

			quint8 grayValue = quint16(m_gray >> Shift);

			return Rgba(grayValue, grayValue, grayValue);
		}

	private:
		qint32 m_gray;
	};

	template <int Shift>
	class RgbCropAccum32
	{
	public:
		RgbCropAccum32()
		{
		}

		RgbCropAccum32(const RgbCropAccum32& value)
		:	m_r(value.m_r), m_g(value.m_g), m_b(value.m_b)
		{
		}

		RgbCropAccum32(const Rgba& value)
		:	m_r(qint32(value.m_r) << Shift), m_g(qint32(value.m_g) << Shift), m_b(qint32(value.m_b) << Shift)
		{
		}

		RgbCropAccum32(int value)
		:	m_r(qint32(value) << Shift), m_g(qint32(value) << Shift), m_b(qint32(value) << Shift)
		{
		}

		RgbCropAccum32(double value)
		:	m_r(qint32(value * (1 << Shift))), m_g(qint32(value * (1 << Shift))), m_b(qint32(value * (1 << Shift)))
		{
		}

		bool operator==(const RgbCropAccum32& value)
		{
			return (m_r == value.m_r) && (m_g == value.m_g) && (m_b == value.m_b);
		}

		bool operator==(int value)
		{
			qint32 gray = (qint32(value) << Shift);

			return (m_r == gray) && (m_g == gray) && (m_b == gray);
		}

		bool operator==(double value)
		{
			qint32 gray = qint32(value * (1 << Shift));

			return (m_r == gray) && (m_g == gray) && (m_b == gray);
		}

		bool operator!=(const RgbCropAccum32& value)
		{
			return (m_r != value.m_r) || (m_g != value.m_g) || (m_b != value.m_b);
		}

		bool operator!=(int value)
		{
			qint32 gray = (qint32(value) << Shift);

			return (m_r != gray) || (m_g != gray) || (m_b != gray);
		}

		bool operator!=(double value)
		{
			qint32 gray = qint32(value * (1 << Shift));

			return (m_r != gray) || (m_g != gray) || (m_b != gray);
		}

		RgbCropAccum32& operator=(const RgbCropAccum32& value)
		{
			m_r = value.m_r;
			m_g = value.m_g;
			m_b = value.m_b;

			return *this;
		}

		RgbCropAccum32& operator=(const Rgba& value)
		{
			m_r = qint32(value.m_r) << Shift;
			m_g = qint32(value.m_g) << Shift;
			m_b = qint32(value.m_b) << Shift;

			return *this;
		}

		RgbCropAccum32& operator=(int value)
		{
			m_r = qint32(value) << Shift;
			m_g = m_r;
			m_b = m_r;

			return *this;
		}

		RgbCropAccum32& operator=(double value)
		{
			m_r = qint32(value * (1 << Shift));
			m_g = m_r;
			m_b = m_r;

			return *this;
		}

		RgbCropAccum32& operator+=(const RgbCropAccum32& value)
		{
			m_r += value.m_r;
			m_g += value.m_g;
			m_b += value.m_b;

			return *this;
		}

		RgbCropAccum32& operator+=(const Rgba& value)
		{
			m_r += qint32(value.m_r) << Shift;
			m_g += qint32(value.m_g) << Shift;
			m_b += qint32(value.m_b) << Shift;

			return *this;
		}

		RgbCropAccum32& operator+=(qint32 value)
		{
			qint32 gray = qint32(value) << Shift;
			m_r += gray;
			m_g += gray;
			m_b += gray;

			return *this;
		}

		RgbCropAccum32& operator+=(double value)
		{
			qint32 gray = qint32(value * (1 << Shift));
			m_r += gray;
			m_g += gray;
			m_b += gray;

			return *this;
		}

		RgbCropAccum32& operator-=(const RgbCropAccum32& value)
		{
			m_r -= value.m_r;
			m_g -= value.m_g;
			m_b -= value.m_b;

			return *this;
		}

		RgbCropAccum32& operator-=(const Rgba& value)
		{
			m_r -= qint32(value.m_r) << Shift;
			m_g -= qint32(value.m_g) << Shift;
			m_b -= qint32(value.m_b) << Shift;

			return *this;
		}

		RgbCropAccum32& operator-=(qint32 value)
		{
			qint32 gray = qint32(value) << Shift;
			m_r -= gray;
			m_g -= gray;
			m_b -= gray;

			return *this;
		}

		RgbCropAccum32& operator-=(double value)
		{
			qint32 gray = qint32(value * (1 << Shift));
			m_r -= gray;
			m_g -= gray;
			m_b -= gray;

			return *this;
		}

		RgbCropAccum32& operator*=(const RgbCropAccum32& value)
		{
			m_r = qint32(quint64(m_r) * quint64(value.m_r) >> Shift);;
			m_g = qint32(quint64(m_g) * quint64(value.m_g) >> Shift);;
			m_b = qint32(quint64(m_b) * quint64(value.m_b) >> Shift);;

			return *this;
		}

		RgbCropAccum32& operator*=(qint32 value)
		{
			m_r *= qint32(value);
			m_g *= qint32(value);
			m_b *= qint32(value);

			return *this;
		}

		RgbCropAccum32& operator*=(double value)
		{
			m_r = qint32(m_r * value);
			m_g = qint32(m_g * value);
			m_b = qint32(m_b * value);

			return *this;
		}

		RgbCropAccum32 operator/(qint32 value)
		{
			RgbCropAccum32 retVal;

			retVal.m_r = m_r / value;
			retVal.m_g = m_g / value;
			retVal.m_b = m_b / value;

			return retVal;
		}

		RgbCropAccum32 operator/(double value)
		{
			RgbCropAccum32 retVal;

			retVal.m_r = qint32(m_r / value);
			retVal.m_g = qint32(m_g / value);
			retVal.m_b = qint32(m_b / value);

			return retVal;
		}

		operator quint8()
		{
			quint64 grayValue = (quint64(m_r) + quint64(m_g) + quint64(m_b)) / 3;

			if (grayValue < 0){
				return 0;
			}
			if (grayValue > (0xff << Shift)){
				return 0xff;
			}

			return quint8(grayValue >> Shift);
		}

		operator quint16()
		{
			quint64 grayValue = (quint64(m_r) + quint64(m_g) + quint64(m_b)) / 3;

			if (grayValue < 0){
				return 0;
			}
			if (grayValue > (0xffff << Shift)){
				return 0xffff;
			}

			return quint16(grayValue >> Shift);
		}

		operator quint32()
		{
			quint64 grayValue = (quint64(m_r) + quint64(m_g) + quint64(m_b)) / 3;

			if (grayValue < 0){
				return 0;
			}

			return quint32(grayValue >> Shift);
		}

		operator float()
		{
			return (float(m_r) + float(m_g) + float(m_b)) / (3 << Shift);
		}

		operator double()
		{
			return (double(m_r) + double(m_g) + double(m_b)) / (3 << Shift);
		}

		operator Rgba()
		{
			qint32 red = m_r;
			if (red < 0){
				red = 0;
			}
			else if (red > (0xff << Shift)){
				red = 0xff;
			}

			qint32 green = m_g;
			if (green < 0){
				green = 0;
			}
			else if (green > (0xff << Shift)){
				green = 0xff;
			}

			qint32 blue = m_b;
			if (blue < 0){
				blue = 0;
			}
			else if (blue > (0xff << Shift)){
				blue = 0xff;
			}

			return Rgba(quint8(red >> Shift), quint8(green >> Shift), quint8(blue >> Shift));
		}

	private:
		qint32 m_r;
		qint32 m_g;
		qint32 m_b;
	};
};


#pragma pack(pop)


} // namespace iipr


#endif // !iipr_CPixelManip_included


