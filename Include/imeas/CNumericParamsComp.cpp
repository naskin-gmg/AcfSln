#include <imeas/CNumericParamsComp.h>


// ACF includes
#include <imath/CDoubleManip.h>


namespace imeas
{


// public methods

// reimplemented (imeas::INumericValue)

const INumericConstraints* CNumericParamsComp::GetNumericConstraints() const
{
	if (m_constraintsCompPtr.IsValid()){
		return m_constraintsCompPtr.GetPtr();
	}

	return NULL;
}


// protected methods

// reimplemented (imeas::INumericValue)

void CNumericParamsComp::OnUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
	const INumericConstraints* constraintsPtr = GetNumericConstraints();
	if (constraintsPtr != NULL){
		const iprm::IOptionsList& valueListInfo = constraintsPtr->GetValueListInfo();
		int count = valueListInfo.GetOptionsCount();

		istd::CChangeNotifier changeNotifier(this);

		for (int i = 0; (i < count) && (i < m_values.GetElementsCount()); ++i){
			// correct the value according to the constraints
			const imath::IUnitInfo* unitInfoPtr = constraintsPtr->GetNumericValueUnitInfo(i);
			if ((unitInfoPtr != NULL) && unitInfoPtr->GetValueRange().IsValid()){
				m_values[i] = unitInfoPtr->GetValueRange().GetClipped(m_values[i]);
			}
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CNumericParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int count = 0;
	int defaultValuesCount = m_defaultValuesAttrPtr.GetCount();

	const INumericConstraints* constraintsPtr = GetNumericConstraints();
	if (constraintsPtr != NULL){
		const iprm::IOptionsList& valueListInfo = constraintsPtr->GetValueListInfo();

		count = valueListInfo.GetOptionsCount();

		// observe constrains
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(const_cast<INumericConstraints*>(constraintsPtr));
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
		}
	}
	else{
		count = defaultValuesCount;
	}

	m_values.SetElementsCount(count);

	double lastValue = 0;
	for (int i = 0; i < count; ++i){
		if (i < defaultValuesCount){
			lastValue = m_defaultValuesAttrPtr[i];
		}

		if (constraintsPtr != NULL){
			// correct the value according to the constraints
			const imath::IUnitInfo* unitInfoPtr = constraintsPtr->GetNumericValueUnitInfo(i);
			if ((unitInfoPtr != NULL) && unitInfoPtr->GetValueRange().IsValid()){
				m_values[i] = unitInfoPtr->GetValueRange().GetClipped(lastValue);
			}
			else{
				m_values[i] = lastValue;
			}
		}
		else{
			m_values[i] = lastValue;
		}
	}
}


void CNumericParamsComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace imeas


