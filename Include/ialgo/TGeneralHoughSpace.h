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

	/**
		Set if this space to be wrapped horizontaly or not.
		Space is horizonally wrapped if the left pixel is neighbour of the right one.
	*/
	void SetDimensionWrapped(int dimensionIndex, bool state);

	/**
		Set mode of extension mode to border or not.
	*/
	void SetExtensionBorder(int dimensionIndex, bool state);

	/**
		Get the distance scaling factors used to calculate how far are elements in this space.
	*/
	double GetDistScalingFactor(int dimensionIndex) const;
	/**
		Set the distance scaling factors used to calculate how far are elements in this space.
	*/
	void SetDistScalingFactor(int dimensionIndex, double factor);

	// reimplemented (ialgo::TIHoughSpace)
	virtual istd::TIndex<Dimensions> GetSpaceSize() const;
	virtual bool CreateHoughSpace(const istd::TIndex<Dimensions>& size, const Element& initValue = 0);
	virtual bool IsDimensionWrapped(int dimensionIndex) const;
	virtual ExtensionMode GetExtensionMode(int dimensionIndex) const;
	virtual void IncreaseValueAt(const imath::TVector<Dimensions>& position, Element value);
	virtual void SmoothHoughSpace(const istd::TIndex<Dimensions>& iterations);
	virtual bool AnalyseHoughSpace(
				const Element& minValue,
				ResultsConsumer& resultProcessor) const;
	virtual bool ExtractToBitmap(iimg::IBitmap& bitmap) const;
	virtual bool GetSpacePosition(const imath::TVector<Dimensions>& position, imath::TVector<Dimensions>& result) const;
	virtual double GetSpaceDistance(const imath::TVector<Dimensions>& position1, const imath::TVector<Dimensions>& position2) const;
	virtual double GetSpaceDistance2(const imath::TVector<Dimensions>& position1, const imath::TVector<Dimensions>& position2) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	void SmoothSingleDimension(int dimensionIndex, int iterations);

private:
	bool m_isWrapped[Dimensions];
	bool m_extensionModeBorders[Dimensions];
	double m_distScalingFactors[Dimensions];
};


// inline methods

template <int Dimensions, class Element>
inline double TGeneralHoughSpace<Dimensions, Element>::GetSpaceDistance(const imath::TVector<Dimensions>& position1, const imath::TVector<Dimensions>& position2) const
{
	return qSqrt(GetSpaceDistance2(position1, position2));
}


