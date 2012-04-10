#ifndef iipr_IFeaturesProvider_included
#define iipr_IFeaturesProvider_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/IChangeable.h"


namespace iipr
{


class IFeature;


/**
	Common interface for someone who can provide access to list of features.
*/
class IFeaturesProvider: virtual public istd::IChangeable
{
public:
	typedef QVector<const iipr::IFeature*> Features;

	/**
		Get container features.
	*/
	virtual Features GetFeatures() const = 0;
};


} // namespace iipr


#endif // !iipr_IFeaturesProvider_included

