#ifndef iipr_TUnaryTransformProcessorComp_included
#define iipr_TUnaryTransformProcessorComp_included


#include <functional>


#include "iipr/CImageProcessorCompBase.h"


namespace iipr
{
	

template <typename ArgumentType>
class InvertFunction: public std::unary_function<ArgumentType, ArgumentType>
{
public:
	InvertFunction()
	{
		m_maxValue = std::numeric_limits<ArgumentType>::max();
	}

	inline ArgumentType operator()(const ArgumentType& value)
	{
		return (m_maxValue - value);
	}

private:
	ArgumentType m_maxValue;
};


/**
	Implementation of general morphological operator.
*/
template <typename PixelType, class Predicate> 
class TUnaryTransformProcessorComp: public iipr::CImageProcessorCompBase
{
public:
	typedef iipr::CImageProcessorCompBase BaseClass;

	I_BEGIN_COMPONENT(TUnaryTransformProcessorComp);
	I_END_COMPONENT;

protected:		
	// reimplemented (iipr::CImageProcessorCompBase)
	virtual bool ProcessImage(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

private:
	Predicate m_predicate;
};


// reimplemented (iipr::CImageProcessorCompBase)

template <typename PixelType, class Predicate> 
bool TUnaryTransformProcessorComp<PixelType, Predicate>::ProcessImage(
			const iprm::IParamsSet* /*paramsPtr*/,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (!outputImage.CreateBitmap(inputImage.GetPixelFormat(), inputImage.GetImageSize())){
		SendErrorMessage(0, "Output bitmap could not be created");

		return false;	// cannot create output image
	}

	int imageWidth = inputImage.GetImageSize().GetX();
	int imageHeight = inputImage.GetImageSize().GetY();
	int componentsCount = inputImage.GetComponentsCount();

	for (int y = 0; y < imageHeight; y++){
		PixelType* inputImageLinePtr = (PixelType*)inputImage.GetLinePtr(y);
		PixelType* outputImageLinePtr = (PixelType*)outputImage.GetLinePtr(y);

		for (int x = 0; x < imageWidth; x++){
			for (int colorComponentIndex = 0; colorComponentIndex < componentsCount; colorComponentIndex++){
				int pixelOffset = x * componentsCount + colorComponentIndex;

				outputImageLinePtr[pixelOffset] = m_predicate(inputImageLinePtr[pixelOffset]);
			}
		}
	}

	return true;
}


typedef TUnaryTransformProcessorComp<I_BYTE, InvertFunction<I_BYTE> > CInvertOperatorComp;


} // namespace iipr


#endif // !iipr_TUnaryTransformProcessorComp_included


