#include "iipr/CSearchFeature.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


// public methods

CSearchFeature::CSearchFeature(
			double weight,
			const i2d::CVector2d& position,
			double angle,
			const i2d::CVector2d& scale)
:	BaseClass(weight),
	m_scale(scale),
	m_angle(angle)
{
	BaseClass::SetPosition(position);
}


double CSearchFeature::GetAngle() const
{
	return m_angle;
}


const i2d::CVector2d& CSearchFeature::GetScale() const
{
	return m_scale;
}


// reimplemented (iser::ISerializable)

bool CSearchFeature::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);

	static iser::CArchiveTag angleTag("Angle", "Angle of found model");
	retVal = retVal && archive.BeginTag(angleTag);
	retVal = retVal && archive.Process(m_angle);
	retVal = retVal && archive.EndTag(angleTag);

	static iser::CArchiveTag scaleTag("Scale", "Scale of found model");
	retVal = retVal && archive.BeginTag(scaleTag);
	retVal = retVal && m_scale.Serialize(archive);
	retVal = retVal && archive.EndTag(scaleTag);

	return retVal;
}


} // namespace iipr

