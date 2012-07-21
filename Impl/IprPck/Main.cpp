#include "IprPck.h"


#include "icomp/export.h"


namespace IprPck
{


I_EXPORT_PACKAGE(
			"AcfSln/IprPck",
			"Image processing package",
			"\"Image Processing\"" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools"));

I_EXPORT_COMPONENT(
			RectImageSmoothProcessor,
			"Fast smooth image processor using simple rectangle kernel",
			"Image Processing Rectangle Kernel" IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ProcessedCamera,
			"Processed Camera",
			"Image Bitmap Acquisition Snap Parameters Camera Processing" IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			InvertOperator,
			"Invertation operator",
			"Operator Image Processing" IM_TAG("Processor") IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			CaliperParams,
			"Parameters for caliper tool",
			"Caliper Weight Direction Gradients Projection Parameters" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			ExtremumCaliperProcessor,
			"Caliper implementation extracting projection extremums",
			"Caliper Extremum Minimum Maximum Projection Processor" IM_TAG("Processor") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			LineProjectionProcessor,
			"Line projection implementation for single line width",
			"Projection Line Processor" IM_TAG("Processor") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			CircleFindProcessor,
			"Circle finder processor",
			"Circle Finder Annulus Caliper Processor" IM_TAG("Processor") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			FeaturesContainer,
			"Container of feature set used to store processing results of some processors",
			"Model Caliper Search Feature Extraction Container" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			FeatureToImageMapperProcessor,
			"Processor converting feature container to set of positions using mapper of features",
			"Feature Position Mapper Caliper Convertion" IM_TAG("Processor") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			DelegatedBitmapSupplier,
			"Image supplier redirecting the image access to another supplier or image object",
			"Supplier Image Bitmap Processing Identity Bridge Direct Identical Dummy Wrap Copy" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			ProcessedBitmapSupplier,
			"Image supplier providing processed image from another one",
			"Supplier Image Bitmap Processing Processor" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			PositionFromImageSupplier,
			"Supplier implementation returning position found using caliper tool",
			"Model Caliper Position Processing Supplier" IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			SearchBasedFeaturesSupplier,
			"Supplier implementation returning set of geometrical features found by pattern search",
			"Model Pattern Search Processing Supplier" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ProjectionSupplier,
			"Supplier implementation returning image projection data",
			"Supplier Projection Data Image Bitmap Model" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			RectDerivativeProcessor,
			"Calculate derivative of projection using rectangular kernel",
			"Projection Derivative Processor Filter Rectangular Kernel" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			ImageHistogramProcessor,
			"Calculate image histogram",
			"Processor Histogram" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			CircleFinderParams,
			"Circle finder parameters",
			"Circle Finder Processor Parameters" IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ImagePolarTransformProcessor,
			"Polar transformation",
			"Processor Polar Transform" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ImageCropProcessor,
			"Image crop processor",
			"Crop Region Processor Transform AOI ROI" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			RingPatternGenerator,
			"Generator of the ring pattern image",
			"Generator Pattern" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ImageInterpolationParams,
			"Parameter for image interpolation",
			"Parameter Interpolation" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			RingPatternGeneratorParams,
			"Parameter for the ring pattern generation",
			"Parameter Ring Generator Test" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			RgbToGrayProcessor,
			"RGB to Grayscale image conversion",
			"RGB Grayscale Image Colorspace" IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ImageBinarizeProcessor,
			"Image binarization processor with static threshold",
			"Binarization Grayscale Image" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			BitmapProviderCache,
			"Bitmap provider cache object used to manage threading barrier for object supplier chain",
			"Provider Cache Bitmap" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Inspection Model") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			SearchParams,
			"General parameter for a pattern search",
			"Pattern Search Model Geometric" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			DifferenceBitmapSupplier,
			"Bitmap supplier based on difference of two other bitmap suppliers",
			"Bitmap Difference Supplier Provider" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ConvolutionProcessor,
			"General convolution processor",
			"Convolution Processor Filter" IM_AUTHOR("\"Kirill Lepskiy\"" "\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			ConvolutionKernel2d,
			"2D-convolution kernel",
			"Convolution Kerlnel Filter" IM_AUTHOR("\"Kirill Lepskiy\"" "\"Witold Gantzke\""));


} // namespace IprPck


