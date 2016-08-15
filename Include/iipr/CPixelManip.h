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

	template <typename IntType, int Shift, bool DoCropMin = true, bool DoCropMax = true>
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
		:	m_gray(IntType(value) << Shift)
		{
		}

		GrayCropAccum32(quint8 value)
		:	m_gray(IntType(value) << Shift)
		{
		}

		GrayCropAccum32(quint16 value)
		:	m_gray(IntType(value) << Shift)
		{
		}

		GrayCropAccum32(quint32 value)
		:	m_gray(IntType(value) << Shift)
		{
		}

		GrayCropAccum32(quint64 value)
		:	m_gray(IntType(value) << Shift)
		{
		}

		GrayCropAccum32(double value)
		:	m_gray(IntType(value * (1 << Shift)))
		{
		}

		bool operator==(const GrayCropAccum32& value)
		{
			return m_gray == value.m_gray;
		}

		bool operator==(int value)
		{
			return m_gray == (IntType(value) << Shift);
		}

		bool operator==(double value)
		{
			return m_gray == IntType(value * (1 << Shift));
		}

		bool operator!=(const GrayCropAccum32& value)
		{
			return m_gray != value.m_gray;
		}

		bool operator!=(int value)
		{
			return m_gray != (IntType(value) << Shift);
		}

		bool operator!=(double value)
		{
			return m_gray != IntType(value * (1 << Shift));
		}

		GrayCropAccum32& operator=(const GrayCropAccum32& value)
		{
			m_gray = value.m_gray;

			return *this;
		}
