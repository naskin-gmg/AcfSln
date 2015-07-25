#ifndef iedge_CEdgesReductorComp_included
#define iedge_CEdgesReductorComp_included


#include <QtCore/QBitArray>

// ACF includes
#include "istd/TSmartPtr.h"
#include "imath/CGeneralUnitInfo.h"
#include "iproc/TSyncProcessorCompBase.h"

// ACF-Solutions includes
#include "imeas/INumericValue.h"
#include "imeas/INumericConstraints.h"

#include "iedge/IEdgeLinesProcessor.h"


namespace iedge
{


class CEdgesReductorComp:
			public iproc::TSyncProcessorCompBase<IEdgeLinesProcessor>,
			virtual public imeas::INumericConstraints
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

	CEdgesReductorComp();

	void GetReducedLine(
				const CEdgeLine& edgeLine,
				double positionTolerance,
				double weightTolerance,
				CEdgeLine& result) const;

	void GetReducedLines(
				const CEdgeLineContainer& edgeLines,
				double positionTolerance,
				double weightTolerance,
				CEdgeLineContainer& result) const;

	// reimplemented (iedge::IEdgeLinesProcessor)
	virtual bool DoLinesProcessing(
				const iprm::IParamsSet* paramsPtr,
				const CEdgeLineContainer& edgeLines,
				CEdgeLineContainer& result) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (imeas::INumericConstraints)
	virtual int GetNumericValuesCount() const;
	virtual QString GetNumericValueName(int index) const;
	virtual QString GetNumericValueDescription(int index) const;
	virtual const imath::IUnitInfo* GetNumericValueUnitInfo(int index) const;

protected:
	typedef QBitArray NodesToRemove;

	// cache of the nodes to avoid constant reallocations
	mutable NodesToRemove m_nodesToRemove;

	/**
		Mark points to remove.
		\return	number of removed points.
	*/
	int ReduceNodes(
				const CEdgeLine& edgeLine,
				double positionTolerance,
				double weightTolerance,
				int firstIndex,
				int lastIndex,
				NodesToRemove& nodesToRemove,
				int& firstInsideIndex,
				int& lastInsideIndex) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(imeas::INumericValue, m_defaultToleranceParamsCompPtr);
	I_ATTR(QByteArray, m_toleranceParamsIdAttrPtr);
	I_REF(imath::IUnitInfo, m_distanceUnitInfoCompPtr);

	imath::CGeneralUnitInfo m_distanceUnitInfo;
};


} // namespace iedge


#endif // !iedge_CEdgesReductorComp_included


