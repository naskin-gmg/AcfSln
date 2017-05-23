#include <iipr/CHoughSpace2d.h>


// STL includes
#include <cmath>

// ACF includes
#include <istd/CChangeNotifier.h>


namespace iipr
{


// template methods for type specific bitmap operations, TODO: implement wrapping for Y axis

template <	typename PixelType,
			typename WorkingType>
void DoSmoothHoughSpace(
			int iterationsX, int iterationsY,
			bool isWrappedX, bool /*isWrappedY*/,
			CHoughSpace2d& space)
{
	istd::CIndex2d spaceSize = space.CGeneralBitmap::GetImageSize();

	if (spaceSize.GetX() >= 3){
		for (int i = 0; i < iterationsX; ++i){
			for (int y = 0; y < spaceSize.GetY(); ++y){
				PixelType* spaceLinePtr = (PixelType*)space.CGeneralBitmap::GetLinePtr(y);

				int nextX;
				PixelType prevValue;
				PixelType value;
				PixelType storedValue = 0;
				if (isWrappedX){
					nextX = 1;
					prevValue = spaceLinePtr[spaceSize.GetX() - 1];
					value = spaceLinePtr[0];
					storedValue = value;
				}
				else{
					nextX = 2;
					prevValue = spaceLinePtr[0];
					value = spaceLinePtr[1];
				}

				for (; nextX < spaceSize.GetX(); ++nextX){
					PixelType nextValue = spaceLinePtr[nextX];

					spaceLinePtr[nextX - 1] = PixelType((WorkingType(value) * 2 + WorkingType(prevValue) + WorkingType(nextValue)) / 4);

					prevValue = value;
					value = nextValue;
				}

				if (isWrappedX){
					spaceLinePtr[nextX - 1] = PixelType((WorkingType(value) * 2 + WorkingType(prevValue) + WorkingType(storedValue)) / 4);
				}
			}
		}
	}

	if (spaceSize.GetY() >= 3){
		for (int i = 0; i < iterationsY; ++i){
			QVector<PixelType> prevLine(spaceSize.GetX(), 0);
		
			PixelType* spaceLinePtr = (PixelType*)space.CGeneralBitmap::GetLinePtr(0);

			for (int y = 0; y < spaceSize.GetY() - 1; ++y){
				PixelType* nextLinePtr = (PixelType*)space.CGeneralBitmap::GetLinePtr(y + 1);

				for (int x = 0; x < spaceSize.GetX(); ++x){
					PixelType value = spaceLinePtr[x];
					PixelType nextValue = nextLinePtr[x];

					spaceLinePtr[x] = PixelType((WorkingType(value) * 2 + WorkingType(prevLine[x]) + WorkingType(nextValue)) / 4);

					prevLine[x] = value;
				}

				spaceLinePtr = nextLinePtr;
			}

			for (int x = 0; x < spaceSize.GetX(); ++x){
				int value = spaceLinePtr[x];

				spaceLinePtr[x] = (WorkingType(value) * 2 + WorkingType(prevLine[x])) / 4;
			}
		}
	}
}


template <	typename PixelType,
			typename WorkingType>
void DoAnalyseSpace(
			int maxPoints,
			PixelType minWeight,
			double minMaxRatio,
			double minDistance,
			double minLocalDynamic,
			bool isWrappedX, bool /*isWrappedY*/,
			const CHoughSpace2d& space,
			QMultiMap<double, i2d::CVector2d>& result)
{
	istd::CIndex2d spaceSize = space.CGeneralBitmap::GetImageSize();

	if (spaceSize.GetX() < 3){
		return;
	}

	PixelType maxValue = 0;
	PixelType minValue = minWeight;

	for (int y = 0; y < spaceSize.GetY(); ++y){
		const PixelType* prevSpaceLinePtr = (const PixelType*)space.CGeneralBitmap::GetLinePtr((y + spaceSize.GetY() - 1) % spaceSize.GetY());
		const PixelType* spaceLinePtr = (const PixelType*)space.CGeneralBitmap::GetLinePtr(y);
		const PixelType* nextSpaceLinePtr = (const PixelType*)space.CGeneralBitmap::GetLinePtr((y + 1) % spaceSize.GetY());

		int prevX;
		int x;
		int nextX;
		if (isWrappedX){
			prevX = spaceSize.GetX() - 2;
			x = spaceSize.GetX() - 1;
			nextX = 0;
		}
		else{
			prevX = 0;
			x = 1;
			nextX = 2;
		}

		for (; nextX < spaceSize.GetX(); ++nextX){
			PixelType value = spaceLinePtr[x];
			if (		(value >= minValue) &&
						(value >= prevSpaceLinePtr[prevX]) &&
						(value > prevSpaceLinePtr[x]) &&
						(value > prevSpaceLinePtr[nextX]) &&
						(value >= spaceLinePtr[prevX]) &&
						(value > spaceLinePtr[nextX]) &&
						(value >= nextSpaceLinePtr[prevX]) &&
						(value >= nextSpaceLinePtr[x]) &&
						(value > nextSpaceLinePtr[nextX])){
				WorkingType diffLeft = WorkingType(value) - WorkingType(spaceLinePtr[prevX]);
				WorkingType diffRight = WorkingType(value) - WorkingType(spaceLinePtr[nextX]);
				WorkingType diffTop = WorkingType(value) - WorkingType(prevSpaceLinePtr[x]);
				WorkingType diffBottom = WorkingType(value) - WorkingType(nextSpaceLinePtr[x]);

				if ((diffLeft + diffRight + diffTop + diffBottom) > value * minLocalDynamic){
					double correctionX = double(diffLeft) / (diffLeft + diffRight);
					double correctionY = double(diffTop) / (diffTop + diffBottom);

					i2d::CVector2d resultPos(x + correctionX, y + correctionY);

					result.insert(value, resultPos);

					if (value > maxValue){
						maxValue = value;
						PixelType propValue = PixelType(value * minMaxRatio);
						if (minValue < propValue){
							minValue = propValue;

							// remove elements weeker than new calculated minValue
							while (!result.isEmpty() && result.firstKey() < minValue){
								QMultiMap<double, i2d::CVector2d>::Iterator lastIter = result.begin();

								result.erase(lastIter);
							}
						}
					}
				}
			}

			prevX = x;
			x = nextX;
		}
	}

	// remove elements beeing to close to each other
	for (		QMultiMap<double, i2d::CVector2d>::Iterator point1Iter = result.begin();
				point1Iter != result.end();){
		const i2d::CVector2d& point1 = point1Iter.value();

		bool isToClose = false;

		for (		QMultiMap<double, i2d::CVector2d>::Iterator point2Iter = point1Iter + 1;
					point2Iter != result.end();
					++point2Iter){
			const i2d::CVector2d& point2 = point2Iter.value();

			double dist2 = space.GetDistance2(point1, point2);
			if (dist2 <= minDistance * minDistance){
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
	if (maxPoints >= 0){
		while (result.size() > maxPoints){
			QMultiMap<double, i2d::CVector2d>::Iterator lastIter = result.begin();

			result.erase(lastIter);	// remove the weekest element
		}
	}
}


template <	typename PixelType,
			typename WorkingType>
bool DoExtractToBitmap(
			const CHoughSpace2d& space,
			iimg::IBitmap& bitmap)
{
	istd::CIndex2d spaceSize = space.CGeneralBitmap::GetImageSize();

	if (bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, spaceSize)){
		PixelType maxValue = 0;
		for (int y = 0; y < spaceSize.GetY(); ++y){
			const PixelType* spaceLinePtr = (const PixelType*)space.CGeneralBitmap::GetLinePtr(y);

			for (int x = 0; x < spaceSize.GetX(); ++x){
				PixelType value = spaceLinePtr[x];

				if (value > maxValue){
					maxValue = value;
				}
			}
		}

		if (maxValue > 0){
			for (int y = 0; y < spaceSize.GetY(); ++y){
				const PixelType* spaceLinePtr = (const PixelType*)space.CGeneralBitmap::GetLinePtr(y);
				quint8* resultLinePtr = (quint8*)bitmap.GetLinePtr(y);

				for (int x = 0; x < spaceSize.GetX(); ++x){
					PixelType value = spaceLinePtr[x];

					resultLinePtr[x] = quint8(WorkingType(value) * 255 / maxValue);
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


// public methods

CHoughSpace2d::CHoughSpace2d()
{
}


CHoughSpace2d::CHoughSpace2d(const istd::CIndex2d& size, bool isWrappedX, bool isWrappedY)
{
	CHoughSpace2d::CreateHoughSpace(size, isWrappedX, isWrappedY);
}


bool CHoughSpace2d::CreateHoughSpace(
			const istd::CIndex2d& size,
			bool isWrappedX,
			bool isWrappedY,
			bool isFloatSpace)
{
	istd::CChangeNotifier notifier(this);

	m_isWrappedX = isWrappedX;
	m_isWrappedY = isWrappedY;

	if (BaseClass::CreateBitmap(isFloatSpace? iimg::IBitmap::PF_FLOAT32: iimg::IBitmap::PF_GRAY32, size)){
		BaseClass::ClearImage();

		return true;
	}

	return false;
}


bool CHoughSpace2d::IsWrappedX() const
{
	return m_isWrappedX;
}


void CHoughSpace2d::SetWrappedX(bool state)
{
	if (state != m_isWrappedX){
		istd::CChangeNotifier notifier(this);

		m_isWrappedX = state;
	}
}


bool CHoughSpace2d::IsWrappedY() const
{
	return m_isWrappedY;
}


void CHoughSpace2d::SetWrappedY(bool state)
{
	if (state != m_isWrappedY){
		istd::CChangeNotifier notifier(this);

		m_isWrappedY = state;
	}
}


void CHoughSpace2d::IncreaseValueAt(const i2d::CVector2d& position, double value)
{
	istd::CIndex2d size = BaseClass::GetImageSize();

	int posX = int(position.GetX());
	if (m_isWrappedX){	// correct the position if is wrapped
		posX = (posX + size.GetX()) % size.GetX();
	}

	int posY = int(position.GetY());
	if (m_isWrappedY){	// correct the position if is wrapped
		posY = (posY + size.GetY()) % size.GetY();
	}

	if ((posX >= 0) && (posX < size.GetX()) && (posY >= 0) && (posY < size.GetY())){
		if (GetPixelFormat() == PF_FLOAT32){
			float* linePtr = (float*)BaseClass::GetLinePtr(posY);

			linePtr[posX] += float(value);
		}
		else{
			Q_ASSERT(GetPixelFormat() == PF_GRAY32);

			quint32* linePtr = (quint32*)BaseClass::GetLinePtr(posY);

			linePtr[posX] += quint32(value);
		}
	}
}


void CHoughSpace2d::SmoothHoughSpace(int iterationsX, int iterationsY)
{
	switch (GetPixelFormat())
	{
	case PF_GRAY32:
		DoSmoothHoughSpace<quint32, quint32>(iterationsX, iterationsY, m_isWrappedX, m_isWrappedY, *this);
		break;

	case PF_FLOAT32:
		DoSmoothHoughSpace<float, float>(iterationsX, iterationsY, m_isWrappedX, m_isWrappedY, *this);
		break;

	default:
		break;
	}
}


void CHoughSpace2d::AnalyseHoughSpace(
			int maxPoints,
			int minWeight,
			double minMaxRatio,
			double minDistance,
			double minLocalDynamic,
			WeightToHoughPosMap& result)
{
	switch (GetPixelFormat())
	{
	case PF_GRAY32:
		DoAnalyseSpace<quint32, qint32>(maxPoints, minWeight, minMaxRatio, minDistance, minLocalDynamic, m_isWrappedX, m_isWrappedY, *this, result);
		break;

	case PF_FLOAT32:
		DoAnalyseSpace<float, float>(maxPoints, minWeight, minMaxRatio, minDistance, minLocalDynamic, m_isWrappedX, m_isWrappedY, *this, result);
		break;

	default:
		break;
	}
}


bool CHoughSpace2d::ExtractToBitmap(iimg::IBitmap& bitmap) const
{
	switch (GetPixelFormat())
	{
	case PF_GRAY32:
		return DoExtractToBitmap<quint32, quint64>(*this, bitmap);

	case PF_FLOAT32:
		return DoExtractToBitmap<float, float>(*this, bitmap);

	default:
		return false;
	}
}


bool CHoughSpace2d::GetSpacePosition(const i2d::CVector2d& position, i2d::CVector2d& result) const
{
	istd::CIndex2d size = BaseClass::GetImageSize();

	result = position;

	if (m_isWrappedX){	// correct the position if is wrapped
		result.SetX(fmod(result.GetX() + size.GetX(), size.GetX()));
	}

	if (m_isWrappedY){	// correct the position if is wrapped
		result.SetY(fmod(result.GetY() + size.GetY(), size.GetY()));
	}

	return ((result.GetX() >= 0) && (result.GetX() < size.GetX()) && (result.GetY() >= 0) && (result.GetY() < size.GetY()));
}


void CHoughSpace2d::CalcSpaceMin(const CHoughSpace2d& space)
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


void CHoughSpace2d::CalcSpaceMax(const CHoughSpace2d& space)
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


// reimplemented (iimg::CGeneralBitmap)

bool CHoughSpace2d::CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, int pixelBitsCount, int componentsCount)
{
	if ((pixelFormat != PF_GRAY32) && (pixelFormat != PF_FLOAT32)){
		return false;
	}

	return BaseClass::CreateBitmap(pixelFormat, size, pixelBitsCount, componentsCount);
}


bool CHoughSpace2d::CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference)
{
	if ((pixelFormat != PF_GRAY32) && (pixelFormat != PF_FLOAT32)){
		return false;
	}

	return BaseClass::CreateBitmap(pixelFormat, size, dataPtr, releaseFlag, linesDifference);
}


} // namespace iipr


