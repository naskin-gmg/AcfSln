#include "iipr/CCaliperFeature.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CCaliperFeature::CCaliperFeature(
			double weight,
			double position,
			int edgeMode)
:	BaseClass(weight),
	m_position(position),
	m_edgeMode(edgeMode)
{
}


double CCaliperFeature::GetPosition() const
{
	return m_position;
}


int CCaliperFeature::GetEdgeMode() const
{
	return m_edgeMode;
}


// reimplemented (iser::ISerializable)

bool CCaliperFeature::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag valueTag("Position", "Position on projection");
	static iser::CArchiveTag edgeModeTag("EdgeMode", "Edge mode");

	bool retVal = true;

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	retVal = retVal && BaseClass::Serialize(archive);

	retVal = retVal && archive.BeginTag(valueTag);
	retVal = retVal && archive.Process(m_position);
	retVal = retVal && archive.EndTag(valueTag);

	retVal = retVal && archive.BeginTag(edgeModeTag);
	retVal = retVal && archive.Process(m_edgeMode);
	retVal = retVal && archive.EndTag(edgeModeTag);

	return retVal;
}


} // namespace iipr

