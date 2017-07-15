#include <ibarcode/CGeneralBarcodeSearchParams.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace ibarcode
{


// public methods

// reimplemented (IGeneralBarcodeSearchParams)

IGeneralBarcodeSearchParams::BarcodeTypes CGeneralBarcodeSearchParams::GetSearchBarcodeTypes() const
{
	return m_barcodeTypes;
}


void CGeneralBarcodeSearchParams::SetSearchBarcodeTypes(const BarcodeTypes& barcodeTypes)
{
	if (m_barcodeTypes != barcodeTypes){
		istd::CChangeNotifier changeNotifier(this);

		m_barcodeTypes = barcodeTypes;
	}
}


// reimplemented (iser::ISerializable)

bool CGeneralBarcodeSearchParams::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag barcodeTypesTag("BarcodeTypes", "List of barcode types", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag barcodeTypeTag("Type", "Barcode type", iser::CArchiveTag::TT_LEAF);

	int typesCount = m_barcodeTypes.count();

	retVal = retVal && archive.BeginMultiTag(barcodeTypesTag, barcodeTypeTag, typesCount);

	if (!archive.IsStoring()){
		for (int typeIndex = 0; typeIndex < typesCount; ++typeIndex){
			IBarcode::BarcodeType barcodeType = ibarcode::IBarcode::BT_UNKNOWN;
			retVal = retVal && archive.BeginTag(barcodeTypeTag);
			retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum<IBarcode::BarcodeType, IBarcode::ToString, IBarcode::FromString>(archive, barcodeType);
			retVal = retVal && archive.EndTag(barcodeTypeTag);
		}
	}
	else{
		for (BarcodeTypes::ConstIterator iter = m_barcodeTypes.constBegin(); iter != m_barcodeTypes.constEnd(); ++iter){
			IBarcode::BarcodeType barcodeType = *iter;

			retVal = retVal && archive.BeginTag(barcodeTypeTag);
			retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum<IBarcode::BarcodeType, IBarcode::ToString, IBarcode::FromString>(archive, barcodeType);
			retVal = retVal && archive.EndTag(barcodeTypeTag);
		}
	}

	retVal = retVal && archive.EndTag(barcodeTypesTag);

	return retVal;
}


} // namespace ibarcode


