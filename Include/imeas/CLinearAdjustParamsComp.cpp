#include "imeas/CLinearAdjustParamsComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

// ACF-Solutions includes
#include "imeas/CGeneralUnitInfo.h"


namespace imeas
{


CLinearAdjustParamsComp::CLinearAdjustParamsComp()
:	m_scale(1.0),
	m_offset(0)
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

const INumericConstraints* CLinearAdjustParamsComp::GetNumericConstraints() const
{
	return this;
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
	bool retVal = true;

	static iser::CArchiveTag scaleTag("ScaleFactor", "Scale factor of linear adjust");
	retVal = retVal && archive.BeginTag(scaleTag);
	retVal = retVal && archive.Process(m_scale);
	retVal = retVal && archive.EndTag(scaleTag);

	static iser::CArchiveTag offsetTag("OffsetFactor", "Offset factor of linear adjust");
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


const imeas::IUnitInfo& CLinearAdjustParamsComp::GetNumericValueUnitInfo(int index) const
{
	static imeas::CGeneralUnitInfo offsetUnitInfo(IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(-1, 1));
	static imeas::CGeneralUnitInfo scaleUnitInfo(IUnitInfo::UT_RELATIVE, "%", 100, istd::CRange(0, 2));

	switch (index){
	case 0:
		return offsetUnitInfo;

	default:
		return scaleUnitInfo;
	}
}


} // namespace imeas


