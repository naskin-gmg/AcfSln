#ifndef imeas_CDataSequenceInfoBase_included
#define imeas_CDataSequenceInfoBase_included


// ACF includes
#include "imeas/IDataSequenceInfo.h"


namespace imeas
{


class CDataSequenceInfoBase: virtual public imeas::IDataSequenceInfo
{
public:
	// reimplemented (imeas::IDataSequenceInfo)
	virtual int GetSequenceInfoFlags() const;
	virtual int GetDefaultSamplesCount() const;
	virtual int GetDefaultChannelsCount() const;
	virtual int GetWeightMode() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
};


} // namespace imeas


#endif // !imeas_CDataSequenceInfoBase_included


