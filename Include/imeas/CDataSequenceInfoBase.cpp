#include "imeas/CDataSequenceInfoBase.h"


// Qt includes
#include <QtCore/QObject>

// ACF-Solutions includes
#include "imeas/CGeneralUnitInfo.h"


namespace imeas
{


// reimplemented (imeas::IDataSequenceInfo)

int CDataSequenceInfoBase::GetSequenceInfoFlags() const
{
	return 0;
}


int CDataSequenceInfoBase::GetDefaultSamplesCount() const
{
	return 0;
}


int CDataSequenceInfoBase::GetDefaultChannelsCount() const
{
	return 0;
}


int CDataSequenceInfoBase::GetWeightMode() const
{
	return WM_NONE;
}


// reimplemented (imeas::INumericConstraints)

int CDataSequenceInfoBase::GetNumericValuesCount() const
{
	return std::max(0, GetDefaultChannelsCount());
}


QString CDataSequenceInfoBase::GetNumericValueName(int index) const
{
	return QObject::tr("Channel %1").arg(index + 1);
}


QString CDataSequenceInfoBase::GetNumericValueDescription(int /*index*/) const
{
	return QString();
}


const imeas::IUnitInfo& CDataSequenceInfoBase::GetNumericValueUnitInfo(int /*index*/) const
{
	static imeas::CGeneralUnitInfo realNumberInfo;

	return realNumberInfo;
}


// reimplemented (iser::ISerializable)

bool CDataSequenceInfoBase::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


} // namespace imeas


