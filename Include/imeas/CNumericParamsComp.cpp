#include "imeas/CNumericParamsComp.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imath/CDoubleManip.h"


namespace imeas
{


// reimplemented (imeas::INumericParams)

const INumericConstraints* CNumericParamsComp::GetConstraints() const
{
	if (m_constraintsCompPtr.IsValid()){
		return m_constraintsCompPtr.GetPtr();
	}

	return this;
}


imath::CVarVector CNumericParamsComp::GetValues() const
{
	return m_filterLengts;
}


bool CNumericParamsComp::SetValues(const imath::CVarVector& lengths)
{
	if (m_filterLengts != lengths){
		const INumericConstraints* constraintsPtr = GetConstraints();
		I_ASSERT(constraintsPtr != NULL);
		if (lengths.GetElementsCount() != constraintsPtr->GetFilterDimensionsCount()){
			return false;
		}

		istd::CChangeNotifier notifier(this);

		m_filterLengts = lengths;
	}

	return true;
}


// reimplemented (imeas::INumericConstraints)

int CNumericParamsComp::GetFilterDimensionsCount() const
{
	return *m_dimensionsCountAttrPtr;
}


istd::CString CNumericParamsComp::GetFilterDescription(int dimension) const
{
	return istd::CString("Value ") + istd::CString::FromNumber(dimension + 1);
}


const imeas::IUnitInfo& CNumericParamsComp::GetFilterUnitInfo(int /*dimension*/) const
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
	return istd::CRange(*m_minFilterLengthAttrPtr, *m_maxFilterLengthAttrPtr);
}


const imath::IDoubleManip& CNumericParamsComp::GetValueManip() const
{
	static imath::CDoubleManip manip;

	return manip;
}


// reimplemented (iser::ISerializable)

bool CNumericParamsComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag filterLengthsTag("FilterLengts", "List of filter lengths for each dimension");

	bool retVal = archive.BeginTag(filterLengthsTag);
	retVal = retVal && m_filterLengts.Serialize(archive);
	retVal = retVal && archive.EndTag(filterLengthsTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CNumericParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	const INumericConstraints* constraintsPtr = GetConstraints();
	I_ASSERT(constraintsPtr != NULL);

	int count = constraintsPtr->GetFilterDimensionsCount();
	m_filterLengts.SetElementsCount(count, 1);

	if (m_filterLengthsAttrPtr.IsValid()){
		int commonCount = istd::Min(count, m_filterLengthsAttrPtr.GetCount());
		for (int i = 0; i < commonCount; ++i){
			m_filterLengts[i] = m_filterLengthsAttrPtr[i];
		}
	}
}


} // namespace imeas

