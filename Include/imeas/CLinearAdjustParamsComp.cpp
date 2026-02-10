// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <imeas/CLinearAdjustParamsComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace imeas
{


CLinearAdjustParamsComp::CLinearAdjustParamsComp()
:	m_scale(1.0),
	m_offset(0),
	m_offsetUnitInfo(imath::IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(0, 1)),
	m_scaleUnitInfo(imath::IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(0, 1))
{
	m_valuesInfo.InsertOption(QObject::tr("Offset"), "Offset", QObject::tr("Offset value"));
	m_valuesInfo.InsertOption(QObject::tr("Scale"), "Scale", QObject::tr("Scale value"));
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
	istd::CChangeNotifier notifier(this);

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
	static iser::CArchiveTag scaleTag("ScaleFactor", "Scale factor of linear adjust", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag offsetTag("OffsetFactor", "Offset factor of linear adjust", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

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

const iprm::IOptionsList& CLinearAdjustParamsComp::GetValueListInfo() const
{
	return m_valuesInfo;
}


const imath::IUnitInfo* CLinearAdjustParamsComp::GetNumericValueUnitInfo(int index) const
{
	switch (index){
	case 0:
		return &m_offsetUnitInfo;

	case 1:
		return &m_scaleUnitInfo;

	default:
		return NULL;
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


