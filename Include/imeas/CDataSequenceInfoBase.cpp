#include "imeas/CDataSequenceInfoBase.h"


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
	return 0;
}


// reimplemented (iser::ISerializable)

bool CDataSequenceInfoBase::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


} // namespace imeas


