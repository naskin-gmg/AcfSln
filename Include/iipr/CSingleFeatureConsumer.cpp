#include "iipr/CSingleFeatureConsumer.h"


// IACF includes
#include "iipr/IFeature.h"


namespace iipr
{


// public methods
	
CSingleFeatureConsumer::CSingleFeatureConsumer(int featurePolicy)
	:m_featurePolicy(featurePolicy)
{
}


int CSingleFeatureConsumer::GetFeaturePolicy() const
{
	return m_featurePolicy;
}


void CSingleFeatureConsumer::SetFeaturePolicy(int featurePolicy)
{
	ResetFeatures();

	m_featurePolicy = featurePolicy;
}


const IFeature* CSingleFeatureConsumer::GetFeature() const
{
	return m_featurePtr.GetPtr();
}


// reimplemented (iipr::IFeaturesConsumer)

void CSingleFeatureConsumer::ResetFeatures()
{
	m_featurePtr.Reset();
}


bool CSingleFeatureConsumer::AddFeature(const IFeature* featurePtr, bool* isFullPtr)
{
	I_ASSERT(featurePtr != NULL);
	if (!m_featurePtr.IsValid() || m_featurePolicy == FP_LAST){
		m_featurePtr.SetPtr(featurePtr);
	}
	else{
		switch (m_featurePolicy){
			case FP_FIRST:
				delete featurePtr;
				break;

			case FP_HEAVIEST:
				if (featurePtr->GetWeight() > m_featurePtr->GetWeight()){
					m_featurePtr.SetPtr(featurePtr);
				}
				else{
					delete featurePtr;
				}
				break;
		}
	}

	if (isFullPtr != NULL){
		*isFullPtr = false;
	}

	return true;
}


// reimplemented (iipr::IFeaturesProvider)

IFeaturesProvider::Features CSingleFeatureConsumer::GetFeatures() const
{
	Features retVal;

	if (m_featurePtr.IsValid()){
		retVal.push_back(m_featurePtr.GetPtr());
	}

	return retVal;
}


} // namespace iipr


