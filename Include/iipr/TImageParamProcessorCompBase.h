#ifndef iipr_TImageParamProcessorCompBase_included
#define iipr_TImageParamProcessorCompBase_included


// ACF includes
#include "iprm/TParamsPtr.h"

// ACF-Solutions includes
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
		\param	paramsPtr			processor parameter set, it can be NULL.
		\param	procParamPtr		extracted parameter set, specific for processor type, it can be NULL.
		\param	outputPixelFormat	pixel format requested for output image, if it is \c iimg::IBitmap::PF_UNKNOWN than automatic selection should be done.
		\param	inputImage			input image should be processed.
		\param	outputImage			output image, where processing results will be stored.
	*/
	virtual bool ParamProcessImage(
				const iprm::IParamsSet* paramsPtr,
				const ParameterType* procParamPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) = 0;

private:
	I_ATTR(QByteArray, m_paramsIdAttrPtr);
};


// public methods

// reimplemented (iipr::CImageProcessorCompBase)

template <class ParameterType>
bool TImageParamProcessorCompBase<ParameterType>::ProcessImage(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	iprm::TParamsPtr<ParameterType> processorParamsPtr;	
	if ((paramsPtr != NULL) && m_paramsIdAttrPtr.IsValid()){
		processorParamsPtr.Init(paramsPtr, *m_paramsIdAttrPtr);
	}
	
	// do image processing:
	return ParamProcessImage(paramsPtr, processorParamsPtr.GetPtr(), inputImage, outputImage);
}


} // namespace iipr


#endif // !iipr_TImageParamProcessorCompBase_included

