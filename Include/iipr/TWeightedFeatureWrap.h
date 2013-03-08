#ifndef iipr_TFeatureWrap_included
#define iipr_TFeatureWrap_included


// ACF includes
#include "istd/TChangeNotifier.h"

// ACF-Solutions includes
#include "imeas/CSimpleNumericValue.h"


namespace iipr
{


/**
	Wrapper allowing to create feature object from some another serializable one.
*/
template <class BaseObject>
class TWeightedFeatureWrap:
			public imeas::CSimpleNumericValue,
			public BaseObject
{
public:
	typedef imeas::CSimpleNumericValue BaseClass;
	typedef BaseObject BaseClass2;

	TWeightedFeatureWrap(double weight = 1.0);

	double GetWeight() const;
	void SetWeight(double weight);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode);

protected:
	double m_weight;
};


// public methods

template <class BaseObject>
TWeightedFeatureWrap<BaseObject>::TWeightedFeatureWrap(double weight)
	:m_weight(weight)
{
}


template <class BaseObject>
double TWeightedFeatureWrap<BaseObject>::GetWeight() const
{
	return m_weight;
}


template <class BaseObject>
void TWeightedFeatureWrap<BaseObject>::SetWeight(double weight)
{
	m_weight = weight;
}


// reimplemented (iser::ISerializable)

template <class BaseObject>
bool TWeightedFeatureWrap<BaseObject>::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);
	retVal = retVal && BaseClass2::Serialize(archive);

	iser::CArchiveTag weightTag("Weight", "Weight");
	retVal = retVal && archive.BeginTag(weightTag);
	retVal = retVal && archive.Process(m_weight);
	retVal = retVal && archive.EndTag(weightTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

template <class BaseObject>
bool TWeightedFeatureWrap<BaseObject>::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const TWeightedFeatureWrap<BaseObject>* objectPtr = dynamic_cast<const TWeightedFeatureWrap<BaseObject>*>(&object);
	if (objectPtr != NULL){
		m_weight = objectPtr->GetWeight();
	}

	return BaseObject::CopyFrom(object, mode);
}


} // namespace iipr


#endif // !iipr_TFeatureWrap_included


