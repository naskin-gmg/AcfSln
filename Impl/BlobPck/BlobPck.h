#ifndef BlobPck_included
#define BlobPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

// ACF-Solutions includes
#include <iblob/CBlobFilterParamsComp.h>
#include <iblob/CPreciseBlobProcessorComp.h>

#include <iblobgui/CBlobFilterParamsGuiComp.h>


/**
	Package for blob extraction components.
*/
namespace BlobPck
{


typedef icomp::TModelCompWrap<iblob::CBlobFilterParamsComp> BlobFilterParams;
typedef iblob::CPreciseBlobProcessorComp PreciseBlobProcessor;

typedef iblobgui::CBlobFilterParamsGuiComp BlobFilterParamsGui;


} // namespace BlobPck


#endif // !BlobPck_included


