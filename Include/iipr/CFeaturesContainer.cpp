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

	return *m_featuresList.GetAt(index);
}


} // namespace iipr


