#ifndef ifileproc_CBitmapPreviewGeneratorComp_included
#define ifileproc_CBitmapPreviewGeneratorComp_included


// ACF includes
#include <istd/TSmartPtr.h>
#include <iproc/TSyncProcessorCompBase.h>
#include <iimg/IBitmap.h>


namespace ifileproc
{


/**
	Generator of the bitmap preview.
*/
class CBitmapPreviewGeneratorComp: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;

	I_BEGIN_COMPONENT(CBitmapPreviewGeneratorComp);
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);
};


} // namespace ifileproc


#endif // !ifileproc_CBitmapPreviewGeneratorComp_included


