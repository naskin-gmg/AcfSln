#ifndef imeas_CDataSequenceStatistics_included
#define imeas_CDataSequenceStatistics_included


// ACF includes
#include "istd/TPointerVector.h"

#include "imod/TModelWrap.h"


// ACF-Solutions includes
#include "imeas/IDataSequenceStatistics.h"
#include "imeas/CDataStatistics.h"


namespace imeas
{


class CDataSequenceStatistics: virtual public imeas::IDataSequenceStatistics
{
public:
	// reimplemented (imeas::IDataSequenceStatistics)
	virtual bool SetChannelStatistics(const imeas::IDataStatistics& channelStatistics, int channelIndex);
	virtual void ResetStatistics();
	virtual int GetChannelsCount() const;
	virtual const imeas::IDataStatistics* GetChannelStatistics(int channelIndex) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	typedef imod::TModelWrap<imeas::CDataStatistics> ChannelStatistics;
	typedef istd::TPointerVector<ChannelStatistics> ChannelStatisticsList;

	ChannelStatisticsList m_channelStatisticsList;
};


} // namespace imeas


#endif // !imeas_CDataSequenceStatistics_included


