#ifndef iblob_CPreciseBlobProcessorComp_included
#define iblob_CPreciseBlobProcessorComp_included


// ACF includes
#include <iimg/CScanlineMask.h>

// ACF-Solutions includes
#include <imeas/INumericValue.h>
#include <imeas/CGeneralNumericConstraints.h>
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
		I_REGISTER_SUBELEMENT(ThresholdContraints);
		I_REGISTER_SUBELEMENT_INTERFACE(ThresholdContraints, imeas::INumericConstraints, ExtractThresholdContraints);
		I_REGISTER_SUBELEMENT_INTERFACE(ThresholdContraints, istd::IChangeable, ExtractThresholdContraints);
		I_ASSIGN(m_aoiParamIdAttrPtr, "AoiParamId", "ID of area of interest in parameter set", false, "AoiParams");
		I_ASSIGN(m_defaultAoiCompPtr, "DefaultAoi", "Area of interest used if not specified in parameters", false, "DefaultAoi");
		I_ASSIGN(m_thresholdParamIdAttrPtr, "ThresholdParamId", "ID of threshold value in parameter set (imeas::INumericValue)", false, "Threshold");
		I_ASSIGN(m_defaultThresholdCompPtr, "DefaultThreshold", "Threshold used if not specified in parameters", false, "DefaultThreshold");
		I_ASSIGN(m_resultConsumerCompPtr, "ResultConsumer", "Consumer of result messages with geometrical layout", false, "ResultConsumer");
		I_ASSIGN(m_tempConsumerCompPtr, "TempConsumer", "Consumer of temporary result messages with geometrical layout", false, "TempConsumer");
	I_END_COMPONENT;

	enum MessageId
	{
		FOUND_BLOB = 0x9354f0
	};

	CPreciseBlobProcessorComp();

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
				const istd::CRange& relValueRange,
				const iblob::IBlobFilterParams* filterParamsPtr,
				const iimg::CScanlineMask& imageMask,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& result,
				ilog::IMessageConsumer* resultConsumerPtr = NULL,
				ilog::IMessageConsumer* tempMessageConsumerPtr = NULL);

protected:
	// reimplemented (iblob::CBlobProcessorCompBase)
	virtual bool CalculateBlobs(
				const iprm::IParamsSet* paramsPtr,
				const iblob::IBlobFilterParams* filterParamsPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& result);

private:
	I_ATTR(QByteArray, m_aoiParamIdAttrPtr);
	I_REF(i2d::IObject2d, m_defaultAoiCompPtr);
	I_ATTR(QByteArray, m_thresholdParamIdAttrPtr);
	I_REF(imeas::INumericValue, m_defaultThresholdCompPtr);
	I_REF(ilog::IMessageConsumer, m_resultConsumerCompPtr);
	I_REF(ilog::IMessageConsumer, m_tempConsumerCompPtr);

	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractThresholdContraints(CPreciseBlobProcessorComp& component)
	{
		return &component.m_thresholdContraints;
	}

	imeas::CGeneralNumericConstraints m_thresholdContraints;
};


} // namespace iblob


#endif // !iblob_CPreciseBlobProcessorComp_included


