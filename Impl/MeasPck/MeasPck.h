#ifndef MeasPck_included
#define MeasPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>
#include <iimg/CGeneralBitmap.h>

// ACF-Solutions includes
#include <imeas/TDiscreteDataSequence.h>
#include <imeas/CGeneralDataSequence.h>
#include <imeas/CWavSamplesLoaderComp.h>
#include <imeas/CSimpleDataStatistics.h>
#include <imeas/CDataSequenceStatistics.h>
#include <imeas/CHistogramStatisticsProcessorComp.h>
#include <imeas/CNumericParamsComp.h>
#include <imeas/CNumericConstraintsComp.h>
#include <imeas/CLinearAdjustParamsComp.h>
#include <imeas/CCrossCorrelationProcessorComp.h>
#include <imeas/CRelativeUnitSelectorComp.h>


/**
	System-independent package for sampling, measurement acquisition and related themas.
*/
namespace MeasPck
{


typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			iimg::CGeneralBitmap,
			iimg::IBitmap,
			iimg::IRasterImage,
			iser::ISerializable,
			istd::IChangeable> > GeneralBitmap;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CGeneralDataSequence,
			imeas::IDataSequence,
			imath::ISampledFunction2d,
			iser::ISerializable,
			istd::IChangeable> > GeneralDataSequence;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CSimpleSamplesSequence8,
			imeas::IDiscreteDataSequence,
			imeas::IDataSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence8;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CSimpleSamplesSequence16,
			imeas::IDiscreteDataSequence,
			imeas::IDataSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence16;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CSimpleSamplesSequence32,
			imeas::IDiscreteDataSequence,
			imeas::IDataSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence32;
typedef imeas::CWavSamplesLoaderComp WavSamplesLoader;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CSimpleDataStatistics,
			imeas::IDataStatistics,
			iser::ISerializable> > SimpleDataStatistics;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CDataSequenceStatistics,
			imeas::IDataSequenceStatistics,
			iser::ISerializable> > DataSequenceStatistics;
typedef imeas::CHistogramStatisticsProcessorComp HistogramStatisticsProcessor;
typedef icomp::TModelCompWrap<imeas::CNumericParamsComp> NumericParams;
typedef imeas::CNumericConstraintsComp NumericConstraints;
typedef icomp::TModelCompWrap<imeas::CLinearAdjustParamsComp> LinearAdjustParams;
typedef imeas::CCrossCorrelationProcessorComp CrossCorrelationProcessor;
typedef icomp::TModelCompWrap<imeas::CRelativeUnitSelectorComp> RelativeUnitSelector;


} // namespace MeasPck


#endif // !MeasPck_included


