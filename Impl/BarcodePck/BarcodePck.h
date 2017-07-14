#ifndef BarcodePck_included
#define BarcodePck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

// ACF-Solutions includes
#include <ibarcode/CGeneralBarcodeSearchParams.h>


/**
	Package for barcode processing components.
*/
namespace BarcodePck
{


typedef icomp::TModelCompWrap<icomp::TMakeComponentWrap<
			ibarcode::CGeneralBarcodeSearchParams,
			ibarcode::IGeneralBarcodeSearchParams,
			iser::ISerializable> > GeneralBarcodeSearchParams;


} // namespace BarcodePck


#endif // !BarcodePck_included


