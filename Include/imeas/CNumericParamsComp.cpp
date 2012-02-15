#include "imeas/CNumericParamsComp.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imath/CDoubleManip.h"


namespace imeas
{


// reimplemented (imeas::INumericParams)

const INumericConstraints* CNumericParamsComp::GetNumericConstraints() const
{
	if (m_constraintsCompPtr.IsValid()){
		return m_constraintsCompPtr.GetPtr();
	}

	return this;
}


imath::CVarVector CNumericParamsComp::GetValues() const
{
	return m_values;
}


bool CNumericParamsComp::SetValues(const imath::CVarVector& lengths)
{
	if (m_values != lengths){
		const INumericConstraints* constraintsPtr = GetNumericConstraints();
		I_ASSERT(constraintsPtr != NULL);
		if (lengths.GetElementsCount() != constraintsPtr->GetNumericValuesCount()){
			return false;
		}

		istd::CChangeNotifier notifier(this);

		m_values = lengths;
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool CNumericParamsComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag valuesTag("Values", "List of numeric values");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool retVal = archive.BeginTag(valuesTag);
	retVal = retVal && m_values.Serialize(archive);
	retVal = retVal && archive.EndTag(valuesTag);

	const INumericConstraints* constraintsPtr = GetNumericConstraints();
	I_ASSERT(constraintsPtr != NULL);

	m_values.SetElementsCount(constraintsPtr->GetNumericValuesCount());

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CNumericParamsComp::CopyFrom(const IChangeable& object)
{
	const INumericParams* nativeParamsPtr = dynamic_cast<const INumericParams*>(&object);
	if (nativeParamsPtr != NULL){
		imath::CVarVector values = nativeParamsPtr->GetValues();

		const INumericConstraints* constraintsPtr = GetNumericConstraints();
		if (constraintsPtr != NULL){
			values.SetElementsCount(constraintsPtr->GetNumericValuesCount(), 0);
		}

		if (values != m_values){
			istd::CChangeNotifier notifier(this);

			m_values = nativeParamsPtr->GetValues();
		}

		return true;
	}

	return false;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CNumericParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	const INumericConstraints* constraintsPtr = GetNumericConstraints();
	I_ASSERT(constraintsPtr != NULL);

	int count = constraintsPtr->GetNumericValuesCount();
	m_values.SetElementsCount(count);

	int defaultValuesCount = m_defaultValuesAttrPtr.GetCount();

	double lastValue = 0;
	for (int i = 0; i < count; ++i){
		if (i < defaultValuesCount){
			lastValue = m_defaultValuesAttrPtr[i];
		}

		m_values[i] = lastValue;
	}
}


// reimplemented (imeas::INumericConstraints)

int CNumericParamsComp::GetNumericValuesCount() const
{
	return *m_dimensionsCountAttrPtr;
}


istd::CString CNumericParamsComp::GetNumericValueDescription(int index) const
{
	return istd::CString("Value ") + istd::CString::FromNumber(index + 1);
}


const imeas::IUnitInfo& CNumericParamsComp::GetNumericValueUnitInfo(int /*index*/) const
{
	return *this;
}


// reimplemented (imeas::IUnitInfo)

int CNumericParamsComp::GetUnitType() const
{
	return UT_UNKNOWN;
}


istd::CString CNumericParamsComp::GetUnitName() const
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

