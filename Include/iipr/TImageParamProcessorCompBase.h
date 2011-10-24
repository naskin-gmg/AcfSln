#ifndef iipr_TImageParamProcessorCompBase_included
#define iipr_TImageParamProcessorCompBase_included


// ACF includes
#include "iipr/CImageProcessorCompBase.h"


namespace iipr
{


/**
	Basic implementation for a simple bitmap processor.
*/
template <class ParameterType>
class TImageParamProcessorCompBase: public CImageProcessorCompBase
{
public:
	typedef CImageProcessorCompBase BaseClass;

	I_BEGIN_BASE_COMPONENT(TImageParamProcessorCompBase);
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of processor parameters in the parameter set", false, "ParamsId");
	I_END_COMPONENT;

protected:
	// reimplemented (iipr::CImageProcessorCompBase)
	virtual bool ProcessImage(
				const iprm::IParamsSet* paramsPtr, 
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

	// abstract methods
	/**
		Do image processing with some (optional) parameter set.
		\param	parameterPtr	extracted parameter set, it can be NULL.
		\param	inputImage		input image should be processed.
		\param	outputImage		output image, where processing results will be stored.
	*/
	virtual bool ParamProcessImage(
				const ParameterType* parameterPtr, 
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) = 0;

private:
	I_ATTR(istd::CString, m_paramsIdAttrPtr);
};


// public methods

// reimplemented (iipr::CImageProcessorCompBase)

template <class ParameterType>
bool TImageParamProcessorCompBase<ParameterType>::ProcessImage(
			const iprm::IParamsSet* paramsPtr, 
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	const ParameterType* processorParamsPtr = NULL;	
	if ((paramsPtr != NULL) && m_paramsIdAttrPtr.IsValid()){
		processorParamsPtr = dynamic_cast<const ParameterType*>(paramsPtr->GetParameter(m_paramsIdAttrPtr->GetValue().ToString()));
	}
	
	// do image processing:
	return ParamProcessImage(processorParamsPtr, inputImage, outputImage);
}


} // namespace iipr


#endif // !iipr_TImageParamProcessorCompBase_included

