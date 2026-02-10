// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibarcode/CBarcode.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>



namespace ibarcode
{


// public methods

CBarcode::CBarcode(int barcodeType, const QString& barcodeData)
	:m_barcodeData(barcodeData),
	m_barcodeType(barcodeType)
{
}


CBarcode::CBarcode(const ibarcode::CBarcode& barcode)
{
	m_barcodeData = barcode.m_barcodeData;
	m_barcodeType = barcode.m_barcodeType;
}


CBarcode& CBarcode::operator=(const CBarcode& barcode)
{
	m_barcodeData = barcode.m_barcodeData;
	m_barcodeType = barcode.m_barcodeType;

	return *this;
}



bool CBarcode::operator==(const CBarcode& barcode) const
{
	if (m_barcodeType == barcode.m_barcodeType && m_barcodeData == barcode.m_barcodeData){
		return true;
	}

	return false;
}


bool CBarcode::operator!=(const CBarcode& barcode) const
{
	return !operator == (barcode);
}


void CBarcode::SetData(const QString& barcodeData)
{
	if (barcodeData != m_barcodeData){
		istd::CChangeNotifier changePtr(this);

		m_barcodeData = barcodeData;
	}
}


// reimplemented (ibarcode::IBarcode)

const QString& CBarcode::GetBarcodeData() const
{
	return m_barcodeData;
}


int CBarcode::GetBarcodeType() const
{
	return m_barcodeType;
}


// reimplemented (iser::ISerializable)

bool CBarcode::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag barcodeTypeTag("BarcodeType", "Type of the barcode");
	retVal = retVal && archive.BeginTag(barcodeTypeTag);
	retVal = retVal && archive.Process(m_barcodeType);
	retVal = retVal && archive.EndTag(barcodeTypeTag);
		
	static iser::CArchiveTag barcodeDataTag("BarcodeData", "Data of the barcode");
	retVal = retVal && archive.BeginTag(barcodeDataTag);
	retVal = retVal && archive.Process(m_barcodeData);

	retVal = retVal && archive.EndTag(barcodeDataTag);

	return retVal;
}


} // namespace ibarcode


