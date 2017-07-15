#ifndef ibarcode_CGeneralBarcodeSearchParams_included
#define ibarcode_CGeneralBarcodeSearchParams_included


// ACF includes
#include <ibarcode/IGeneralBarcodeSearchParams.h>


namespace ibarcode
{


/**
	Interface for basic barcode search parameters.
*/
class CGeneralBarcodeSearchParams: virtual public IGeneralBarcodeSearchParams
{
public:
	// reimplemented (IGeneralBarcodeSearchParams)
	virtual BarcodeTypes GetSearchBarcodeTypes() const;
	virtual void SetSearchBarcodeTypes(const BarcodeTypes& barcodeTypes);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	BarcodeTypes m_barcodeTypes;
};


} // namespace ibarcode


#endif // !ibarcode_CGeneralBarcodeSearchParams_included


