#ifndef IprPck_included
#define IprPck_included


// ACF includes
#include "imod/TModelWrap.h"
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

// ACF-Solutions includes
#include "iipr/CRectImageSmoothProcessorComp.h"
#include "iipr/CProcessedAcquisitionComp.h"
#include "iipr/TUnaryTransformProcessorComp.h"
#include "iipr/CCaliperParamsComp.h"
#include "iipr/CExtremumCaliperProcessorComp.h"
#include "iipr/CLineProjectionProcessorComp.h"
#include "iipr/CCircleFindProcessorComp.h"
#include "iipr/CDelegatedBitmapSupplierComp.h"
#include "iipr/CProcessedBitmapSupplierComp.h"
#include "iipr/CPositionFromImageSupplierComp.h"
#include "iipr/CRectDerivativeProcessorComp.h"
#include "iipr/CFeaturesContainer.h"
#include "iipr/CSingleFeatureConsumer.h"
#include "iipr/CFeatureToImageMapperProcessorComp.h"
#include "iipr/CSearchBasedFeaturesSupplierComp.h"
#include "iipr/CLineProjectionSupplierComp.h"
#include "iipr/CImageHistogramProcessorComp.h"
#include "iipr/CCircleFinderParamsComp.h"
#include "iipr/CImagePolarTransformProcessorComp.h"
#include "iipr/CImageCropProcessorComp.h"
#include "iipr/CImageInterpolationParams.h"
#include "iipr/CRingPatternGeneratorComp.h"
#include "iipr/CRingPatternGeneratorParamsComp.h"
#include "iipr/CRgbToGrayProcessorComp.h"
#include "iipr/CImageBinarizeProcessorComp.h"
#include "iipr/CBitmapProviderCacheComp.h"
#include "iipr/CSearchParams.h"
#include "iipr/CDifferenceBitmapSupplierComp.h"
#include "iipr/CConvolutionProcessorComp.h"
#include "iipr/CConvolutionKernel2d.h"


/**
	System-undependent image processing package.
*/
namespace IprPck
{


typedef iipr::CProcessedAcquisitionComp ProcessedCamera;
typedef iipr::CRectImageSmoothProcessorComp RectImageSmoothProcessor;
typedef iipr::CInvertOperatorComp InvertOperator;
typedef icomp::TModelCompWrap<iipr::CCaliperParamsComp> CaliperParams;
typedef icomp::TModelCompWrap<iipr::CCircleFinderParamsComp> CircleFinderParams;
typedef iipr::CExtremumCaliperProcessorComp ExtremumCaliperProcessor;
typedef iipr::CLineProjectionProcessorComp LineProjectionProcessor;
typedef iipr::CCircleFindProcessorComp CircleFindProcessor;
typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<iipr::CFeaturesContainer>,
			iipr::IFeaturesConsumer,
			imeas::INumericValueProvider,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> FeaturesContainer;
typedef iipr::CFeatureToImageMapperProcessorComp FeatureToImageMapperProcessor;
typedef icomp::TModelCompWrap<iipr::CDelegatedBitmapSupplierComp> DelegatedBitmapSupplier;
typedef icomp::TModelCompWrap<iipr::CProcessedBitmapSupplierComp> ProcessedBitmapSupplier;
typedef icomp::TModelCompWrap<iipr::CPositionFromImageSupplierComp> PositionFromImageSupplier;
typedef icomp::TModelCompWrap<iipr::CSearchBasedFeaturesSupplierComp> SearchBasedFeaturesSupplier;
typedef icomp::TModelCompWrap<iipr::CLineProjectionSupplierComp> ProjectionSupplier;

typedef iipr::CRectDerivativeProcessorComp RectDerivativeProcessor;
typedef iipr::CImageHistogramProcessorComp ImageHistogramProcessor;
typedef iipr::CImagePolarTransformProcessorComp ImagePolarTransformProcessor;
typedef iipr::CImageCropProcessorComp ImageCropProcessor;
typedef iipr::CRingPatternGeneratorComp RingPatternGenerator;

typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<iipr::CImageInterpolationParams>,
			iipr::IImageInterpolationParams,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> ImageInterpolationParams;

typedef icomp::TModelCompWrap<iipr::CRingPatternGeneratorParamsComp> RingPatternGeneratorParams;
typedef iipr::CRgbToGrayProcessorComp RgbToGrayProcessor;
typedef iipr::CImageBinarizeProcessorComp ImageBinarizeProcessor;
typedef icomp::TModelCompWrap<iipr::CBitmapProviderCacheComp> BitmapProviderCache;
typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<iipr::CSearchParams>,
			iipr::ISearchParams,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> SearchParams;

typedef icomp::TModelCompWrap<iipr::CDifferenceBitmapSupplierComp> DifferenceBitmapSupplier;
typedef iipr::CConvolutionProcessorComp ConvolutionProcessor;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iipr::CConvolutionKernel2d,
						iipr::IConvolutionKernel2d,
						iser::ISerializable,
						imath::ISampledFunction2d> > ConvolutionKernel2d;

} // namespace IprPck


#endif // !IprPck_included


