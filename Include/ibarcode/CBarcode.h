#ifndef ibarcode_CBarcode_included
#define ibarcode_CBarcode_included


// ACF includes
#include <ibarcode/IBarcode.h>


namespace ibarcode
{


/**
	Implementation of the simple barcode data container.
*/
class CBarcode: virtual public IBarcode
{
public:
	CBarcode(int barcodeType = BT_UNKNOWN);
	CBarcode(const QString& data, int barcodeType);
	CBarcode(const ibarcode::CBarcode& barcode);

	void Initialize(const QString& data, int barcodeType);

	virtual CBarcode& operator=(const CBarcode& barcode);
	virtual bool operator==(const CBarcode& barcode) const;
	virtual bool operator!=(const CBarcode& barcode) const;

	virtual void SetData(const QString& barcodeData);

	// reimplemented (ibarcode::IBarcode)
	virtual const QString& GetBarcodeData() const;
	virtual int GetBarcodeType() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	QString m_barcodeData;
	int m_barcodeType;
};


} // namespace ibarcode


#endif // !ibarcode_CBarcode_included


