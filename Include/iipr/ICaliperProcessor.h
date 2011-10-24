#ifndef iipr_ICaliperProcessor_included
#define iipr_ICaliperProcessor_included


// ACF includes
#include "iproc/IProcessor.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"

#include "iipr/IFeaturesConsumer.h"


namespace iipr
{


class ICaliperParams;


class ICaliperProcessor: virtual public iproc::IProcessor
{
public:
	enum FeatureTypeId
	{
		/**
			Unique ID of caliper fearutes.
		*/
		FTI_CALIPER_FEATURE = 6572
	};

	virtual bool DoCaliper(
				const imeas::IDataSequence& derivative,
				const ICaliperParams* paramsPtr,
				IFeaturesConsumer& results) = 0;
};


} // namespace iipr


#endif // !iipr_ICaliperProcessor_included


