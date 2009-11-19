#ifndef MeasPck_included
#define MeasPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iimg/CGeneralBitmap.h"
#include "imeas/TDiscrSamplesSequence.h"


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
			imeas::CSimpleSamplesSequence8,
			imeas::IDiscrSamplesSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence8;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CSimpleSamplesSequence16,
			imeas::IDiscrSamplesSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence16;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			imeas::CSimpleSamplesSequence32,
			imeas::IDiscrSamplesSequence,
			iser::ISerializable,
			istd::IChangeable> > SimpleSamplesSequence32;


} // namespace MeasPck


#endif // !MeasPck_included


