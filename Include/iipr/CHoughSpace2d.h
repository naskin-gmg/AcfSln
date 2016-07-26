#ifndef iipr_CHoughSpace2d_included
#define iipr_CHoughSpace2d_included


// Qt includes
#include <QtCore/QMultiMap>

// ACF includes
#include "i2d/CVector2d.h"
#include "iimg/CGeneralBitmap.h"


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

	/**
		Create Hough space with specified size.
	*/
	bool CreateHoughSpace(const istd::CIndex2d& size);

	/**
		Smooth this space with specified stronness.
	*/
	void SmoothHoughSpace(int iterations);
	/**
		Analyse this Hough space to find set of local maximums.
	*/
	void AnalyseHoughSpace(int maxPoints, int minWeight, WeightToHoughPosMap& result);
};


} // namespace iipr


#endif // !iipr_CHoughSpace2d_included


