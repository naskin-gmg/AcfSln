// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibarcode/CBarcodeSearchFeature.h>


// ACF includes
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <istd/CChangeNotifier.h>


namespace ibarcode
{


// public methods

CBarcodeSearchFeature::CBarcodeSearchFeature(
			BarcodeType barcodeType,
			const QString& barcodeData,
			const i2d::CPolygon& barcodeArea,
			bool isChecksumValid,
			double weight,
			const i2d::CVector2d& position,
			double angle,
			const i2d::CVector2d& scale)
	:BaseClass(weight, position, angle, scale),
	BaseClass2(barcodeType, barcodeData),
	m_isCheckSumValid(isChecksumValid),
	m_barcodeArea(barcodeArea)
{
	m_validSymbolCount = barcodeData.count();
}


bool CBarcodeSearchFeature::IsBarcodeValid() const
{
	return (m_isCheckSumValid && !m_barcodeData.isEmpty());
}


void CBarcodeSearchFeature::ResetBarcode()
{
	m_validSymbolCount = 0;
	m_isCheckSumValid = false;
	m_barcodeArea = i2d::CPolygon();

	SetData(QByteArray());
}


i2d::CPolygon CBarcodeSearchFeature::GetBarcodeArea() const
{
	return m_barcodeArea;
}


void CBarcodeSearchFeature::SetBarcodeArea(const i2d::CPolygon& barcodeArea)
{
	if (!m_barcodeArea.IsEqual(barcodeArea)){
		istd::CChangeNotifier changePtr(this);

		m_barcodeArea = barcodeArea;
	}
}


bool CBarcodeSearchFeature::IsCheckSumValid() const
{
	return m_isCheckSumValid;
}


void CBarcodeSearchFeature::SetCheckSumValid(bool isCheckSumValid)
{
	if (m_isCheckSumValid != isCheckSumValid){
		istd::CChangeNotifier changePtr(this);

		m_isCheckSumValid = isCheckSumValid;

		EnsureWeightCalculated();
	}
}


int CBarcodeSearchFeature::GetValidSymbolCount() const
{
	return m_validSymbolCount;
}


void CBarcodeSearchFeature::SetValidSymbolCount(int validSymbolCount)
{
	if (m_validSymbolCount != validSymbolCount){
		istd::CChangeNotifier changePtr(this);

		m_validSymbolCount = validSymbolCount;

		EnsureWeightCalculated();
	}
}


// reimplemented (IBarcode)

void CBarcodeSearchFeature::SetData(const QByteArray& barcodeData)
{
	BaseClass2::SetData(barcodeData);

	EnsureWeightCalculated();
}


// reimplemented (imeas::INumericValue)

bool CBarcodeSearchFeature::IsValueTypeSupported(ValueTypeId valueTypeId) const
{
	switch (valueTypeId){
	case VTI_AUTO:
	case VTI_WEIGHT:
	case VTI_POSITION:
		return true;

	default:
		return false;
	}
}


imath::CVarVector CBarcodeSearchFeature::GetComponentValue(ValueTypeId valueTypeId) const
{
	imath::CVarVector retVal;

	switch (valueTypeId){
	case VTI_AUTO:
	case VTI_WEIGHT:
		return BaseClass::GetComponentValue(valueTypeId);

	case VTI_POSITION:
		retVal = GetPosition();
		break;

	default:
		break;
	}

	return retVal;
}


// reimplemented (iser::ISerializable)

bool CBarcodeSearchFeature::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag barcodeTag("Barcode", "Barcode");
	retVal = retVal && archive.BeginTag(barcodeTag);
	retVal = retVal && BaseClass2::Serialize(archive);
	retVal = retVal && archive.EndTag(barcodeTag);

	static iser::CArchiveTag isCheckSumValidTag("CheckSum", "CheckSum");
	retVal = retVal && archive.BeginTag(isCheckSumValidTag);
	retVal = retVal && archive.Process(m_isCheckSumValid);
	retVal = retVal && archive.EndTag(isCheckSumValidTag);

	static iser::CArchiveTag validSymbolCountTag("ValidSymbolCount", "Number of valid symbols in the barcode data");
	retVal = retVal && archive.BeginTag(validSymbolCountTag);
	retVal = retVal && archive.Process(m_validSymbolCount);
	retVal = retVal && archive.EndTag(validSymbolCountTag);

	static iser::CArchiveTag barcodeAreaTag("BarcodeArea", "Region in the image where the barcode was found");
	retVal = retVal && archive.BeginTag(barcodeAreaTag);
	retVal = retVal && m_barcodeArea.Serialize(archive);
	retVal = retVal && archive.EndTag(barcodeAreaTag);

	if (!archive.IsStoring()){
		EnsureWeightCalculated();
	}

	return retVal;
}


// private methods

void CBarcodeSearchFeature::EnsureWeightCalculated()
{
	if (m_barcodeData.isEmpty() || (m_barcodeType == BT_UNKNOWN)){
		SetWeight(0);
	}
	else{
		int barcodeSymbols = m_barcodeData.count();

		SetWeight(0.5 * (m_validSymbolCount / double(barcodeSymbols) + (m_isCheckSumValid ? 1 : 0)));
	}
}


} // namespace ibarcode