template <int Dimensions, class Element>
inline double TGeneralHoughSpace<Dimensions, Element>::GetSpaceDistance2(const imath::TVector<Dimensions>& position1, const imath::TVector<Dimensions>& position2) const
{
	imath::TVector<Dimensions> diff = position2 - position1;

	double distance2 = 0;

	for (int i = 0; i < Dimensions; ++i){
		double diff = position2[i] - position1[i];

		if (m_isWrapped[i]){
			double offset = m_sizes[i] * 0.5;

			diff = std::fmod(diff + offset + m_sizes[i], m_sizes[i]) - offset;
		}

		diff *= m_distScalingFactors[i];

		distance2 += diff * diff;
	}

	return distance2;
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


template <int Dimensions, class Element>
void TGeneralHoughSpace<Dimensions, Element>::SetDimensionWrapped(int dimensionIndex, bool state)
{
	Q_ASSERT(dimensionIndex >= 0);
	Q_ASSERT(dimensionIndex < Dimensions);

	m_isWrapped[dimensionIndex] = state;
}


template <int Dimensions, class Element>
void TGeneralHoughSpace<Dimensions, Element>::SetExtensionBorder(int dimensionIndex, bool state)
{
	Q_ASSERT(dimensionIndex >= 0);
	Q_ASSERT(dimensionIndex < Dimensions);

	m_extensionModeBorders[dimensionIndex] = state;
}


template <int Dimensions, class Element>
double TGeneralHoughSpace<Dimensions, Element>::GetDistScalingFactor(int dimensionIndex) const
{
	Q_ASSERT(dimensionIndex >= 0);
	Q_ASSERT(dimensionIndex < Dimensions);

	return m_distScalingFactors[dimensionIndex];
}


template <int Dimensions, class Element>
void TGeneralHoughSpace<Dimensions, Element>::SetDistScalingFactor(int dimensionIndex, double factor)
{
	Q_ASSERT(dimensionIndex >= 0);
	Q_ASSERT(dimensionIndex < Dimensions);

	m_distScalingFactors[dimensionIndex] = factor;
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
		m_extensionModeBorders[i] = false;
		
		m_distScalingFactors[i] = 1;
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
typename TIHoughSpace<Dimensions, Element>::ExtensionMode TGeneralHoughSpace<Dimensions, Element>::GetExtensionMode(int dimensionIndex) const
{
	Q_ASSERT(dimensionIndex >= 0);
	Q_ASSERT(dimensionIndex < Dimensions);

	return m_extensionModeBorders[dimensionIndex]? EM_ZERO: EM_BORDER;
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
		Q_ASSERT(singleIndex < size);

		elementOffset += singleIndex;
	}

	m_elements[elementOffset] += Element(value);
}


template <int Dimensions, class Element>
void TGeneralHoughSpace<Dimensions, Element>::SmoothHoughSpace(const istd::TIndex<Dimensions>& iterations)
{
	for (int i = 0; i < Dimensions; ++i){
		if (m_sizes[i] >= 3){
			int iterCount = iterations[i];
			if (iterCount > 0){
				SmoothSingleDimension(i, iterCount);
			}
		}
	}
}


template <int Dimensions, class Element>
bool TGeneralHoughSpace<Dimensions, Element>::AnalyseHoughSpace(
			const Element& minValue,
			ResultsConsumer& resultProcessor) const
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

	resultProcessor.OnProcessingBegin(*this, minValue);

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
							else if (m_extensionModeBorders[i]){
								neighbours[i * 2] = value;
							}
							else{
								neighbours[i * 2] = 0;
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
							else if (m_extensionModeBorders[i]){
								neighbours[i * 2 + 1] = value;
							}
							else{
								neighbours[i * 2 + 1] = 0;
							}
						}
					}
					else{
						neighbours[i * 2] = value;
						neighbours[i * 2 + 1] = value;
					}
				}

				if (resultProcessor.OnMaximumFound(*this, index, value, neighbours, Dimensions * 2, currentMinValue)){
					resultProcessor.OnProcessingEnd(*this);

					return true;
				}
			}

		nextElement:;
		} while (index.Increase(m_sizes));
	}

	resultProcessor.OnProcessingEnd(*this);

	return true;
}


template <int Dimensions, class Element>
bool TGeneralHoughSpace<Dimensions, Element>::ExtractToBitmap(iimg::IBitmap& bitmap) const
{
	istd::CIndex2d bitmapSize(0, 1);

	for (int i = 0; i < Dimensions; ++i){
		if (i == 0){
			bitmapSize[0] = m_sizes[i];
		}
		else{
			bitmapSize[1] *= m_sizes[i];
		}
	}

	if (!bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, bitmapSize)){
		return false;
	}

	Element maxValue = 0;
	for (Elements::const_iterator iter = m_elements.begin(); iter != m_elements.end(); ++iter){
		Element value = *iter;

		if (value > maxValue){
			maxValue = value;
		}
	}

	if (maxValue <= 0){
		bitmap.ClearImage();

		return true;
	}

	Elements::const_iterator iter = m_elements.begin();
	for (int y = 0; y < bitmapSize.GetY(); ++y){
		quint8* outputLinePtr = (quint8*)bitmap.GetLinePtr(y);

		for (int x = 0; x < bitmapSize.GetX(); ++x){
			outputLinePtr[x] = quint8(*iter * 255 / maxValue);

			++iter;
		}
	}

	return true;
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
	for (Iterator iter = BaseClass::Begin(); iter != BaseClass::End(); ++iter){
		Element& value = *iter;

		value = operation(value);
	}
}


