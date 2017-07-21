#ifndef ialgo_TSimpleSpaceResultConsumer_included
#define ialgo_TSimpleSpaceResultConsumer_included


// Qt includes
#include <QtCore/QMultiMap>

// ACF includes
#include <imath/TVector.h>

// ACF-Solutions includes
#include <ialgo/TIHoughSpace.h>


namespace ialgo
{


/**
	Consumer of results generated for some multidimensional space (typically Hough space).
*/
template <int Dimensions, class Element = double>
class TSimpleSpaceResultConsumer:
			public iimg::CGeneralBitmap,
			virtual public TIHoughSpace<Dimensions, Element>::ResultsConsumer
{
public:
	typedef imath::TVector<Dimensions> PosVector;
	/**
		List of weights and positions.
	*/
	typedef QMultiMap<double, PosVector> PosMap;

	/**
		Construct the result processor.
		\param	maxPoints				maximal number of points beeing in result set.
		\param	maxConsideredPoints		maximal number of points stored internally.
										Setting this number to smaller value speeds up the processing.
		\param	minDistance				Minimal distance between the points.
										Please note that the neighbourshood will be analysed at end.
	*/
	TSimpleSpaceResultConsumer(int maxPoints, int maxConsideredPoints, double minDistance, double minMaxRatio);

	// reimplemented (ialgo::TIHoughSpace::ResultsConsumer)
	virtual QList<int> GetSupportedNeghboursCount() const;
	virtual void OnProcessingBegin(
				const TIHoughSpace<Dimensions, Element>& space,
				const double& minValue);
	virtual void OnProcessingEnd(const TIHoughSpace<Dimensions, Element>& space);
	virtual bool OnMaximumFound(
				const TIHoughSpace<Dimensions, Element>& space,
				const istd::TIndex<2>& position,
				const double& value,
				const double* neghboursPtr,
				int neghboursCount,
				double& minValue);

	PosMap positions;

private:
	int m_maxPoints;
	int m_maxConsideredPoints;
	double m_minDistance;
	double m_minMaxRatio;

	double m_maxValue;
};


// public methods

// reimplemented (ialgo::TIHoughSpace::ResultsConsumer)

template <int Dimensions, class Element>
TSimpleSpaceResultConsumer<Dimensions, Element>::TSimpleSpaceResultConsumer(int maxPoints, int maxConsideredPoints, double minDistance, double minMaxRatio)
:	m_maxPoints(maxPoints),
	m_maxConsideredPoints(maxConsideredPoints),
	m_minDistance(minDistance),
	m_minMaxRatio(minMaxRatio)
{
}


template <int Dimensions, class Element>
QList<int> TSimpleSpaceResultConsumer<Dimensions, Element>::GetSupportedNeghboursCount() const
{
	return QList<int>() << Dimensions * 2;
}


template <int Dimensions, class Element>
void TSimpleSpaceResultConsumer<Dimensions, Element>::OnProcessingBegin(
			const TIHoughSpace<Dimensions, Element>& /*space*/,
			const double& /*minValue*/)
{
	m_maxValue = 0;
}


template <int Dimensions, class Element>
void TSimpleSpaceResultConsumer<Dimensions, Element>::OnProcessingEnd(const TIHoughSpace<Dimensions, Element>& space)
{
	PosMap finalPositions;

	if (m_maxPoints != 0){
		// remove elements beeing to close to each other
		for (		PosMap::ConstIterator pointIter = positions.constBegin();
					pointIter != positions.constEnd();
					++pointIter){
			const PosVector& point1 = pointIter.value();

			bool isToClose = false;

			for (		PosMap::ConstIterator searchClosedIter = finalPositions.constBegin();
						searchClosedIter != finalPositions.constEnd();
						++searchClosedIter){
				const PosVector& point2 = searchClosedIter.value();

				double dist2 = space.GetDistance2(point1, point2);
				if (dist2 <= m_minDistance * m_minDistance){
					isToClose = true;

					break;
				}
			}

			if (!isToClose){
				finalPositions.insertMulti(-pointIter.key(), point1);

				if ((m_maxPoints >= 0) && (finalPositions.size() >= m_maxPoints)){
					break;
				}
			}
		}
	}

	positions.swap(finalPositions);
}


template <int Dimensions, class Element>
bool TSimpleSpaceResultConsumer<Dimensions, Element>::OnMaximumFound(
			const TIHoughSpace<Dimensions, Element>& /*space*/,
			const istd::TIndex<2>& position,
			const double& value,
			const double* neghboursPtr,
			int neghboursCount,
			double& minValue)
{
	Q_ASSERT(neghboursCount == Dimensions * 2);
	Q_UNUSED(neghboursCount);

	PosVector resultPos;

	for (int dimensionIndex = 0; dimensionIndex < Dimensions; ++dimensionIndex){
		double diffLeft = value - neghboursPtr[dimensionIndex * 2];
		double diffRight = value - neghboursPtr[dimensionIndex * 2 + 1];

		double correction = diffLeft / (diffLeft + diffRight);

		resultPos[dimensionIndex] = position[dimensionIndex] + correction;
	}

	positions.insert(-value, resultPos);

	if (value > m_maxValue){
		m_maxValue = value;
		double propValue = value * m_minMaxRatio;
		if (minValue < propValue){
			minValue = propValue;

			// remove elements weeker than new calculated minValue
			while (!positions.isEmpty() && (-positions.lastKey() < minValue)){
				typename PosMap::Iterator lastIter = positions.end() - 1;

				positions.erase(lastIter);
			}
		}
	}

	// try remove the last one if we have too many points
	if (positions.count() > m_maxConsideredPoints){
		typename PosMap::Iterator lastIter = positions.end() - 1;
		double lastValue = -lastIter.key();
		positions.erase(lastIter);

		if (lastValue > minValue){
			minValue = lastValue;
		}
	}

	return false;
}


} // namespace ialgo


#endif // !ialgo_TSimpleSpaceResultConsumer_included


