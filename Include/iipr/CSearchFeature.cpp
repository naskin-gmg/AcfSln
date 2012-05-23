#include "iipr/CSearchFeature.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


// public methods

CSearchFeature::CSearchFeature(
			double weight,
			const i2d::CVector2d& position,
			double angle,
			const i2d::CVector2d& scale,
			int index)
:	BaseClass(weight),
	m_scale(scale),
	m_angle(angle),
	m_index(index)
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


int CSearchFeature::GetIndex() const
{
	return m_index;
}


// reimplemented (iser::ISerializable)

bool CSearchFeature::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag angleTag("Angle", "Angle of found model");
	static iser::CArchiveTag scaleTag("Scale", "Scale of found model");
	static iser::CArchiveTag indexTag("Index", "Index of found model");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);

	retVal = retVal && archive.BeginTag(angleTag);
	retVal = retVal && archive.Process(m_angle);
	retVal = retVal && archive.EndTag(angleTag);

	retVal = retVal && archive.BeginTag(scaleTag);
	retVal = retVal && m_scale.Serialize(archive);
	retVal = retVal && archive.EndTag(scaleTag);

	retVal = retVal && archive.BeginTag(indexTag);
	retVal = retVal && archive.Process(m_index);
	retVal = retVal && archive.EndTag(indexTag);

	return retVal;
}


} // namespace iipr