template <int Dimensions, class Element>
template <typename Operation>
void TGeneralHoughSpace<Dimensions, Element>::CombineWithSpace(const TGeneralHoughSpace& space, Operation operation)
{
	istd::TIndex<Dimensions> commonSize;
	for (int i = 0; i < Dimensions; ++i){
		commonSize[i] = qMin(m_sizes[i], space.m_sizes[i]);
	}

	istd::TIndex<Dimensions> index = istd::TIndex<Dimensions>::GetZero();
	if (index.IsInside(commonSize)){
		do{
			Element& destValue = BaseClass::GetAt(index);
			const Element& secondValue = space.BaseClass::GetAt(index);

			destValue = operation(destValue, secondValue);
		} while (index.Increase(commonSize));
	}
}


// reimplemented (iser::ISerializable)
template <int Dimensions, class Element>
bool TGeneralHoughSpace<Dimensions, Element>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag spaceSizeTag("SpaceSize", "Size of Hough space", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag dimensionSizeTag("DimensionSize", "Size of single dimension", iser::CArchiveTag::TT_GROUP, &spaceSizeTag);
	static iser::CArchiveTag elementsTag("Elements", "List of space elements", iser::CArchiveTag::TT_GROUP, &spaceSizeTag);

	bool retVal = true;

	bool isStoring = archive.IsStoring();

	IndexType spaceSize = BaseClass::GetSizes();
	int dimensionsCount = Dimensions;

	retVal = retVal && archive.BeginMultiTag(spaceSizeTag, dimensionSizeTag, dimensionsCount);
	if (dimensionsCount != Dimensions){
		return false;
	}

	for (int dimensionIndex = 0; dimensionIndex < Dimensions; ++dimensionIndex){
		retVal = retVal && archive.BeginTag(dimensionSizeTag);
		retVal = retVal && archive.Process(spaceSize[dimensionIndex]);
		retVal = retVal && archive.EndTag(dimensionSizeTag);
	}
	retVal = retVal && archive.EndTag(spaceSizeTag);

	if (!isStoring){
		if (!BaseClass::SetSizes(spaceSize)){
			return false;
		}
	}

	retVal = retVal && archive.BeginTag(elementsTag);
	for (BaseClass::Iterator iter = BaseClass::Begin(); iter != BaseClass::End(); ++iter){
		retVal = retVal && archive.Process(*iter);
	}
	retVal = retVal && archive.EndTag(elementsTag);

	return retVal;
}


// protected methods

template <int Dimensions, class Element>
void TGeneralHoughSpace<Dimensions, Element>::SmoothSingleDimension(int dimensionIndex, int iterations)
{
	int elementDiff = 1;
	int blocksCount = 1;
	int elementsCount = int(m_elements.size());

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
				int elementOffset = axisElementOffset;
				int nextPos;
				Element value;
				Element prevValue;
				Element storedValue;

				if (m_isWrapped[dimensionIndex]){
					value = m_elements[elementOffset];
					nextPos = 0;
					prevValue = m_elements[axisElementOffset + (smoothAxisSize - 1) * elementDiff];
					storedValue = value;
				}
				else{
					value = m_elements[elementOffset];
					nextPos = 1;
					prevValue = 0;
					storedValue = 0;
				}

				for (; nextPos < smoothAxisSize; ++nextPos){
					int nextElementOffset = (elementOffset + elementDiff) % elementsCount;

					Element nextValue = m_elements[nextElementOffset];

					m_elements[elementOffset] = (value * 2 + prevValue + nextValue) / 4;

					prevValue = value;
					value = nextValue;

					elementOffset = nextElementOffset;
				}

				m_elements[elementOffset] = (value * 2 + prevValue + storedValue) / 4;
			}
		}

		outerElementOffset += smoothAxisSize * elementDiff;
	}
}


} // namespace ialgo


#endif // !ialgo_TGeneralHoughSpace_included


