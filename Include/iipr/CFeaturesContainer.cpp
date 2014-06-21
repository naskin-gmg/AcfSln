#include "iipr/CFeaturesContainer.h"


// ACF includes
#include "istd/CChangeNotifier.h"


namespace iipr
{


void CFeaturesContainer::RemoveFeature(int featureIndex)
{
	m_featuresList.RemoveAt(featureIndex);
}


imeas::INumericValue& CFeaturesContainer::GetFeatureRef(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < GetValuesCount());

	imeas::INumericValue* featurePtr = m_featuresList.GetAt(index);
	Q_ASSERT(featurePtr != NULL);

	return *featurePtr;
}


// reimplemented (iipr::IFeaturesConsumer)

void CFeaturesContainer::ResetFeatures()
{
	istd::CChangeNotifier changePtr(this);

	m_featuresList.Reset();
}


bool CFeaturesContainer::AddFeature(const imeas::INumericValue* featurePtr, bool* /*isFullPtr*/)
{
	if (featurePtr != NULL){
		istd::CChangeNotifier changePtr(this);

		m_featuresList.PushBack(const_cast<imeas::INumericValue*>(featurePtr));
		
		return true;
	}

	return false;
}



// reimplemented (imeas::INumericValueProvider)

int CFeaturesContainer::GetValuesCount() const
{
	return m_featuresList.GetCount();
}


const imeas::INumericValue& CFeaturesContainer::GetNumericValue(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < GetValuesCount());

	const imeas::INumericValue* featurePtr = m_featuresList.GetAt(index);
	Q_ASSERT(featurePtr != NULL);

	return *featurePtr;
}


} // namespace iipr


