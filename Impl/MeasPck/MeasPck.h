#ifndef MeasPck_included
#define MeasPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"
#include "iimg/CGeneralBitmap.h"

// ACF-Solutions includes
#include "imeas/TDiscrSamplesSequence.h"
#include "imeas/CGeneralSamplesSequence.h"
#include "imeas/CWavSamplesLoaderComp.h"


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
			imeas::CGeneralSamplesSequence,
			imeas::ISamplesSequence,
			iser::ISerializable,
			istd::IChangeable> > GeneralSamplesSequence;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CSimpleSamplesSequence8,
			imeas::IDiscrSamplesSequence,
			imeas::ISamplesSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence8;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CSimpleSamplesSequence16,
			imeas::IDiscrSamplesSequence,
			imeas::ISamplesSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence16;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CSimpleSamplesSequence32,
			imeas::IDiscrSamplesSequence,
			imeas::ISamplesSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence32;
typedef imeas::CWavSamplesLoaderComp WavSamplesLoader;


} // namespace MeasPck


#endif // !MeasPck_included


