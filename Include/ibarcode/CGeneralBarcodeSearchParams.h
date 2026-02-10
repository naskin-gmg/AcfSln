// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
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
	virtual BarcodeTypes GetSearchBarcodeTypes() const override;
	virtual void SetSearchBarcodeTypes(const BarcodeTypes& barcodeTypes) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	BarcodeTypes m_barcodeTypes;
};


} // namespace ibarcode


#endif // !ibarcode_CGeneralBarcodeSearchParams_included


