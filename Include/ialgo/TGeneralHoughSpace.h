#ifndef ialgo_TGeneralHoughSpace_included
#define ialgo_TGeneralHoughSpace_included


// Qt includes
#include <QtCore/QMultiMap>

// ACF includes
#include <i2d/CVector2d.h>
#include <istd/TArray.h>

// ACF-Solutions includes
#include <ialgo/TIHoughSpace.h>


namespace ialgo
{


/**
	General Hough space.
*/
template <int Dimensions, class Element = double>
class TGeneralHoughSpace:
			public istd::TArray<Element, Dimensions>,
			virtual public TIHoughSpace<Dimensions, Element>
{
public:
	typedef istd::TArray<Element, Dimensions> BaseClass;

	TGeneralHoughSpace();
	TGeneralHoughSpace(const istd::TIndex<Dimensions>& size);

	/**
		Apply some operation to each element.
	*/
	template <typename Operation>
	void ApplyOperation(Operation operation);

	/**
		Combine this space with some other space.
	*/
	template <typename Operation>
	void CombineWithSpace(const TGeneralHoughSpace& space, Operation operation);

	// reimplemented (ialgo::TIHoughSpace)
	virtual istd::TIndex<Dimensions> GetSpaceSize() const;
	virtual bool CreateHoughSpace(const istd::TIndex<Dimensions>& size, const Element& initValue = 0);
	virtual bool IsDimensionWrapped(int dimensionIndex) const;
	virtual void SetDimensionWrapped(int dimensionIndex, bool state);
	virtual void IncreaseValueAt(const imath::TVector<Dimensions>& position, Element value);
	virtual void SmoothHoughSpace(const istd::TIndex<Dimensions>& iterations);
	virtual bool AnalyseHoughSpace(
				const Element& minValue,
				ResultsConsumer& resultProcessor);
	virtual bool ExtractToBitmap(iimg::IBitmap& bitmap) const;
	virtual bool GetSpacePosition(const imath::TVector<Dimensions>& position, imath::TVector<Dimensions>& result) const;
	virtual double GetDistance(const imath::TVector<Dimensions>& position1, const imath::TVector<Dimensions>& position2) const;
	virtual double GetDistance2(const imath::TVector<Dimensions>& position1, const imath::TVector<Dimensions>& position2) const;

protected:
	void SmoothSingleDimension(int dimensionIndex, int iterations);

private:
	bool m_isWrapped[Dimensions];
};


// inline methods

template <int Dimensions, class Element>
inline double TGeneralHoughSpace<Dimensions, Element>::GetDistance(const imath::TVector<Dimensions>& position1, const imath::TVector<Dimensions>& position2) const
{
	return qSqrt(GetDistance2(position1, position2));
}


template <int Dimensions, class Element>
inline double TGeneralHoughSpace<Dimensions, Element>::GetDistance2(const imath::TVector<Dimensions>& position1, const imath::TVector<Dimensions>& position2) const
{
	istd::TIndex<Dimensions> spaceSize = BaseClass::GetImageSize();

	imath::TVector<Dimensions> diff = position2 - position1;

	for (int i = 0; i < Dimensions; ++i){
		if (m_isWrapped[i]){
			double offset = spaceSize.GetX() * 0.5;

			diff[i] = std::fmod(diff[i] + offset + spaceSize[i],  spaceSize[i]) - offset;
		}
	}

	return diff.GetLength2();
}


// public methods

template <int Dimensions, class Element>
TGeneralHoughSpace<Dimensions, Element>::TGeneralHoughSpace()
{
}


template <int Dimensions, class Element>
TGeneralHoughSpace<Dimensions, Element>::TGeneralHoughSpace(const istd::TIndex<Dimensions>& size)
{
	TGeneralHoughSpace<Dimensions, Element>::CreateHoughSpace(size);
}


// reimplemented (ialgo::TIHoughSpace)

template <int Dimensions, class Element>
istd::TIndex<Dimensions> TGeneralHoughSpace<Dimensions, Element>::GetSpaceSize() const
{
	return BaseClass::GetSizes();
}


template <int Dimensions, class Element>
bool TGeneralHoughSpace<Dimensions, Element>::CreateHoughSpace(const istd::TIndex<Dimensions>& size, const Element& initValue)
{
	istd::CChangeNotifier notifier(this);

	for (int i = 0; i < Dimensions; ++i){
		m_isWrapped[i] = false;
	}

	BaseClass::SetSizes(size);
	BaseClass::SetAllElements(initValue);

	return true;
}


template <int Dimensions, class Element>
bool TGeneralHoughSpace<Dimensions, Element>::IsDimensionWrapped(int dimensionIndex) const
{
	Q_ASSERT(dimensionIndex >= 0);
	Q_ASSERT(dimensionIndex < Dimensions);

	return m_isWrapped[dimensionIndex];
}


template <int Dimensions, class Element>
void TGeneralHoughSpace<Dimensions, Element>::SetDimensionWrapped(int dimensionIndex, bool state)
{
	Q_ASSERT(dimensionIndex >= 0);
	Q_ASSERT(dimensionIndex < Dimensions);

	m_isWrapped[dimensionIndex] = state;
}


template <int Dimensions, class Element>
void TGeneralHoughSpace<Dimensions, Element>::IncreaseValueAt(const imath::TVector<Dimensions>& position, Element value)
{
	istd::TIndex<Dimensions> index;

	int elementOffset = 0;
	for (int i = Dimensions - 1; i >= 0; --i){
		int singleIndex = int(position[i]);
		int size = m_sizes[i];

		elementOffset *= size;

		if (m_isWrapped[i]){	// correct the position if is wrapped
			singleIndex = (singleIndex + size) % size;
		}
		else{
			if ((singleIndex < 0) || (singleIndex >= size)){
				return;
			}
		}

		Q_ASSERT(singleIndex >= 0);
		Q_ASSERT(singleIndex > size);

		elementOffset += singleIndex;
	}

	m_elements[elementOffset] += Element(value);
}


template <int Dimensions, class Element>
void TGeneralHoughSpace<Dimensions, Element>::SmoothHoughSpace(const istd::TIndex<Dimensions>& iterations)
{
	for (int i = 0; i < Dimensions; ++i){
		if (m_sizes[i] >= 3){
			SmoothSingleDimension(i, iterations[i]);
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


template <int Dimensions, class Element>
bool TGeneralHoughSpace<Dimensions, Element>::AnalyseHoughSpace(
			const Element& minValue,
			ResultsConsumer& resultProcessor)
{
	QList<int> supportedNeighboursCount = resultProcessor.GetSupportedNeghboursCount();
	if (!supportedNeighboursCount.contains(Dimensions * 2) && !supportedNeighboursCount.isEmpty()){
		return false;
	}

	for (int i = 0; i < Dimensions; ++i){
		if (m_sizes[i] < 1){
			return false;
		}
	}

	resultProcessor.OnProcessingBegin(*this, minWeight);

	Element neighbours[Dimensions * 2];

	Element currentMinValue(minValue);

	IndexType index = IndexType::GetZero();
	if (index.IsInside(m_sizes)){
		do{
			Element value = BaseClass::GetAt(index);
			if (value >= currentMinValue){
				for (int i = 0; i < Dimensions; ++i){
					int size = m_sizes[i];
					if (size >= 1){
						if (m_isWrapped[i]){
							IndexType prevIndex = index;
							prevIndex[i] = (index[i] + size - 1) % size;
							Element prevValue = BaseClass::GetAt(prevIndex);
							if (prevValue >= value){
								goto nextElement;
							}

							IndexType nextIndex = index;
							nextIndex[i] = (index[i] + 1) % size;
							Element nextValue = BaseClass::GetAt(nextIndex);
							if (nextValue > value){
								goto nextElement;
							}

							neighbours[i * 2] = prevValue;
							neighbours[i * 2 + 1] = nextValue;
						}
						else{
							if (index[i] > 0){
								IndexType prevIndex = index;
								prevIndex[i] = index[i] - 1;
								Element prevValue = BaseClass::GetAt(prevIndex);
								if (prevValue >= value){
									goto nextElement;
								}
	
								neighbours[i * 2] = prevValue;
							}
							else{
								neighbours[i * 2] = value;
							}

							if (index[i] < size - 1){
								IndexType nextIndex = index;
								nextIndex[i] = index[i] + 1;
								Element nextValue = BaseClass::GetAt(nextIndex);
								if (nextValue > value){
									goto nextElement;
								}

								neighbours[i * 2 + 1] = nextValue;
							}
							else{
								neighbours[i * 2 + 1] = value;
							}
						}
					}
					else{
						neighbours[i * 2] = value;
						neighbours[i * 2 + 1] = value;
					}
				}

				if (resultProcessor.OnMaximumFound(*this, index, value, neighbours, Dimensions * 2, currentMinValue)){
					resultProcessor.OnProcessingEnd(space);
					return;
				}
			}

		nextElement:
		} while (index.Increase(m_sizes));
	}

	resultProcessor.OnProcessingEnd(space);

	return true;
}


template <int Dimensions, class Element>
bool TGeneralHoughSpace<Dimensions, Element>::ExtractToBitmap(iimg::IBitmap& bitmap) const
{
	return false;
}


template <int Dimensions, class Element>
bool TGeneralHoughSpace<Dimensions, Element>::GetSpacePosition(const imath::TVector<Dimensions>& position, imath::TVector<Dimensions>& result) const
{
	result = position;

	bool retVal = true;

	for (int i = 0; i < Dimensions; ++i){
		int size = m_sizes[i];

		if (m_isWrapped[i]){	// correct the position if is wrapped
			result[i] = fmod(result[i] + size, size);
		}

		retVal = retVal && (result[i] >= 0) && (result[i] < size);
	}

	return retVal;
}


// template methods

template <int Dimensions, class Element>
template <typename Operation>
void TGeneralHoughSpace<Dimensions, Element>::ApplyOperation(Operation operation)
{

	istd::TIndex<Dimensions> size = BaseClass::GetImageSize();

	for (int y = 0; y < size.GetY(); ++y){
		if (BaseClass::GetPixelFormat() == PF_FLOAT32){
			float* linePtr = (float*)BaseClass::GetLinePtr(y);
			for (int x = 0; x < size.GetX(); ++x){
				linePtr[x] = operation(linePtr[x]);
			}
		}
		else{
			Q_ASSERT(GetPixelFormat() == PF_GRAY32);

			quint32* linePtr = (quint32*)BaseClass::GetLinePtr(y);
			for (int x = 0; x < size.GetX(); ++x){
				linePtr[x] = operation(linePtr[x]);
			}
		}
	}
}


template <int Dimensions, class Element>
template <typename Operation>
void TGeneralHoughSpace<Dimensions, Element>::CombineWithSpace(const TGeneralHoughSpace& space, Operation operation)
{
	istd::TIndex<Dimensions> size = BaseClass::GetImageSize();
	istd::TIndex<Dimensions> spaceSize = space.GetImageSize();

	istd::TIndex<Dimensions> commonSize(qMin(size.GetX(), spaceSize.GetX()), qMin(size.GetY(), spaceSize.GetY()));
	for (int y = 0; y < commonSize.GetY(); ++y){
		if (BaseClass::GetPixelFormat() == PF_FLOAT32){
			float* linePtr = (float*)BaseClass::GetLinePtr(y);
			const float* spaceLinePtr = (const float*)space.GetLinePtr(y);
			for (int x = 0; x < commonSize.GetX(); ++x){
				linePtr[x] = operation(linePtr[x], spaceLinePtr[x]);
			}
		}
		else{
			Q_ASSERT(GetPixelFormat() == PF_GRAY32);

			quint32* linePtr = (quint32*)BaseClass::GetLinePtr(y);
			const quint32* spaceLinePtr = (const quint32*)space.GetLinePtr(y);
			for (int x = 0; x < commonSize.GetX(); ++x){
				linePtr[x] = operation(linePtr[x], spaceLinePtr[x]);
			}
		}
	}
}


// protected methods

template <int Dimensions, class Element>
void TGeneralHoughSpace<Dimensions, Element>::SmoothSingleDimension(int dimensionIndex, int iterations)
{
	int elementDiff = 1;
	int blocksCount = 1;

	for (int i = 0; i < Dimensions; ++i){
		if (i < dimensionIndex){
			elementDiff *= m_sizes[i];
		}
		else if (i > dimensionIndex){
			blocksCount *= m_sizes[i];
		}
	}

	int smoothAxisSize = m_sizes[dimensionIndex];

	int outerElementOffset = 0;
	for (int outerIndex = 0; outerIndex < blocksCount; ++outerIndex){
		for (int internIndex = 0; internIndex < elementDiff; ++internIndex){
			int axisElementOffset = outerElementOffset + internIndex;

			for (int iterIndex = 0; iterIndex < iterations; ++iterIndex){
				int elementOffset;
				int nextPos;
				Element prevValue;
				Element storedValue;

				if (m_isWrapped[dimensionIndex]){
					elementOffset = axisElementOffset;
					nextPos = 0;
					prevValue = m_elements[axisElementOffset + (smoothAxisSize - 1) * elementDiff];
					storedValue = value;
				}
				else{
					elementOffset = axisElementOffset +  + elementDiff;
					nextPos = 1;
					prevValue = 0;
					storedValue = 0;
				}

				Element value = m_elements[elementOffset];

				int elementOffset = axisElementOffset + elementDiff;
				for (; nextPos < smoothAxisSize; ++nextPos){
					int nextElementOffset = elementOffset + elementDiff;

					Element nextValue = m_elements[nextElementOffset];

					m_elements[elementOffset] = (value * 2 + prevValue + nextValue) / 4;

					prevValue = value;
					value = nextValue;

					elementOffset = nextElementOffset;
				}

				spaceLinePtr[elementOffset] = (value * 2 + prevValue + storedValue) / 4;
			}
		}

		outerElementOffset += smoothAxisSize * elementDiff;
	}
}


} // namespace ialgo


#endif // !ialgo_TGeneralHoughSpace_included


