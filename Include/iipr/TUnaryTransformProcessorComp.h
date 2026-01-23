#ifndef iipr_TUnaryTransformProcessorComp_included
#define iipr_TUnaryTransformProcessorComp_included


// STL includes
#include <functional>

// ACF includes
#include <iipr/CImageProcessorCompBase.h>

#ifdef max
	#undef max
#endif


namespace iipr
{
	

template <typename ArgumentType>
#if defined (_MSC_VER) && _MSC_VER < 1900
class InvertFunction: public std::unary_function<ArgumentType, ArgumentType>
#else
//Unary function is deprecated in VS2017
class InvertFunction
#endif
{
public:
	InvertFunction()
	{
		m_maxValue = std::numeric_limits<ArgumentType>::max();
	}

	inline ArgumentType operator()(const ArgumentType& value) const
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
				iimg::IBitmap& outputImage) const;

private:
	Predicate m_predicate;
};


// reimplemented (iipr::CImageProcessorCompBase)

template <typename PixelType, class Predicate> 
bool TUnaryTransformProcessorComp<PixelType, Predicate>::ProcessImage(
			const iprm::IParamsSet* /*paramsPtr*/,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage) const
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


typedef TUnaryTransformProcessorComp<quint8, InvertFunction<quint8> > CInvertOperatorComp;


} // namespace iipr


#endif // !iipr_TUnaryTransformProcessorComp_included


