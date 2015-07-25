#ifndef imeas_CDataSequenceInfoBase_included
#define imeas_CDataSequenceInfoBase_included


// ACF includes
#include "imeas/IDataSequenceInfo.h"


namespace imeas
{


/**
	Simple base class for implementations of interface \c imeas::IDataSequenceInfo.
*/
class CDataSequenceInfoBase: virtual public imeas::IDataSequenceInfo
{
public:
	// reimplemented (imeas::IDataSequenceInfo)
	virtual int GetSequenceInfoFlags() const;
	virtual int GetDefaultSamplesCount() const;
	virtual int GetDefaultChannelsCount() const;
	virtual int GetWeightMode() const;

	// reimplemented (imeas::INumericConstraints)
	virtual int GetNumericValuesCount() const;
	virtual QString GetNumericValueName(int index) const;
	virtual QString GetNumericValueDescription(int index) const;
	virtual const imath::IUnitInfo* GetNumericValueUnitInfo(int index) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
};


} // namespace imeas


#endif // !imeas_CDataSequenceInfoBase_included


