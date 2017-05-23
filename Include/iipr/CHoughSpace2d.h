#ifndef iipr_CHoughSpace2d_included
#define iipr_CHoughSpace2d_included


// Qt includes
#include <QtCore/QMultiMap>

// ACF includes
#include <i2d/CVector2d.h>
#include <iimg/CGeneralBitmap.h>


namespace iipr
{


/**
	Hough space used for 2d Hough transformations.
*/
class CHoughSpace2d: public iimg::CGeneralBitmap
{
public:
	typedef iimg::CGeneralBitmap BaseClass;

	typedef QMultiMap<double, i2d::CVector2d> WeightToHoughPosMap;

	CHoughSpace2d();
	CHoughSpace2d(const istd::CIndex2d& size, bool isWrappedX = false, bool isWrappedY = false);

	/**
		Create Hough space with specified size.
	*/
	bool CreateHoughSpace(
				const istd::CIndex2d& size,
				bool isWrappedX = false,
				bool isWrappedY = false,
				bool isFloatSpace = false);

	/**
		Check if this space is wrapped horizontaly, it means the the left pixel is neighbour of the right one.
	*/
	bool IsWrappedX() const;
	/**
		Set if this space to be wrapped horizontaly or not.
		Space is horizonally wrapped if the left pixel is neighbour of the right one.
	*/
	void SetWrappedX(bool state);
	/**
		Check if this space is wrapped vertically, it means the top pixel is neighbour of the bottom one.
	*/
	bool IsWrappedY() const;
	/**
		Set if this space to be wrapped verticaly or not.
		Space is horizonally wrapped if the top pixel is neighbour of the bottom one.
	*/
	void SetWrappedY(bool state);

	/**
		Increase the value at specified position.
	*/
	void IncreaseValueAt(const i2d::CVector2d& position, double value);

	/**
		Smooth this space with specified stronness.
	*/
	void SmoothHoughSpace(int iterationsX, int iterationsY = 0);
	/**
		Analyse this Hough space to find set of local maximums.
		\param	maxPoints		maximal number of points to find or negative value, if disabled.
		\param	minWeight		minimal weight of point.
		\param	minMaxRatio		maximal proportion between best and worse.
		\param	minDistance		minimal distance between two found points.
		\param	minLocalDynamic	describes how strong must maximal point differs from its neighbourhood
		\param	result			will be filled with list of found points.
	*/
	void AnalyseHoughSpace(
				int maxPoints,
				int minWeight,
				double minMaxRatio,
				double minDistance,
				double minLocalDynamic,
				WeightToHoughPosMap& result);

	/**
		Extract this Hough space to some gray scale bitmap.
	*/
	bool ExtractToBitmap(iimg::IBitmap& bitmap) const;

	/**
		Get position in space for some input.
		It consides the wrapping of space.
		\return	true, if this position is representable in the space.
	*/
	bool GetSpacePosition(const i2d::CVector2d& position, i2d::CVector2d& result) const;

	/**
		Get distance between two hough space positions considering the space wrapping.
	*/
	double GetDistance(const i2d::CVector2d& position1, const i2d::CVector2d& position2) const;

	/**
		Get square of distance between two hough space positions considering the space wrapping.
	*/
	double GetDistance2(const i2d::CVector2d& position1, const i2d::CVector2d& position2) const;

	/**
		Calculate minimum of all pixels in this space.
	*/
	void CalcSpaceMin(const CHoughSpace2d& space);
	/**
		Calculate maximum of all pixels in this space.
	*/
	void CalcSpaceMax(const CHoughSpace2d& space);

	// reimplemented (iimg::CGeneralBitmap)
	virtual bool CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, int pixelBitsCount = 0, int componentsCount = 0);
	virtual bool CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference = 0);

private:
	bool m_isWrappedX;
	bool m_isWrappedY;
};


// inline methods

inline double CHoughSpace2d::GetDistance(const i2d::CVector2d& position1, const i2d::CVector2d& position2) const
{
	return qSqrt(GetDistance2(position1, position2));
}


inline double CHoughSpace2d::GetDistance2(const i2d::CVector2d& position1, const i2d::CVector2d& position2) const
{
	istd::CIndex2d spaceSize = BaseClass::GetImageSize();

	i2d::CVector2d diff = position2 - position1;
	if (m_isWrappedX){
		double offset = spaceSize.GetX() * 0.5;
		diff.SetX(std::fmod(diff.GetX() + offset + spaceSize.GetX(),  spaceSize.GetX()) - offset);
	}

	if (m_isWrappedY){
		double offset = spaceSize.GetY() * 0.5;
		diff.SetY(std::fmod(diff.GetY() + offset + spaceSize.GetY(),  spaceSize.GetY()) - offset);
	}

	return diff.GetLength2();
}


} // namespace iipr


#endif // !iipr_CHoughSpace2d_included


