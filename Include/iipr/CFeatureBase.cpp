#include "iipr/CFeatureBase.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


// public methods

CFeatureBase::CFeatureBase(double weight)
:	m_weight(weight)
{
}


void CFeatureBase::SetWeight(double weight)
{
	if (weight != m_weight){
		istd::CChangeNotifier notifier(this);

		m_weight = weight;
	}
}


// reimplemented (iipr::IFeature)

double CFeatureBase::GetWeight() const
{
	return m_weight;
}


// reimplemented (iser::ISerializable)

bool CFeatureBase::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag weightTag("Weight", "Weight of feature");
	retVal = retVal && archive.BeginTag(weightTag);
	retVal = retVal && archive.Process(m_weight);
	retVal = retVal && archive.EndTag(weightTag);

	return retVal;
}


} // namespace iipr

