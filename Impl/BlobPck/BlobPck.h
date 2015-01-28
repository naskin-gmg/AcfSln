#ifndef BlobPck_included
#define BlobPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

// ACF-Solutions includes
#include "iblob/CBlobFilterParams.h"

#include "iblobgui/CBlobFilterParamsGuiComp.h"


/**
	Package for blob extraction components.
*/
namespace BlobPck
{


typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iblob::CBlobFilterParams, iblob::IBlobFilterParams, iser::ISerializable, istd::IChangeable> > BlobFilterParams;

typedef iblobgui::CBlobFilterParamsGuiComp BlobFilterParamsGui;


} // namespace BlobPck


#endif // !BlobPck_included


