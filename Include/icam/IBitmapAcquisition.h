#ifndef icam_IBitmapAcquisition_included
#define icam_IBitmapAcquisition_included


// ACF includes
#include "istd/CIndex2d.h"
#include "iproc/IProcessor.h"


namespace icam
{


/**
	Bitmap acquisition processor.
	This processor uses optional input parameter of type \c istd::ITimeStamp to identify the image by its trigger time.
*/
class IBitmapAcquisition: virtual public iproc::IProcessor
{
public:
	/**
		Get size of acquired bitmap for specified parameters.
		\return		if size of bitmap is known it returns this value. Otherwise invalid size will be returned.
					\sa \c istd::TIndex::IsValid().
	*/
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const = 0;
};


} // namespace icam


#endif // !icam_IBitmapAcquisition_included


