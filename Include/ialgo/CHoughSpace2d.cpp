#include <ialgo/CHoughSpace2d.h>


// STL includes
#include <cmath>

// ACF includes
#include <istd/CChangeNotifier.h>


namespace ialgo
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

				int nextX = 1;
				PixelType prevValue;
				PixelType value;
				PixelType storedValue;
				if (isWrappedX){
					prevValue = spaceLinePtr[spaceSize.GetX() - 1];
					value = spaceLinePtr[0];
					storedValue = value;
				}
				else{
					prevValue = spaceLinePtr[0];
					value = spaceLinePtr[1];
					storedValue = 0;
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
				else{
					spaceLinePtr[nextX - 1] = PixelType((WorkingType(value) * 2 + WorkingType(prevValue)) / 4);
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
			double minWeight,
			bool isWrappedX, bool /*isWrappedY*/,
			const CHoughSpace2d& space,
			CHoughSpace2d::ResultsConsumer& resultProcessor)
{
	resultProcessor.OnProcessingBegin(space, minWeight);

	istd::CIndex2d spaceSize = space.CGeneralBitmap::GetImageSize();

	if (spaceSize.GetX() < 3){
		return;
	}

	PixelType minValue = minWeight;

	istd::CIndex2d position;
	for (position.SetY(0); position.GetY() < spaceSize.GetY(); ++position[1]){
		const PixelType* prevSpaceLinePtr = (const PixelType*)space.CGeneralBitmap::GetLinePtr((position.GetY() + spaceSize.GetY() - 1) % spaceSize.GetY());
		const PixelType* spaceLinePtr = (const PixelType*)space.CGeneralBitmap::GetLinePtr(position.GetY());
		const PixelType* nextSpaceLinePtr = (const PixelType*)space.CGeneralBitmap::GetLinePtr((position.GetY() + 1) % spaceSize.GetY());

		int prevX;
		int nextX;
		if (isWrappedX){
			prevX = spaceSize.GetX() - 2;
			position.SetX(spaceSize.GetX() - 1);
			nextX = 0;
		}
		else{
			prevX = 0;
			position.SetX(1);
			nextX = 2;
		}

		for (; nextX < spaceSize.GetX(); ++nextX){
			PixelType value = spaceLinePtr[position.GetX()];
			if (		(value >= minValue) &&
						(value > prevSpaceLinePtr[position.GetX()]) &&
						(value >= spaceLinePtr[prevX]) &&
						(value > spaceLinePtr[nextX]) &&
						(value >= nextSpaceLinePtr[position.GetX()])){
				double neighbours[] = {
							double(spaceLinePtr[prevX]),
							double(spaceLinePtr[nextX]),
							double(prevSpaceLinePtr[position.GetX()]),
							double(nextSpaceLinePtr[position.GetX()])};

				if (resultProcessor.OnMaximumFound(space, position, double(value), neighbours, 4, minWeight)){
					resultProcessor.OnProcessingEnd(space);
					return;
				}

				minValue = minWeight;
			}

			prevX = position.GetX();
			position.SetX(nextX);
		}
	}

	resultProcessor.OnProcessingEnd(space);
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
	CHoughSpace2d::CreateHoughSpace(size, isWrappedX, isWrappedY, false);
}


bool CHoughSpace2d::CreateHoughSpace(
			const istd::CIndex2d& size,
			bool isWrappedX,
			bool isWrappedY,
			bool isFloatSpace)
{
	istd::CChangeNotifier notifier(this);

	m_isWrapped[0] = isWrappedX;
	m_isWrapped[1] = isWrappedY;

	if (BaseClass::CreateBitmap(isFloatSpace? iimg::IBitmap::PF_FLOAT32: iimg::IBitmap::PF_GRAY32, size)){
		BaseClass::ClearImage();

		return true;
	}

	return false;
}


// reimplemented (ialgo::TIHoughSpace<2>)

istd::TIndex<2> CHoughSpace2d::GetSpaceSize() const
{
	return BaseClass::GetImageSize();
}


bool CHoughSpace2d::CreateHoughSpace(const istd::TIndex<2>& size, const double& initValue)
{
	istd::CChangeNotifier notifier(this);

	m_isWrapped[0] = false;
	m_isWrapped[1] = false;

	if (BaseClass::CreateBitmap(iimg::IBitmap::PF_FLOAT32, size)){

		if (initValue == 0){
			BaseClass::ClearImage();
		}
		else{
			float initValueFloat = initValue;

			for (int y = 0; y < size[1]; ++y){
				float* linePtr = (float*)BaseClass::GetLinePtr(y);

				for (int x = 0; x < size[0]; ++x){
					linePtr[x] = initValueFloat;
				}
			}
		}

		return true;
	}

	return false;
}


bool CHoughSpace2d::IsDimensionWrapped(int dimensionIndex) const
{
	Q_ASSERT(dimensionIndex >= 0);
	Q_ASSERT(dimensionIndex < 2);

	return m_isWrapped[dimensionIndex];
}


void CHoughSpace2d::SetDimensionWrapped(int dimensionIndex, bool state)
{
	Q_ASSERT(dimensionIndex >= 0);
	Q_ASSERT(dimensionIndex < 2);

	m_isWrapped[dimensionIndex] = state;
}


void CHoughSpace2d::IncreaseValueAt(const imath::TVector<2>& position, double value)
{
	istd::CIndex2d size = BaseClass::GetImageSize();

	int posX = int(position[0]);
	if (m_isWrapped[0]){	// correct the position if is wrapped
		posX = (posX + size.GetX()) % size.GetX();
	}

	int posY = int(position[1]);
	if (m_isWrapped[1]){	// correct the position if is wrapped
		posY = (posY + size.GetY()) % size.GetY();
	}

	if ((posX >= 0) && (posX < size.GetX()) && (posY >= 0) && (posY < size.GetY())){
		if (BaseClass::GetPixelFormat() == PF_FLOAT32){
			float* linePtr = (float*)BaseClass::GetLinePtr(posY);

			linePtr[posX] += float(value);
		}
		else{
			Q_ASSERT(BaseClass::GetPixelFormat() == PF_GRAY32);

			quint32* linePtr = (quint32*)BaseClass::GetLinePtr(posY);

			linePtr[posX] += quint32(value);
		}
	}
}


void CHoughSpace2d::SmoothHoughSpace(const istd::TIndex<2>& iterations)
{
	switch (GetPixelFormat()){
	case PF_GRAY32:
		DoSmoothHoughSpace<quint32, quint32>(iterations[0], iterations[1], m_isWrapped[0], m_isWrapped[1], *this);
		break;

	case PF_FLOAT32:
		DoSmoothHoughSpace<float, float>(iterations[0], iterations[1], m_isWrapped[0], m_isWrapped[1], *this);
		break;

	default:
		break;
	}
}


bool CHoughSpace2d::AnalyseHoughSpace(
			const double& minValue,
			ResultsConsumer& resultProcessor) const
{
	QList<int> supportedNeighboursCount = resultProcessor.GetSupportedNeghboursCount();
	if (supportedNeighboursCount.contains(4) || supportedNeighboursCount.isEmpty()){
		switch (GetPixelFormat())
		{
		case PF_GRAY32:
			DoAnalyseSpace<quint32, qint32>(minValue, m_isWrapped[0], m_isWrapped[1], *this, resultProcessor);
			return true;

		case PF_FLOAT32:
			DoAnalyseSpace<float, float>(minValue, m_isWrapped[0], m_isWrapped[1], *this, resultProcessor);
			return true;

		default:
			break;
		}
	}

	return false;
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


bool CHoughSpace2d::GetSpacePosition(const imath::TVector<2>& position, imath::TVector<2>& result) const
{
	istd::CIndex2d size = BaseClass::GetImageSize();

	result = position;

	if (m_isWrapped[0]){	// correct the position if is wrapped
		result[0] = fmod(result[0] + size.GetX(), size.GetX());
	}

	if (m_isWrapped[1]){	// correct the position if is wrapped
		result[1] = (fmod(result[1] + size.GetY(), size.GetY()));
	}

	return ((result[0] >= 0) && (result[0] < size.GetX()) && (result[1] >= 0) && (result[1] < size.GetY()));
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


} // namespace ialgo


