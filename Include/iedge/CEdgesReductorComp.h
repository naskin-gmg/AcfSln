#ifndef iedge_CEdgesReductorComp_included
#define iedge_CEdgesReductorComp_included


// ACF includes
#include "istd/TSmartPtr.h"
#include "iproc/TSyncProcessorCompBase.h"

// ACF-Solutions includes
#include "imeas/INumericParams.h"
#include "imeas/INumericConstraints.h"

// QSF includes
#include "iedge/IEdgeLinesProcessor.h"


namespace iedge
{


class CEdgesReductorComp: public iproc::TSyncProcessorCompBase<IEdgeLinesProcessor>
{
public:
	typedef iproc::TSyncProcessorCompBase<IEdgeLinesProcessor> BaseClass;

	I_BEGIN_COMPONENT(CEdgesReductorComp);
		I_REGISTER_INTERFACE(IEdgeLinesProcessor);
		I_REGISTER_INTERFACE(imeas::INumericConstraints);
		I_ASSIGN(m_defaultToleranceParamsCompPtr, "DefaultToleranceParams", "Default parameter for thresholds, if not taken from parameters", false, "DefaultToleranceParams");
		I_ASSIGN(m_toleranceParamsIdAttrPtr, "ToleranceParamsId", "Parameter ID (imeas::INumericValue) for reduction tolerance", false, "ReductionTolerances");
		I_ASSIGN(m_distanceUnitInfoCompPtr, "DistanceUnitInfo", "Unit information used for distance tolerance", false, "DistanceUnit");
	I_END_COMPONENT;

	void GetReducedLine(
				const CEdgeLine& edgeLine,
				double positionTolerance,
				double weightTolerance,
				CEdgeLine& result) const;
	void GetReducedLines(
				const CEdgeLine::Container& edgeLines,
				double positionTolerance,
				double weightTolerance,
				CEdgeLine::Container& result) const;

	// reimplemented (iedge::IEdgeLinesProcessor)
	virtual bool DoLinesProcessing(
				const iprm::IParamsSet* paramsPtr,
				const CEdgeLine::Container& edgeLines,
				CEdgeLine::Container& result) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (imeas::INumericConstraints)
	virtual int GetNumericValuesCount() const;
	virtual QString GetNumericValueName(int index) const;
	virtual QString GetNumericValueDescription(int index) const;
	virtual const imeas::IUnitInfo& GetNumericValueUnitInfo(int index) const;

protected:
	struct NodeInfo
	{
		int index;
		double removedDistance;		// distance to line if removed
		double removedWeightDiff;	// difference to interpolated weight if removed
	};

	typedef QVector<NodeInfo> NodeInfos;

	bool ReduceNodes(
				const CEdgeLine& edgeLine,
				double positionTolerance,
				double weightTolerance,
				NodeInfos::Iterator firstIter,
				NodeInfos::Iterator endIter,
				NodeInfos& nodeInfos) const;

private:
	I_REF(imeas::INumericParams, m_defaultToleranceParamsCompPtr);
	I_ATTR(QByteArray, m_toleranceParamsIdAttrPtr);
	I_REF(imeas::IUnitInfo, m_distanceUnitInfoCompPtr);
};


} // namespace iedge


#endif // !iedge_CEdgesReductorComp_included


