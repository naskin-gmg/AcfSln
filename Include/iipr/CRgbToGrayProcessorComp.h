#ifndef iipr_CRgbToGrayProcessorComp_included
#define iipr_CRgbToGrayProcessorComp_included


// ACF includes
#include <iimg/IBitmap.h>
#include <iipr/CImageProcessorCompBase.h>


namespace iipr
{


/**
	Implementation of a processor for the RGB to Grayscale image conversion.
*/
class CRgbToGrayProcessorComp: public iipr::CImageProcessorCompBase
{
public:
	typedef iipr::CImageProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CRgbToGrayProcessorComp);
		I_ASSIGN(m_channelWeightsParamsIdAttrPtr, "ChannelWeightsParamId", "ID of the color channel weight parameter in the parameter set", true, "ChannelWeights");
	I_END_COMPONENT;

protected:
	// reimplemented (iipr::CImageProcessorCompBase)
	virtual bool ProcessImage(
				const iprm::IParamsSet* paramsPtr, 
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) const override;

private:
	I_ATTR(QByteArray, m_channelWeightsParamsIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CRgbToGrayProcessorComp_included

