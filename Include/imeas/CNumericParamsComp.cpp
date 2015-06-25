#include "imeas/CNumericParamsComp.h"


#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imath/CDoubleManip.h"


namespace imeas
{


CNumericParamsComp::CNumericParamsComp()
:	imod::CMultiModelBridgeBase(this)
{
}


// reimplemented (imeas::INumericValue)

const INumericConstraints* CNumericParamsComp::GetNumericConstraints() const
{
	if (m_constraintsCompPtr.IsValid()){
		return m_constraintsCompPtr.GetPtr();
	}

	return NULL;
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


} // namespace imeas