/*
		GrayCropAccum32& operator=(unsigned long long value)
		{
			m_gray = IntType(value) << Shift;

			return *this;
		}

		GrayCropAccum32& operator=(unsigned int value)
		{
			m_gray = IntType(value) << Shift;

			return *this;
		}

		GrayCropAccum32& operator=(double value)
		{
			m_gray = IntType(value * (1 << Shift));

			return *this;
		}
*/
		GrayCropAccum32& operator+=(const GrayCropAccum32& value)
		{
			m_gray += value.m_gray;

			return *this;
		}

		GrayCropAccum32& operator+=(unsigned int value)
		{
			m_gray += IntType(value) << Shift;

			return *this;
		}

		GrayCropAccum32& operator+=(double value)
		{
			m_gray += IntType(value * (1 << Shift));

			return *this;
		}

		GrayCropAccum32& operator-=(const GrayCropAccum32& value)
		{
			m_gray -= value.m_gray;

			return *this;
		}

		GrayCropAccum32& operator-=(unsigned int value)
		{
			m_gray -= IntType(value) << Shift;

			return *this;
		}

		GrayCropAccum32& operator-=(double value)
		{
			m_gray -= IntType(value * (1 << Shift));

			return *this;
		}

		GrayCropAccum32& operator*=(const GrayCropAccum32& value)
		{
			m_gray = IntType((quint64(m_gray) * quint64(value.m_gray)) >> Shift);

			return *this;
		}

		GrayCropAccum32& operator*=(unsigned int value)
		{
			m_gray *= IntType(value);

			return *this;
		}

		GrayCropAccum32& operator*=(double value)
		{
			m_gray = IntType(m_gray * value);

			return *this;
		}

		GrayCropAccum32 operator/(int value)
		{
			GrayCropAccum32 retVal;

			retVal.m_gray = m_gray / value;

			return retVal;
		}

		GrayCropAccum32 operator/(double value)
		{
			GrayCropAccum32 retVal;

			retVal.m_gray = IntType(m_gray / value);

			return retVal;
		}

		operator quint8()
		{

			if (DoCropMin && (m_gray < 0)){
				return 0;
			}
			if (DoCropMax && (m_gray > (0xff << Shift))){
				return 0xff;
			}

			return quint8(m_gray >> Shift);
		}

		operator quint16()
		{
			if (DoCropMin && (m_gray < 0)){
				return 0;
			}
			if (DoCropMin && (m_gray > (0xffff << Shift))){
				return 0xffff;
			}

			return quint16(m_gray >> Shift);
		}

		operator quint32()
		{
			if (DoCropMin && (m_gray < 0)){
				return 0;
			}

			return quint32(m_gray >> Shift);
		}

		operator Rgba()
		{
			quint8 grayValue;

			if (DoCropMin && (m_gray < 0)){
				grayValue = 0;
			}
			else if (DoCropMin && (m_gray > (0xff << Shift))){
				grayValue = 0xff;
			}
			else{
				grayValue = quint8(m_gray >> Shift);
			}

			return Rgba(grayValue, grayValue, grayValue);
		}

	protected:
		IntType m_gray;
	};

	template <typename IntType, int Shift, bool DoCropMin = true, bool DoCropMax = true>
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
		:	m_r(IntType(value.m_r) << Shift), m_g(IntType(value.m_g) << Shift), m_b(IntType(value.m_b) << Shift)
		{
		}

		RgbCropAccum32(int value)
		:	m_r(IntType(value) << Shift), m_g(IntType(value) << Shift), m_b(IntType(value) << Shift)
		{
		}

		RgbCropAccum32(quint8 value)
		:	m_r(IntType(value) << Shift), m_g(IntType(value) << Shift), m_b(IntType(value) << Shift)
		{
		}

		RgbCropAccum32(quint16 value)
		:	m_r(IntType(value) << Shift), m_g(IntType(value) << Shift), m_b(IntType(value) << Shift)
		{
		}

		RgbCropAccum32(quint32 value)
		:	m_r(IntType(value) << Shift), m_g(IntType(value) << Shift), m_b(IntType(value) << Shift)
		{
		}

		RgbCropAccum32(quint64 value)
		:	m_r(IntType(value) << Shift), m_g(IntType(value) << Shift), m_b(IntType(value) << Shift)
		{
		}

		RgbCropAccum32(double value)
		:	m_r(IntType(value * (1 << Shift))), m_g(IntType(value * (1 << Shift))), m_b(IntType(value * (1 << Shift)))
		{
		}

		bool operator==(const RgbCropAccum32& value)
		{
			return (m_r == value.m_r) && (m_g == value.m_g) && (m_b == value.m_b);
		}

		bool operator==(int value)
		{
			IntType gray = (IntType(value) << Shift);

			return (m_r == gray) && (m_g == gray) && (m_b == gray);
		}

		bool operator==(double value)
		{
			IntType gray = IntType(value * (1 << Shift));

			return (m_r == gray) && (m_g == gray) && (m_b == gray);
		}

		bool operator!=(const RgbCropAccum32& value)
		{
			return (m_r != value.m_r) || (m_g != value.m_g) || (m_b != value.m_b);
		}

		bool operator!=(int value)
		{
			IntType gray = (IntType(value) << Shift);

			return (m_r != gray) || (m_g != gray) || (m_b != gray);
		}

		bool operator!=(double value)
		{
			IntType gray = IntType(value * (1 << Shift));

			return (m_r != gray) || (m_g != gray) || (m_b != gray);
		}

		RgbCropAccum32& operator=(const RgbCropAccum32& value)
		{
			m_r = value.m_r;
			m_g = value.m_g;
			m_b = value.m_b;

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
			m_r += IntType(value.m_r) << Shift;
			m_g += IntType(value.m_g) << Shift;
			m_b += IntType(value.m_b) << Shift;

			return *this;
		}

		RgbCropAccum32& operator+=(unsigned int value)
		{
			IntType gray = IntType(value) << Shift;
			m_r += gray;
			m_g += gray;
			m_b += gray;

			return *this;
		}

		RgbCropAccum32& operator+=(double value)
		{
			IntType gray = IntType(value * (1 << Shift));
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
			m_r -= IntType(value.m_r) << Shift;
			m_g -= IntType(value.m_g) << Shift;
			m_b -= IntType(value.m_b) << Shift;

			return *this;
		}

		RgbCropAccum32& operator-=(quint32 value)
		{
			IntType gray = IntType(value) << Shift;
			m_r -= gray;
			m_g -= gray;
			m_b -= gray;

			return *this;
		}

		RgbCropAccum32& operator-=(double value)
		{
			IntType gray = IntType(value * (1 << Shift));
			m_r -= gray;
			m_g -= gray;
			m_b -= gray;

			return *this;
		}

		RgbCropAccum32& operator*=(const RgbCropAccum32& value)
		{
			m_r = IntType(quint64(m_r) * quint64(value.m_r) >> Shift);;
			m_g = IntType(quint64(m_g) * quint64(value.m_g) >> Shift);;
			m_b = IntType(quint64(m_b) * quint64(value.m_b) >> Shift);;

			return *this;
		}

		RgbCropAccum32& operator*=(int value)
		{
			m_r *= IntType(value);
			m_g *= IntType(value);
			m_b *= IntType(value);

			return *this;
		}

		RgbCropAccum32& operator*=(double value)
		{
			m_r = IntType(m_r * value);
			m_g = IntType(m_g * value);
			m_b = IntType(m_b * value);

			return *this;
		}

		RgbCropAccum32 operator/(int value)
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

			retVal.m_r = IntType(m_r / value);
			retVal.m_g = IntType(m_g / value);
			retVal.m_b = IntType(m_b / value);

			return retVal;
		}

		operator quint8()
		{
			quint64 grayValue = (quint64(m_r) + quint64(m_g) + quint64(m_b)) / 3;

			if (DoCropMin && (grayValue < 0)){
				return 0;
			}
			if (DoCropMin && (grayValue > (0xff << Shift))){
				return 0xff;
			}

			return quint8(grayValue >> Shift);
		}

		operator quint16()
		{
			quint64 grayValue = (quint64(m_r) + quint64(m_g) + quint64(m_b)) / 3;

			if (DoCropMin && (grayValue < 0)){
				return 0;
			}
			if (DoCropMin && (grayValue > (0xffff << Shift))){
				return 0xffff;
			}

			return quint16(grayValue >> Shift);
		}

		operator quint32()
		{
			quint64 grayValue = (quint64(m_r) + quint64(m_g) + quint64(m_b)) / 3;

			if (DoCropMin && (grayValue < 0)){
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
			IntType red = m_r;
			if (DoCropMin && (red < 0)){
				red = 0;
			}
			else if (DoCropMin && (red > (0xff << Shift))){
				red = 0xff;
			}

			IntType green = m_g;
			if (DoCropMin && (green < 0)){
				green = 0;
			}
			else if (DoCropMin && (green > (0xff << Shift))){
				green = 0xff;
			}

			IntType blue = m_b;
			if (DoCropMin && (blue < 0)){
				blue = 0;
			}
			else if (DoCropMin && (blue > (0xff << Shift))){
				blue = 0xff;
			}

			return Rgba(quint8(red >> Shift), quint8(green >> Shift), quint8(blue >> Shift));
		}

	protected:
		IntType m_r;
		IntType m_g;
		IntType m_b;
	};

	template <typename IntType, int Shift, bool DoCropMin = true, bool DoCropMax = true>
	class RgbaCropAccum32: public RgbCropAccum32<IntType, Shift, DoCropMin, DoCropMax>
	{
	public:
		typedef RgbCropAccum32<IntType, Shift, DoCropMin, DoCropMax> BaseClass;

		RgbaCropAccum32()
		{
		}

		RgbaCropAccum32(const RgbaCropAccum32& value)
		:	BaseClass(value),
			m_a(value.m_a)
		{
		}

		RgbaCropAccum32(const Rgba& value)
		:	BaseClass(value),
			m_a(IntType(255) << Shift)
		{
		}

		RgbaCropAccum32(int value)
		:	BaseClass(value),
			m_a(IntType(255) << Shift)
		{
		}

		RgbaCropAccum32(quint8 value)
		:	BaseClass(value),
			m_a(IntType(255) << Shift)
		{
		}

		RgbaCropAccum32(quint16 value)
		:	BaseClass(value),
			m_a(IntType(255) << Shift)
		{
		}

		RgbaCropAccum32(quint32 value)
		:	BaseClass(value),
			m_a(IntType(255) << Shift)
		{
		}

		RgbaCropAccum32(quint64 value)
		:	BaseClass(value),
			m_a(IntType(255) << Shift)
		{
		}

		RgbaCropAccum32(double value)
		:	BaseClass(value),
			m_a(IntType(255) << Shift)
		{
		}

		bool operator==(const RgbaCropAccum32& value)
		{
			return (BaseClass::m_r == value.m_r) && (BaseClass::m_g == value.m_g) && (BaseClass::m_b == value.m_b) && (m_a == value.m_a);
		}

		bool operator==(int value)
		{
			IntType gray = (IntType(value) << Shift);

			return (BaseClass::m_r == gray) && (BaseClass::m_g == gray) && (BaseClass::m_b == gray) && (m_a == (IntType(255) << Shift));
		}

		bool operator==(double value)
		{
			IntType gray = IntType(value * (1 << Shift));

			return (BaseClass::m_r == gray) && (BaseClass::m_g == gray) && (BaseClass::m_b == gray) && (m_a == (IntType(255) << Shift));
		}

		bool operator!=(const RgbaCropAccum32& value)
		{
			return (BaseClass::m_r != value.m_r) || (BaseClass::m_g != value.m_g) || (BaseClass::m_b != value.m_b) || (m_a != value.m_a);
		}

		bool operator!=(int value)
		{
			IntType gray = (IntType(value) << Shift);

			return (BaseClass::m_r != gray) || (BaseClass::m_g != gray) || (BaseClass::m_b != gray) || (m_a != (IntType(255) << Shift));
		}

		bool operator!=(double value)
		{
			IntType gray = IntType(value * (1 << Shift));

			return (BaseClass::m_r != gray) || (BaseClass::m_g != gray) || (BaseClass::m_b != gray) || (m_a != (IntType(255) << Shift));
		}

		RgbaCropAccum32& operator=(const RgbaCropAccum32& value)
		{
			BaseClass::m_r = value.m_r;
			BaseClass::m_g = value.m_g;
			BaseClass::m_b = value.m_b;
			
			m_a = value.m_a;

			return *this;
		}

		RgbaCropAccum32& operator+=(const RgbaCropAccum32& value)
		{
			IntType a = (m_a + value.m_a) / 2;
			if (a != 0){
				BaseClass::m_r = (BaseClass::m_r * m_a + value.m_r * value.m_a) / a;
				BaseClass::m_g = (BaseClass::m_g * m_a + value.m_g * value.m_a) / a;
				BaseClass::m_b = (BaseClass::m_b * m_a + value.m_b * value.m_a) / a;
			}
			m_a = a;

			return *this;
		}

		RgbaCropAccum32& operator+=(const Rgba& value)
		{
			IntType a = (m_a + (IntType(value.m_a) << Shift)) / 2;
			if (a != 0){
				BaseClass::m_r = (BaseClass::m_r * (m_a >> Shift) + (IntType(value.m_r) * IntType(value.m_a) << Shift)) / a;
				BaseClass::m_g = (BaseClass::m_g * (m_a >> Shift) + (IntType(value.m_g) * IntType(value.m_a) << Shift)) / a;
				BaseClass::m_b = (BaseClass::m_b * (m_a >> Shift) + (IntType(value.m_b) * IntType(value.m_a) << Shift)) / a;
			}
			m_a = a;				  
									  
			return *this;
		}

		RgbaCropAccum32& operator+=(unsigned int value)
		{
			IntType gray_corr = (IntType(value) * IntType(255) << Shift);
			IntType a = (m_a + (IntType(255) << Shift)) / 2;
			if (a != 0){
				BaseClass::m_r = (BaseClass::m_r * (m_a >> Shift) + gray_corr) / a;
				BaseClass::m_g = (BaseClass::m_g * (m_a >> Shift) + gray_corr) / a;
				BaseClass::m_b = (BaseClass::m_b * (m_a >> Shift) + gray_corr) / a;
			}
			m_a = a;				  

			return *this;
		}

		RgbaCropAccum32& operator+=(double value)
		{
			IntType gray_corr = IntType(value * (255 << Shift));
			IntType a = (m_a + (IntType(255) << Shift)) / 2;
			if (a != 0){
				BaseClass::m_r = (BaseClass::m_r * (m_a >> Shift) + gray_corr) / a;
				BaseClass::m_g = (BaseClass::m_g * (m_a >> Shift) + gray_corr) / a;
				BaseClass::m_b = (BaseClass::m_b * (m_a >> Shift) + gray_corr) / a;
			}
			m_a = a;

			return *this;
		}

		RgbaCropAccum32& operator*=(const RgbaCropAccum32& value)
		{
			BaseClass::operator*=(value);

			return *this;
		}

		RgbaCropAccum32& operator*=(int value)
		{
			BaseClass::operator*=(value);

			return *this;
		}

		RgbaCropAccum32& operator*=(double value)
		{
			BaseClass::operator*=(value);

			return *this;
		}

		RgbaCropAccum32 operator/(int value)
		{
			BaseClass::operator/=(value);

			return *this;
		}

		RgbaCropAccum32 operator/(double value)
		{
			BaseClass::operator/=(value);

			return *this;
		}

		operator Rgba()
		{
			IntType red = BaseClass::m_r;
			if (DoCropMin && (red < 0)){
				red = 0;
			}
			else if (DoCropMin && (red > (0xff << Shift))){
				red = 0xff;
			}

			IntType green = BaseClass::m_g;
			if (DoCropMin && (green < 0)){
				green = 0;
			}
			else if (DoCropMin && (green > (0xff << Shift))){
				green = 0xff;
			}

			IntType blue = BaseClass::m_b;
			if (DoCropMin && (blue < 0)){
				blue = 0;
			}
			else if (DoCropMin && (blue > (0xff << Shift))){
				blue = 0xff;
			}

			return Rgba(quint8(red >> Shift), quint8(green >> Shift), quint8(blue >> Shift), quint8(m_a >> Shift));
		}

	protected:
		IntType m_a;
	};
};


#pragma pack(pop)


} // namespace iipr


#endif // !iipr_CPixelManip_included


