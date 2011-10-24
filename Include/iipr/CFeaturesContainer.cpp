#include "iipr/CFeaturesContainer.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iipr
{


// reimplemented (iipr::IFeaturesConsumer)

void CFeaturesContainer::ResetFeatures()
{
	istd::CChangeNotifier changePtr(this);

	m_featuresList.Reset();
}


bool CFeaturesContainer::AddFeature(const iipr::IFeature* featurePtr, bool* /*isFullPtr*/)
{
	if (featurePtr != NULL){
		istd::CChangeNotifier changePtr(this);

		m_featuresList.PushBack(const_cast<iipr::IFeature*>(featurePtr));
		
		return true;
	}

	return false;
}


// reimplemented (iipr::IFeaturesProvider)

iipr::IFeaturesProvider::Features CFeaturesContainer::GetFeatures() const
{
	iipr::IFeaturesProvider::Features featuresList;
	
	for (int index = 0; index < m_featuresList.GetCount(); index++){
		featuresList.push_back(m_featuresList.GetAt(index));
	}

	return featuresList;
}


} // namespace iipr


