#ifndef ibarcode_CBarcodeSearchFeature_included
#define ibarcode_CBarcodeSearchFeature_included


// ACF includes
#include <imath/CVarVector.h>
#include <i2d/CPolygon.h>
#include <iipr/TWeightedFeatureWrap.h>
#include <ibarcode/CBarcode.h>


namespace ibarcode
{


/**
	Implementation of the barcode feature. This is a output of a barcode search processor.
*/
class CBarcodeSearchFeature: public iipr::TWeightedFeatureWrap<CBarcode>
{
public:
	typedef iipr::TWeightedFeatureWrap<CBarcode> BaseClass;
	typedef BaseClass2 BarcodeImpl;

	CBarcodeSearchFeature(
				ibarcode::IBarcode::BarcodeType barcodeType = BT_UNKNOWN,
				double weight = 0.0,
				const QString& barcodeData = QString(),
				const i2d::CPolygon& barcodeArea = i2d::CPolygon(),
				bool isChecksumValid = false);

	virtual bool IsBarcodeValid() const;

	virtual void ResetBarcode();

	virtual i2d::CPolygon GetBarcodeArea() const;
	virtual void SetBarcodeArea(const i2d::CPolygon& barcodeArea);

	virtual bool IsCheckSumValid() const;
	virtual void SetCheckSumValid(bool isCheckSumValid = true);

	virtual int GetValidSymbolCount() const;
	virtual void SetValidSymbolCount(int validSymbolCount);

	virtual imath::CVarVector GetPosition() const;

	// reimplemented (IBarcode)
	virtual void SetData(const QByteArray& barcodeData);

	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const;
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	void EnsureWeightCalculated();

private:
	bool m_isCheckSumValid;
	int m_validSymbolCount;
	i2d::CPolygon m_barcodeArea;
};


} // namespace ibarcode


#endif // !ibarcode_CBarcodeSearchFeature_included


