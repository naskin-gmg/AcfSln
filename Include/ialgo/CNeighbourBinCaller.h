#ifndef ialgo_CNeighbourBinCaller_included
#define ialgo_CNeighbourBinCaller_included


// ACF includes
#include <istd/CVarIndex.h>


namespace ialgo
{


/**
	Class providing mechanism to iterate over neighbours in some multidimensional discrete space.
*/
class CNeighbourBinCaller
{
public:
	CNeighbourBinCaller();

	void InitForCyclicSpace(const istd::CVarIndex& spaceSizes, int leftTolerance, int rightTolerance);

	template <typename Visitor>
	void VisitAllCorrelations(Visitor& visitor)
	{
		istd::TIndex<CFastModelFinder::FEATURE_LEVELS_COUNT> angleDeltaSizes;
		angleDeltaSizes.SetAllTo(m_leftTolerance + m_rightTolerance + 1);

		visitor.BeginVisiting();

		istd::TIndex<CFastModelFinder::FEATURE_LEVELS_COUNT> imageAngleIndices = istd::TIndex<CFastModelFinder::FEATURE_LEVELS_COUNT>::GetZero();
		do{
			int imageBinIndex = imageAngleIndices.GetIterationIndex(m_spaceSizes);

			if (visitor.BeginImageBin(imageBinIndex)){
				istd::CVarIndex angleDeltaIndices(m_spaceSizes.GetDimensionsCount(), 0);
				do{
					istd::TIndex<CFastModelFinder::FEATURE_LEVELS_COUNT> modelAngleIndices;
					for (int i = 0; i < CFastModelFinder::FEATURE_LEVELS_COUNT; ++i){
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


} // namespace ialgo


#endif // !ialgo_CNeighbourBinCaller_included


