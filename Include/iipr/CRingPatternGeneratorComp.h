#ifndef iipr_CRingPatternGeneratorComp_included
#define iipr_CRingPatternGeneratorComp_included


// ACF includes
#include "iimg/IBitmap.h"

#include "iproc/TSyncProcessorCompBase.h"


namespace iipr
{


class CRingPatternGeneratorComp: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CRingPatternGeneratorComp);
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of the processor parameter in the parameter set", false, "ParamsId");
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

private:
	I_ATTR(istd::CString, m_paramsIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CRingPatternGeneratorComp_included

