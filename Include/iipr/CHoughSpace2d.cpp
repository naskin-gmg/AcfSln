#include "iipr/CHoughSpace2d.h"



namespace iipr
{


bool CHoughSpace2d::CreateHoughSpace(const istd::CIndex2d& size)
{
	if (BaseClass::CreateBitmap(iimg::IBitmap::PF_GRAY32, size)){
		BaseClass::ClearImage();

		return true;
	}

	return false;
}


void CHoughSpace2d::SmoothHoughSpace(int iterations)
{
	istd::CIndex2d spaceSize = BaseClass::GetImageSize();

	if (spaceSize.GetX() < 2){
		return;
	}

	for (int i = 0; i < iterations; ++i){
		for (int y = 0; y < spaceSize.GetY(); ++y){
			quint32* spaceLinePtr = (quint32*)BaseClass::GetLinePtr(y);

			int prevValue = spaceLinePtr[0];
			int value = spaceLinePtr[1];

			for (int nextX = 2; nextX < spaceSize.GetX(); ++nextX){
				int nextValue = spaceLinePtr[nextX];

				spaceLinePtr[nextX - 1] = (value * 2 + prevValue + nextValue) >> 2;

				prevValue = value;
				value = nextValue;
			}
		}
	}
}


void CHoughSpace2d::AnalyseHoughSpace(int maxPoints, int minWeight, double minMaxRatio, double minDistance, WeightToHoughPosMap& result)
{
	istd::CIndex2d spaceSize = BaseClass::GetImageSize();

	quint32 maxValue = 0;
	quint32 minValue = quint32(minWeight);

	for (int y = 0; y < spaceSize.GetY(); ++y){
		const quint32* prevSpaceLinePtr = (const quint32*)BaseClass::GetLinePtr((y + spaceSize.GetY() - 1) % spaceSize.GetY());
		const quint32* spaceLinePtr = (const quint32*)BaseClass::GetLinePtr(y);
		const quint32* nextSpaceLinePtr = (const quint32*)BaseClass::GetLinePtr((y + 1) % spaceSize.GetY());

		int prevX = 0;
		int x = 0;
		for (int nextX = 0; nextX < spaceSize.GetX(); ++nextX){
			quint32 value = spaceLinePtr[x];
			if (		(value >= minValue) &&
						(value >= prevSpaceLinePtr[prevX]) &&
						(value > prevSpaceLinePtr[x]) &&
						(value > prevSpaceLinePtr[nextX]) &&
						(value >= spaceLinePtr[prevX]) &&
						(value > spaceLinePtr[nextX]) &&
						(value >= nextSpaceLinePtr[prevX]) &&
						(value >= nextSpaceLinePtr[x]) &&
						(value > nextSpaceLinePtr[nextX])){
				int diffLeft = value - spaceLinePtr[prevX];
				int diffRight = value - spaceLinePtr[nextX];
				int diffTop = value - prevSpaceLinePtr[x];
				int diffBottom = value - nextSpaceLinePtr[x];
				double correctionX = double(diffLeft) / (diffLeft + diffRight);
				double correctionY = double(diffTop) / (diffTop + diffBottom);

				i2d::CVector2d resultPos(x + correctionX, y + correctionY);

				result.insert(value, resultPos);

				if (value > maxValue){
					maxValue = value;
					quint32 propValue = quint32(value * minMaxRatio);
					if (minValue < propValue){
						minValue = propValue;

						// remove elements weeker than new calculated minValue
						while (!result.isEmpty() && result.firstKey() < minValue){
							WeightToHoughPosMap::Iterator lastIter = result.begin();

							result.erase(lastIter);
						}
					}
				}
			}

			prevX = x;
			x = nextX;
		}
	}

	// remove elements beeing to close to each other
	for (		WeightToHoughPosMap::Iterator point1Iter = result.begin();
				point1Iter != result.end();){
		const i2d::CVector2d& point1 = point1Iter.value();

		bool isToClose = false;
		WeightToHoughPosMap::Iterator foundIter = result.end();

		for (		WeightToHoughPosMap::Iterator point2Iter = point1Iter + 1;
					point2Iter != result.end();
					++point2Iter){
			const i2d::CVector2d& point2 = point2Iter.value();

			double dist = point1.GetDistance(point2);
			if (dist <= minDistance){
				isToClose = true;

				break;
			}
		}

		if (isToClose){
			point1Iter = result.erase(point1Iter);
		}
		else{
			++point1Iter;
		}
	}

	// cut this list to user defined maximal size
	while (result.size() > maxPoints){
		WeightToHoughPosMap::Iterator lastIter = result.begin();

		result.erase(lastIter);	// remove the weekest element
	}
}


bool CHoughSpace2d::ExtractToBitmap(iimg::IBitmap& bitmap)
{
	istd::CIndex2d spaceSize = BaseClass::GetImageSize();

	if (bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, spaceSize)){
		quint32 maxValue = 0;
		for (int y = 0; y < spaceSize.GetY(); ++y){
			const quint32* spaceLinePtr = (const quint32*)BaseClass::GetLinePtr(y);

			for (int x = 0; x < spaceSize.GetX(); ++x){
				quint32 value = spaceLinePtr[x];

				if (value > maxValue){
					maxValue = value;
				}
			}
		}

		if (maxValue > 0){
			for (int y = 0; y < spaceSize.GetY(); ++y){
				const quint32* spaceLinePtr = (const quint32*)BaseClass::GetLinePtr(y);
				quint8* resultLinePtr = (quint8*)bitmap.GetLinePtr(y);

				for (int x = 0; x < spaceSize.GetX(); ++x){
					quint64 value = spaceLinePtr[x];

					resultLinePtr[x] = value * 255 / maxValue;
				}
			}
		}
		else{
			bitmap.ClearImage();
		}

		return true;
	}

	return false;
}


void CHoughSpace2d::CalcMin(const CHoughSpace2d& space)
{
	istd::CIndex2d size = BaseClass::GetImageSize();
	istd::CIndex2d spaceSize = space.GetImageSize();

	istd::CIndex2d commonSize(qMin(size.GetX(), spaceSize.GetX()), qMin(size.GetY(), spaceSize.GetY()));
	for (int y = 0; y < commonSize.GetY(); ++y){
		quint32* linePtr = (quint32*)BaseClass::GetLinePtr(y);
		const quint32* spaceLinePtr = (const quint32*)space.GetLinePtr(y);
		for (int x = 0; x < commonSize.GetX(); ++x){
			linePtr[x] = qMin(linePtr[x], spaceLinePtr[x]);
		}
	}
}


void CHoughSpace2d::CalcMax(const CHoughSpace2d& space)
{
	istd::CIndex2d size = BaseClass::GetImageSize();
	istd::CIndex2d spaceSize = space.GetImageSize();

	istd::CIndex2d commonSize(qMin(size.GetX(), spaceSize.GetX()), qMin(size.GetY(), spaceSize.GetY()));
	for (int y = 0; y < commonSize.GetY(); ++y){
		quint32* linePtr = (quint32*)BaseClass::GetLinePtr(y);
		const quint32* spaceLinePtr = (const quint32*)space.GetLinePtr(y);
		for (int x = 0; x < commonSize.GetX(); ++x){
			linePtr[x] = qMax(linePtr[x], spaceLinePtr[x]);
		}
	}
}


} // namespace iipr


