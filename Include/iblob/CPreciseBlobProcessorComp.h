#ifndef iblob_CPreciseBlobProcessorComp_included
#define iblob_CPreciseBlobProcessorComp_included


// ACF includes
#include <iimg/CScanlineMask.h>

// ACF-Solutions includes
#include <iblob/CBlobProcessorCompBase.h>


namespace iblob
{


/**
	Implementation of image processor using convolution kernel.
*/
class CPreciseBlobProcessorComp: public CBlobProcessorCompBase
{
public:
	typedef CBlobProcessorCompBase BaseClass;

	I_BEGIN_COMPONENT(CPreciseBlobProcessorComp);
		I_ASSIGN(m_aoiParamIdAttrPtr, "AoiParamId", "ID of area of interest in parameter set", false, "AoiParams");
		I_ASSIGN(m_defaultAoiCompPtr, "DefaultAoi", "Area of interest used if not specified in parameters", false, "DefaultAoi");
	I_END_COMPONENT;

	// static methods
	/**
		Calculate complete convolution with all parameters as static function.
		\param	filterParamsPtr		optional filter of considered blobs. If \c NULL no filter will be used.
		\param	imageMask			AOI mask where the image pixels should be used.
		\param	image				input image.
		\param	result				consumer of result blob features.
		\param	loggerPtr			optional object collecting processing messages.
	*/
	static bool DoCalculateBlobs(
				const iblob::IBlobFilterParams* filterParamsPtr,
				const iimg::CScanlineMask& imageMask,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& result,
				istd::ILogger* loggerPtr = NULL);

protected:
	// reimplemented (iblob::CBlobProcessorCompBase)
	virtual bool CalculateBlobs(
				const iprm::IParamsSet* paramsPtr,
				const iblob::IBlobFilterParams* filterParamsPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& result);
	static bool IsValueAcceptedByFilter(const iblob::IBlobFilterParams::Filter& filter, double value);

private:
	I_ATTR(QByteArray, m_aoiParamIdAttrPtr);
	I_REF(i2d::IObject2d, m_defaultAoiCompPtr);
};


} // namespace iblob


#endif // !iblob_CPreciseBlobProcessorComp_included


