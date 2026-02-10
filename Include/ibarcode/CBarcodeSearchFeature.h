// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef ibarcode_CBarcodeSearchFeature_included
#define ibarcode_CBarcodeSearchFeature_included


// ACF includes
#include <imath/CVarVector.h>
#include <i2d/CPolygon.h>
#include <iipr/CObjectFeature.h>
#include <ibarcode/CBarcode.h>


namespace ibarcode
{


/**
	Implementation of the barcode feature. This is a output of a barcode search processor.
*/
class CBarcodeSearchFeature: public iipr::CObjectFeature, public CBarcode
{
public:
	typedef iipr::CObjectFeature BaseClass;
	typedef CBarcode BaseClass2;

	CBarcodeSearchFeature(
				ibarcode::IBarcode::BarcodeType barcodeType = BT_UNKNOWN,
				const QString& barcodeData = QString(),
				const i2d::CPolygon& barcodeArea = i2d::CPolygon(),
				bool isChecksumValid = false,
				double weight = 0.0,
				const i2d::CVector2d& position = i2d::CVector2d(0, 0),
				double angle = 0.0,
				const i2d::CVector2d& scale = i2d::CVector2d(1.0, 1.0));

	virtual bool IsBarcodeValid() const;

	virtual void ResetBarcode();

	virtual i2d::CPolygon GetBarcodeArea() const;
	virtual void SetBarcodeArea(const i2d::CPolygon& barcodeArea);

	virtual bool IsCheckSumValid() const;
	virtual void SetCheckSumValid(bool isCheckSumValid = true);

	virtual int GetValidSymbolCount() const;
	virtual void SetValidSymbolCount(int validSymbolCount);

	// reimplemented (IBarcode)
	virtual void SetData(const QByteArray& barcodeData);

	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const override;
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	void EnsureWeightCalculated();

private:
	bool m_isCheckSumValid;
	int m_validSymbolCount;
	i2d::CPolygon m_barcodeArea;
};


} // namespace ibarcode


#endif // !ibarcode_CBarcodeSearchFeature_included


