#ifndef ialgo_TNeighbourBinCaller_included
#define ialgo_TNeighbourBinCaller_included


// ACF includes
#include <istd/CVarIndex.h>


namespace ialgo
{


/**
	Class providing mechanism to iterate over neighbours in some multidimensional discrete space.
*/
template <int SpaceDimension>
class TNeighbourBinCaller
{
public:
	TNeighbourBinCaller();

	void InitForCyclicSpace(const istd::CVarIndex& spaceSizes, int leftTolerance, int rightTolerance);

	template <typename Visitor>
	void VisitAllCorrelations(Visitor& visitor)
	{
		istd::TIndex<CFastModelFinder::FEATURE_LEVELS_COUNT> angleDeltaSizes;
		angleDeltaSizes.SetAllTo(m_leftTolerance + m_rightTolerance + 1);

		visitor.BeginVisiting();

		istd::TIndex<SpaceDimension> imageAngleIndices = istd::TIndex<SpaceDimension>::GetZero();
		do{
			int imageBinIndex = imageAngleIndices.GetIterationIndex(m_spaceSizes);

			if (visitor.BeginImageBin(imageBinIndex)){
				istd::CVarIndex angleDeltaIndices(m_spaceSizes.GetDimensionsCount(), 0);
				do{
					istd::TIndex<SpaceDimension> modelAngleIndices;
					for (int i = 0; i < SpaceDimension; ++i){
						modelAngleIndices[i] = (imageAngleIndices[i] + angleDeltaIndices[i] + m_spaceSizes[i] - m_leftTolerance) % m_spaceSizes[i];
					}

					int modelBinIndex = modelAngleIndices.GetIterationIndex(m_spaceSizes);

					visitor.VisitCorrelation(imageBinIndex, modelBinIndex);
				} while (angleDeltaIndices.Increase(angleDeltaSizes));
			}

			visitor.EndImageBin(imageBinIndex);
		} while (imageAngleIndices.Increase(m_spaceSizes));

		visitor.EndVisiting();
	}

private:
	istd::CVarIndex m_spaceSizes;
	int m_leftTolerance;
	int m_rightTolerance;
};


// public methods

template <int SpaceDimension>
TNeighbourBinCaller<SpaceDimension>::TNeighbourBinCaller()
:	m_spaceSizes(),
	m_leftTolerance(0),
	m_rightTolerance(0)
{
}


template <int SpaceDimension>
void TNeighbourBinCaller<SpaceDimension>::InitForCyclicSpace(const istd::CVarIndex& spaceSizes, int leftTolerance, int rightTolerance)
{
	m_spaceSizes = spaceSizes;
	m_leftTolerance = leftTolerance;
	m_rightTolerance = rightTolerance;
}


} // namespace ialgo


#endif // !ialgo_TNeighbourBinCaller_included


