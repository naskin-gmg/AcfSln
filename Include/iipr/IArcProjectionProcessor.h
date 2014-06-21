#ifndef iipr_IArcProjectionProcessor_included
#define iipr_IArcProjectionProcessor_included


#include "i2d/CArc.h"
#include "iproc/IProcessor.h"
#include "iimg/IBitmap.h"
#include "imeas/IDataSequence.h"


namespace iipr
{


class IProjectionParams;


class IArcProjectionProcessor: virtual public iproc::IProcessor
{
public:
	/**
		Do line projection with explicit projection parameters.
	*/
	virtual bool DoProjection(
				const iimg::IBitmap& bitmap,
                const i2d::CArc& projectionLine,
				const IProjectionParams* paramsPtr,
				imeas::IDataSequence& results) = 0;
};


} // namespace iipr


#endif // !iipr_IArcProjectionProcessor_included


