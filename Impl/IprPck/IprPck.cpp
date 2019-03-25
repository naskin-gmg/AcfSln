#include "IprPck.h"


#include <icomp/export.h>


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
			ExtremumCaliperSupplier,
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
			ProcessedQtBitmapSupplier,
			"Implementation of Qt-bitmap based processed bitmap supplier",
			"Image Standard Bitmap Processing Supplier");

I_EXPORT_COMPONENT(
			PositionFromImageSupplier,
			"Supplier implementation returning position found using caliper tool",
			"Model Caliper Position Processing Supplier" IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			SearchBasedFeaturesSupplier,
			"Supplier implementation returning set of geometrical features found by pattern search",
			"Model Pattern Search Processing Supplier" IM_AUTHOR("\"Kirill Lepskiy\""));

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
			ImageCropDecalibrateProcessor,
			"Image crop processor providing rectification",
			"Crop Region Processor Transform AOI ROI Decalibration Calibration Rectification Equalization" IM_AUTHOR("\"Witold Gantzke\""));

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
			GrayToRgbProcessor,
			"Grayscale to RGB image conversion",
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
			MultiBitmapProviderCacheComp,
			"Multi bitmap provider cache object used to manage threading barrier for object supplier chain",
			"Provider Cache Bitmap Multi" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Inspection Model") IM_AUTHOR("\"Witold Gantzke\""));

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

I_EXPORT_COMPONENT(
			BitmapJoinerSupplier,
			"Bitmap joiner from a multi-bitmap-provider",
			"Bitmap Supplier Provider Join" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			EdgeDistanceProcessor,
			"Processor for calculation of distance between two edges",
			"Bitmap Edge Caliper Distance" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			EdgeDistancesSupplier,
			"Supplier of found distances between two edges based on caliper calculation",
			"Bitmap Edge Caliper Distance" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			TubeProjectionLinesProvider,
			"Supplier list of projection lines in a tube region",
			"Bitmap Projection Line Tube Region" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			MorphologicalProcessor,
			"Processor for simple morphological operations on a bitmap",
			"Bitmap Morphology Erosion Dilatation Opening Closing Structure" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			MultiLineProjectionSupplier, 
			"Multi line projection supplier",
			"Multi Line Projection Supplier")

I_EXPORT_COMPONENT(
			SelectableCalibrationProvider, 
			"Provider of a single (selected) calibration from a list of calibrations",
			"Multi Single Calibration Provider" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			CheckerboardPatternGenerator, 
			"Generator of checker board pattern bitmap",
			"Generator Bitmap Processor" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ImageFlipProcessor,
			"Image flipping processor (vertical/horizontal)",
			"Flip Mirror Image" IM_AUTHOR("\"Arseniy Masyuk\""));

I_EXPORT_COMPONENT(
			ColorPatternController,
			"Pattern controller for color matching",
			"Pattern Color Matching" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ColorPatternComparator,
			"Pattern controller for color matching",
			"Pattern Color Matching Compare" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ImageHistogramSupplier,
			"Image histogram supplier",
			"Image Bitmap Histogram Statistics" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			FastGaussianProcessor,
			"Fast Gaussian processor with statically defined kernel",
			"Filter Smoothing Convolution Gaussian Blur" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ImageNormalizeProcessor,
			"Image adjustment with optional histogram based automatic image normalization",
			"Image Adjustment Histogram Normalization Brightness Contrast Enhancing Equalization Linear Scale" IM_AUTHOR("\"Kirill Lepskiy\" \"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			ProjectionSupplier,
			"Supplier implementation returning image projection data",
		 	"Supplier Projection Data Image Bitmap Model" IM_TAG("2D"));

I_EXPORT_COMPONENT(
			AdaptiveImageBinarizeProcessor,
			"Adapative image binarization",
		 	"Image Bitmap Binarization Threshold Adaptive");

I_EXPORT_COMPONENT(
			HoughLineFinder,
			"Find line on the image using Hough transformation",
			"Image Bitmap Line Finder Straight Hough Transformation Detection" IM_TAG("2D") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			HoughLineFromPosFinder,
			"Find lines in point cloud using Hough transformation",
			"Features Points Cloud Line Finder Straight Hough Transformation Detection" IM_TAG("2D") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			CheckboardCalibSupplier,
			"Calculate calibration based on checkboard pattern",
			"Checkboard Chessboard Pattern Calibrator Calibration Perspective" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Supplier") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			MultiPointPosCorrSupplier,
			"Calculate position correction using list of position finders",
			"Position Positioning Reference Calibration Correction Translation Rotation" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Supplier") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			LensCorrFindSupplier,
			"Calculate lens correction using set of found feature points lying on lines",
			"Objective Lens Correction Finder Calculator Computation Factor Calibration Correction Optical Axis" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Supplier") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			ImageIntegralProcessor,
			"Processor calculation integral image of some input image",
			"Integral Derivative Image Processor" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processor") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			CheckerboardPointGridExtractor,
			"Extractor of grid points in a checkerboard image",
			"Calibration Checkerboard Grid Processor" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processor"));

I_EXPORT_COMPONENT(
			PerspectiveCalibrationSupplier,
			"Supplier of the perspective calibration based on some point grid",
			"Calibration Perspective Supplier Provider" IM_CATEGORY(I_DATA_PROCESSING));


} // namespace IprPck


