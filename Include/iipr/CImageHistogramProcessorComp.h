#ifndef iipr_CImageHistogramProcessorComp_included
#define iipr_CImageHistogramProcessorComp_included


// ACF includes
#include "i2d/CRectangle.h"
#include "iimg/IBitmap.h"
#include "iproc/TSyncProcessorCompBase.h"

// ACF-Solutions includes
#include "imeas/IDiscreteDataSequence.h"


namespace iipr
{


/**	
	Implementation of a processor for the image histogram calculation.
*/
class CImageHistogramProcessorComp: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CImageHistogramProcessorComp);
		I_ASSIGN(m_aoiParamIdAttrPtr, "AoiParamId", "ID of the AOI parameter in the parameter set", false, "AoiParams");
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

private:
	bool CalculateHistogramFromBitmap(
					const iimg::IBitmap& input,
					const i2d::IObject2d* aoiPtr,
					imeas::IDiscreteDataSequence& histogram) const;

private:
	I_ATTR(std::string, m_aoiParamIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CImageHistogramProcessorComp_included

