#include "imeas/CLinearAdjustParamsComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace imeas
{


CLinearAdjustParamsComp::CLinearAdjustParamsComp()
:	m_scale(1.0),
	m_offset(0),
	m_offsetUnitInfo(imath::IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(0, 1)),
	m_scaleUnitInfo(imath::IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(0, 1))
{
}


// reimplemented (imeas::ILinearAdjustParams)

const ILinearAdjustConstraints* CLinearAdjustParamsComp::GetAdjustConstraints() const
{
	return m_constraintsCompPtr.GetPtr();
}


double CLinearAdjustParamsComp::GetScaleFactor() const
{
	return m_scale;
}


void CLinearAdjustParamsComp::SetScaleFactor(double scale)
{
	if (scale != m_scale){
		istd::CChangeNotifier notifier(this);

		m_scale = scale;
	}
}


double CLinearAdjustParamsComp::GetOffsetFactor() const
{
	return m_offset;
}


void CLinearAdjustParamsComp::SetOffsetFactor(double offset)
{
	if (offset != m_offset){
		istd::CChangeNotifier notifier(this);

		m_offset = offset;
	}
}


// reimplemented (imeas::ILinearAdjustParams)

bool CLinearAdjustParamsComp::IsValueTypeSupported(ValueTypeId valueTypeId) const
{
	if (valueTypeId == VTI_AUTO){
		return true;
	}

	return false;
}


const INumericConstraints* CLinearAdjustParamsComp::GetNumericConstraints() const
{
	return this;
}


imath::CVarVector CLinearAdjustParamsComp::GetComponentValue(ValueTypeId valueTypeId) const
{
	if (!IsValueTypeSupported(valueTypeId)){
		return imath::CVarVector();
	}

	return GetValues();
}


imath::CVarVector CLinearAdjustParamsComp::GetValues() const
{
	imath::CVarVector retVal(2, 0);

	retVal[0] = m_offset;
	retVal[1] = m_scale;

	return retVal;
}


bool CLinearAdjustParamsComp::SetValues(const imath::CVarVector& values)
{
	if (values.GetElementsCount() >= 1){
		m_offset = values[0];
	}

	if (values.GetElementsCount() >= 2){
		m_scale = values[1];
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool CLinearAdjustParamsComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag scaleTag("ScaleFactor", "Scale factor of linear adjust");
	static iser::CArchiveTag offsetTag("OffsetFactor", "Offset factor of linear adjust");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(scaleTag);
	retVal = retVal && archive.Process(m_scale);
	retVal = retVal && archive.EndTag(scaleTag);

	retVal = retVal && archive.BeginTag(offsetTag);
	retVal = retVal && archive.Process(m_offset);
	retVal = retVal && archive.EndTag(offsetTag);

	return retVal;
}


// protected methods

// reimplemented (imeas::INumericConstraints)

int CLinearAdjustParamsComp::GetNumericValuesCount() const
{
	return 2;
}


QString CLinearAdjustParamsComp::GetNumericValueName(int index) const
{
	switch (index){
	case 0:
		return QObject::tr("Offset");

	case 1:
		return QObject::tr("Scale");

	default:
		return "";
	}
}


QString CLinearAdjustParamsComp::GetNumericValueDescription(int index) const
{
	switch (index){
	case 0:
		return QObject::tr("Offset value");

	case 1:
		return QObject::tr("Scale value");

	default:
		return "";
	}
}


const imath::IUnitInfo& CLinearAdjustParamsComp::GetNumericValueUnitInfo(int index) const
{
	switch (index){
	case 0:
		return m_offsetUnitInfo;

	default:
		return m_scaleUnitInfo;
	}
}


// reimplemented (icomp::CComponentBase)

void CLinearAdjustParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_constraintsCompPtr.IsValid()){
		m_offsetUnitInfo.SetValueRange(m_constraintsCompPtr->GetOffsetFactorRange());
		m_scaleUnitInfo.SetValueRange(m_constraintsCompPtr->GetScaleFactorRange());
	}
}


} // namespace imeas


