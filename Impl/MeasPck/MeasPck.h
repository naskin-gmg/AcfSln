#ifndef MeasPck_included
#define MeasPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"
#include "iimg/CGeneralBitmap.h"

// ACF-Solutions includes
#include "imeas/TDiscrDataSequence.h"
#include "imeas/CGeneralDataSequence.h"
#include "imeas/CWavSamplesLoaderComp.h"
#include "imeas/CDataStatistics.h"
#include "imeas/CDataSequenceStatistics.h"
#include "imeas/CHistogramStatisticsProcessorComp.h"


/**
	System-undependent package for sampling, measurement acquisition and related thema.
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
			imeas::IDiscrDataSequence,
			imeas::IDataSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence8;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CSimpleSamplesSequence16,
			imeas::IDiscrDataSequence,
			imeas::IDataSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence16;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CSimpleSamplesSequence32,
			imeas::IDiscrDataSequence,
			imeas::IDataSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence32;
typedef imeas::CWavSamplesLoaderComp WavSamplesLoader;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CDataStatistics,
			imeas::IDataStatistics,
			iser::ISerializable> > DataStatistics;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CDataSequenceStatistics,
			imeas::IDataSequenceStatistics,
			iser::ISerializable> > DataSequenceStatistics;
typedef imeas::CHistogramStatisticsProcessorComp HistogramStatisticsProcessor;


} // namespace MeasPck


#endif // !MeasPck_included


