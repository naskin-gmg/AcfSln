#include "imeas/CNumericParamsComp.h"


#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imath/CDoubleManip.h"


namespace imeas
{


// reimplemented (imeas::INumericValue)

const INumericConstraints* CNumericParamsComp::GetNumericConstraints() const
{
	if (m_constraintsCompPtr.IsValid()){
		return m_constraintsCompPtr.GetPtr();
	}

	return this;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CNumericParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	const INumericConstraints* constraintsPtr = GetNumericConstraints();
	Q_ASSERT(constraintsPtr != NULL);

	int count = constraintsPtr->GetNumericValuesCount();
	m_values.SetElementsCount(count);

	int defaultValuesCount = m_defaultValuesAttrPtr.GetCount();

	double lastValue = 0;
	for (int i = 0; i < count; ++i){
		if (i < defaultValuesCount){
			lastValue = m_defaultValuesAttrPtr[i];
		}

		// correct the value according to the constraints
		const imath::IUnitInfo& unitInfo = constraintsPtr->GetNumericValueUnitInfo(i);
		if (unitInfo.GetValueRange().IsValid()){
			m_values[i] = unitInfo.GetValueRange().GetClipped(lastValue);
		} else {
			m_values[i] = lastValue;
		}
	}

	if (m_constraintsModelCompPtr.IsValid()){
		m_constraintsModelCompPtr->AttachObserver(this);
	}
}


void CNumericParamsComp::OnComponentDestroyed()
{
	EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imeas::INumericConstraints)

int CNumericParamsComp::GetNumericValuesCount() const
{
	return *m_dimensionsCountAttrPtr;
}


QString CNumericParamsComp::GetNumericValueName(int index) const
{
	return QString("Value ") + QString().setNum(index + 1);
}


QString CNumericParamsComp::GetNumericValueDescription(int /*index*/) const
{
	return QString();
}


const imath::IUnitInfo& CNumericParamsComp::GetNumericValueUnitInfo(int /*index*/) const
{
	return *this;
}


// reimplemented (imath::IUnitInfo)

int CNumericParamsComp::GetUnitType() const
{
	return UT_UNKNOWN;
}


QString CNumericParamsComp::GetUnitName() const
{
	return "";
}


double CNumericParamsComp::GetDisplayMultiplicationFactor() const
{
	return 1;
}


istd::CRange CNumericParamsComp::GetValueRange() const
{
	return istd::CRange(*m_minValueAttrPtr, *m_maxValueAttrPtr);
}


const imath::IDoubleManip& CNumericParamsComp::GetValueManip() const
{
	static imath::CDoubleManip manip;

	return manip;
}


} // namespace imeas

