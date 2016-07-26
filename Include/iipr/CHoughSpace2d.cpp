#include "iipr/CHoughSpace2d.h"



namespace iipr
{


bool CHoughSpace2d::CreateHoughSpace(const istd::CIndex2d& size)
{
	return BaseClass::CreateBitmap(iimg::IBitmap::PF_GRAY32, size);
}


void CHoughSpace2d::SmoothHoughSpace(int /*iterations*/)
{
	// TODO: implement it
}


void CHoughSpace2d::AnalyseHoughSpace(int maxPoints, int minWeight, WeightToHoughPosMap& result)
{
	istd::CIndex2d spaceSize = GetImageSize();

	for (int angleIndex = 0; angleIndex < spaceSize.GetY(); ++angleIndex){
		const quint32* angleLinePtr = (const quint32*)GetLinePtr((angleIndex + spaceSize.GetY() - 1) % spaceSize.GetY());
		const quint32* prevAngleLinePtr = (const quint32*)GetLinePtr(angleIndex);
		const quint32* nextAngleLinePtr = (const quint32*)GetLinePtr((angleIndex + 1) % spaceSize.GetY());

		int prevRadiusIndex = 0;
		int radiusIndex = 0;
		for (int nextRadiusIndex = 0; nextRadiusIndex < spaceSize.GetX(); ++nextRadiusIndex){
			quint32 value = angleLinePtr[radiusIndex];
			if (		(value >= quint32(minWeight)) &&
						(value >= prevAngleLinePtr[prevRadiusIndex]) &&
						(value > prevAngleLinePtr[radiusIndex]) &&
						(value > prevAngleLinePtr[nextRadiusIndex]) &&
						(value >= angleLinePtr[prevRadiusIndex]) &&
						(value > angleLinePtr[nextRadiusIndex]) &&
						(value >= nextAngleLinePtr[prevRadiusIndex]) &&
						(value >= nextAngleLinePtr[radiusIndex]) &&
						(value > nextAngleLinePtr[nextRadiusIndex])){
				int diffLeft = value - angleLinePtr[prevRadiusIndex];
				int diffRight = value - angleLinePtr[nextRadiusIndex];
				int diffTop = value - prevAngleLinePtr[radiusIndex];
				int diffBottom = value - nextAngleLinePtr[radiusIndex];
				double correctionX = double(diffLeft) / (diffLeft + diffRight);
				double correctionY = double(diffTop) / (diffTop + diffBottom);

				i2d::CVector2d resultPos(radiusIndex + correctionX, angleIndex + correctionY);

				result.insert(value, resultPos);

				if (result.size() > maxPoints){
					WeightToHoughPosMap::Iterator lastIter = result.end()--;

					minWeight = lastIter.key();

					result.erase(lastIter);	// remove the weekest element
				}
			}

			prevRadiusIndex = radiusIndex;
			radiusIndex = nextRadiusIndex;
		}
	}
}


} // namespace iipr


