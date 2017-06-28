#include <ialgo/CNeighbourBinCaller.h>


namespace ialgo
{


CNeighbourBinCaller::CNeighbourBinCaller()
:	m_spaceSizes(),
	m_leftTolerance(0),
	m_rightTolerance(0)
{
}


void CNeighbourBinCaller::InitForCyclicSpace(const istd::CVarIndex& spaceSizes, int leftTolerance, int rightTolerance)
{
	m_spaceSizes = spaceSizes;
	m_leftTolerance = leftTolerance;
	m_rightTolerance = rightTolerance;
}


} // namespace ialgo


