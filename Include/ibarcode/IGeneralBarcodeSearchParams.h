#ifndef ibarcode_IGeneralBarcodeSearchParams_included
#define ibarcode_IGeneralBarcodeSearchParams_included


// ACF includes
#include <iser/ISerializable.h>
#include <ibarcode/IBarcode.h>


namespace ibarcode
{


/**
	Interface for basic barcode search parameters.
*/
class IGeneralBarcodeSearchParams: virtual public iser::ISerializable
{
public:
	typedef QSet<IBarcode::BarcodeType> BarcodeTypes;

	/**
		Get the list of barcode types should be searched for.
	*/
	virtual BarcodeTypes GetSearchBarcodeTypes() const = 0;

	/**
		Set the list of barcode types should be searched for.
	*/
	virtual void SetSearchBarcodeTypes(const BarcodeTypes& barcodeTypes) = 0;
};


} // namespace ibarcode


#endif // !ibarcode_IGeneralBarcodeSearchParams_included


