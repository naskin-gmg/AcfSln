#ifndef iipr_CImageBinarizeProcessorComp_included
#define iipr_CImageBinarizeProcessorComp_included


// ACF includes
#include "iimg/IBitmap.h"
#include "iproc/TSyncProcessorCompBase.h"

// ACF-Solutions includes
#include "imeas/INumericParams.h"


namespace iipr
{


/**	
	Processor for image binarization using static threshold.
*/
class CImageBinarizeProcessorComp: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CImageBinarizeProcessorComp);
		I_ASSIGN(m_binarizationParamsIdAttrPtr, "BinarizationParameterId", "ID of binarization parameters in the parameter set", false, "BinarizationParameterId");
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

private:
	bool ConvertImage(
				const iimg::IBitmap& inputBitmap,
				const imeas::INumericParams& thresholdParams,
				iimg::IBitmap& outputBitmap) const;

private:
	I_ATTR(QByteArray, m_binarizationParamsIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CImageBinarizeProcessorComp_included

