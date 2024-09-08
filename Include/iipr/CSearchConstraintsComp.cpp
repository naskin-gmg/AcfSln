#include <iipr/CSearchConstraintsComp.h>


namespace iipr
{


// public methods

// reimplemented (iipr::ISearchConstraints)

int CSearchConstraintsComp::GetSearchSupportedFlags() const
{
	if (m_searchContraintsBaseCompPtr.IsValid()){
		return m_searchContraintsBaseCompPtr->GetSearchSupportedFlags();
	}

	return SSF_NONE;
}


const istd::CRange& CSearchConstraintsComp::GetRotationRangeConstraints() const
{
	return m_rotationContraints;
}


const istd::CRange& CSearchConstraintsComp::GetScaleRangeConstraints() const
{
	return m_scaleContraints;
}


const istd::CIntRange& CSearchConstraintsComp::GetResultsCountConstraints() const
{
	return m_resultCountContraints;
}


// reimplemented (icomp::CComponentBase)

void CSearchConstraintsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	istd::CRange baseRotation;
	istd::CRange baseScale;
	istd::CIntRange baseResultCount;

	if (m_searchContraintsBaseCompPtr.IsValid()){
		baseRotation = m_searchContraintsBaseCompPtr->GetRotationRangeConstraints();
		baseScale = m_searchContraintsBaseCompPtr->GetScaleRangeConstraints();
		baseResultCount = m_searchContraintsBaseCompPtr->GetResultsCountConstraints();

		// rotation
		if (m_rotationContraintsCompPtr.IsValid()){
			istd::CRange setRotation = m_rotationContraintsCompPtr->GetNumericValueUnitInfo(0)->GetValueRange();
			double rotationMin = (setRotation.GetMinValue() > baseRotation.GetMinValue()) ? setRotation.GetMinValue() : baseRotation.GetMinValue();
			double rotationMax = (setRotation.GetMaxValue() < baseRotation.GetMaxValue()) ? setRotation.GetMaxValue() : baseRotation.GetMaxValue();
			m_rotationContraints = istd::CRange(rotationMin, rotationMax);
		}
		else{
			m_rotationContraints = baseRotation;
		}

		// scale
		if (m_scaleRangeContraintsCompPtr.IsValid()){
			istd::CRange setScale = m_scaleRangeContraintsCompPtr->GetNumericValueUnitInfo(0)->GetValueRange();
			double scaleMin = (setScale.GetMinValue() > baseScale.GetMinValue()) ? setScale.GetMinValue() : baseScale.GetMinValue();
			double scaleMax = (setScale.GetMaxValue() < baseScale.GetMaxValue()) ? setScale.GetMaxValue() : baseScale.GetMaxValue();
			m_scaleContraints = istd::CRange(scaleMin, scaleMax);
		}
		else{
			m_scaleContraints = baseScale;
		}

		// model occurence count
		if (m_modelOccurenceLimitationCompPtr.IsValid()){
			imath::CVarVector setCountContraints = m_modelOccurenceLimitationCompPtr->GetValues();
			int maxCount = 10000;
			int minCount = 0;
			if (setCountContraints.GetElementsCount() == 1){
				maxCount = setCountContraints.GetElement(0);
			}
			else if (setCountContraints.GetElementsCount() > 1){
				int element0 = setCountContraints.GetElement(0);
				int element1 = setCountContraints.GetElement(1);
				maxCount = qMax(element0, element1);
				minCount = qMin(element0, element1);
			}

			minCount = qMax(minCount, baseResultCount.GetMinValue());
			maxCount = qMin(maxCount, baseResultCount.GetMaxValue());
			m_resultCountContraints = istd::CIntRange(minCount, maxCount).GetValidated();
		}
		else{
			m_resultCountContraints = baseResultCount;
		}
	}
	else{
		// set empty ranges
	}
}


} // namespace iipr


