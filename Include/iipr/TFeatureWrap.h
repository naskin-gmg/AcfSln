#ifndef iipr_TFeatureWrap_included
#define iipr_TFeatureWrap_included


#include "iipr/CFeatureBase.h"


namespace iipr
{


/**
	Wrapper allowing to create feature object from some another serializable one.
*/
template <class BaseObject>
class TFeatureWrap:
			public CFeatureBase,
			public BaseObject
{
public:
	typedef CFeatureBase BaseClass;
	typedef BaseObject BaseClass2;

	TFeatureWrap(double weight = 1.0);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
};


// public methods

template <class BaseObject>
TFeatureWrap<BaseObject>::TFeatureWrap(double weight)
:	BaseClass(weight)
{
}


// reimplemented (iser::ISerializable)

template <class BaseObject>
bool TFeatureWrap<BaseObject>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);
	retVal = retVal && BaseClass2::Serialize(archive);

	return retVal;
}


} // namespace iipr


#endif // !iipr_TFeatureWrap_included


