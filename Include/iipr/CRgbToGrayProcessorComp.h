#ifndef iipr_CRgbToGrayProcessorComp_included
#define iipr_CRgbToGrayProcessorComp_included


// ACF includes
#include "istd/itr.h"

#include "iimg/IBitmap.h"

#include "iproc/TSyncProcessorCompBase.h"


namespace iipr
{


/**	
	Implementation of a processor for the RGB to Grayscale image conversion.
*/
class CRgbToGrayProcessorComp: public iproc::CSyncProcessorCompBase
{
public:
	I_DECLARE_TR_FUNCTION(CRgbToGrayProcessorComp);

	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CRgbToGrayProcessorComp);
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

private:
	bool ConvertImage(const iimg::IBitmap& input, iimg::IBitmap& outputBitmap) const;
};


} // namespace iipr


#endif // !iipr_CRgbToGrayProcessorComp_included

