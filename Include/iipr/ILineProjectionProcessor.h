#ifndef iipr_ILineProjectionProcessor_included
#define iipr_ILineProjectionProcessor_included


#include "i2d/CLine2d.h"

#include "iproc/IProcessor.h"

#include "iimg/IBitmap.h"

#include "imeas/IDataSequence.h"


namespace iipr
{


class IProjectionParams;


class ILineProjectionProcessor: virtual public iproc::IProcessor
{
public:
	/**
		Do line projection with explicite projection parameters.
	*/
	virtual bool DoProjection(
				const iimg::IBitmap& bitmap,
				const i2d::CLine2d& projectionLine,
				const IProjectionParams* paramsPtr,
				imeas::IDataSequence& results) = 0;
};


} // namespace iipr


#endif // !iipr_ILineProjectionProcessor_included


