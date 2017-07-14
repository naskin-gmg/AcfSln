#ifndef ibarcode_IBarcode_included
#define ibarcode_IBarcode_included


// ACF includes
#include <iser/ISerializable.h>


namespace ibarcode
{


/**
	Interface for access the barcode data.
*/
class IBarcode: virtual public iser::ISerializable
{
public:
	/**
		Supported barcode types.
	*/
	enum BarcodeType
	{
		BT_UNKNOWN,
		BT_CODABAR,
		BT_EAN_8,
		BT_EAN_13,
		BT_CODE_128,
		BT_CODE_39,
		BT_CODE_93,
		BT_INTERLEAVED_2_5,
		BT_PDF_417,
		BT_DATAMATRIX,
		BT_QR
	};

	I_DECLARE_ENUM(BarcodeType, BT_UNKNOWN, BT_CODABAR, BT_EAN_8, BT_EAN_13, BT_CODE_128, BT_CODE_39, BT_CODE_93, BT_INTERLEAVED_2_5, BT_PDF_417, BT_DATAMATRIX, BT_QR);

	/**
		Get the data of the barcode.
	*/
	virtual const QString& GetBarcodeData() const = 0;

	/**
		Get the type of the barcode.
		\sa BarcodeType
	*/
	virtual int GetBarcodeType() const = 0;
};


} // namespace ibarcode


#endif // !ibarcode_IBarcode_included


