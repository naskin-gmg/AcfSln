#include "iipr/CSingleFeatureConsumer.h"


// IACF includes
#include "imeas/INumericValue.h"


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


// reimplemented (iipr::IFeaturesConsumer)

void CSingleFeatureConsumer::ResetFeatures()
{
	m_featurePtr.Reset();
}


bool CSingleFeatureConsumer::AddFeature(const imeas::INumericValue* featurePtr, bool* isFullPtr)
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
				if (		m_featurePtr.IsValid() &&
							featurePtr->IsValueTypeSupported(imeas::INumericValue::VTI_WEIGHT) && 
							m_featurePtr->IsValueTypeSupported(imeas::INumericValue::VTI_WEIGHT)){
				
					double featureWeight = featurePtr->GetComponentValue(imeas::INumericValue::VTI_WEIGHT).GetElement(0);
					double currentWeight = m_featurePtr->GetComponentValue(imeas::INumericValue::VTI_WEIGHT).GetElement(0);
				
					if (featureWeight > currentWeight){
						m_featurePtr.SetPtr(featurePtr);
					}
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


// reimplemented (imeas::INumericValueProvider)

int CSingleFeatureConsumer::GetValuesCount() const
{
	return m_featurePtr.IsValid() ? 1 : 0;
}


const imeas::INumericValue& CSingleFeatureConsumer::GetNumericValue(int I_IF_DEBUG(index)) const
{
	I_ASSERT(m_featurePtr.IsValid());
	I_ASSERT(index == 0);

	return *m_featurePtr.GetPtr();
}


} // namespace iipr


