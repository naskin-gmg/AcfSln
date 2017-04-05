#pragma once


// ACF-Solutions includes
#include <imeas/IDataSequenceInfo.h>
#include <imeas/CGeneralNumericConstraints.h>


namespace imeas
{


/**
	Simple implementation of imeas::IDataSequenceInfo interface.
*/
class CGeneralDataSequenceInfo : public CGeneralNumericConstraints,
								virtual public IDataSequenceInfo
{
public:

	typedef imeas::CGeneralNumericConstraints BaseClass;


	CGeneralDataSequenceInfo(int defaultChannelsCount,
									int defaultSamplesCount,
									WeightMode mode = WeightMode::WM_NONE,
									int sequenceFlags = 0);

	bool InsertValueInfo(const QString& name, const QString& description, const imath::CGeneralUnitInfo& unitInfo, int index = -1);

	void SetSequenceInfoFlags(unsigned flags);
	void SetDefaultSamplesCount(unsigned samplesCount);
	void SetDefaultChannelsCount(unsigned channelsCount);
	void SetWeightMode(WeightMode mode);

	// reimplemented (imeas::IDataSequenceInfo)
	virtual int GetSequenceInfoFlags() const;
	virtual int GetDefaultSamplesCount() const;
	virtual int GetDefaultChannelsCount() const;
	virtual int GetWeightMode() const;


	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:

	WeightMode m_weightMode;
	int m_sequenceInfoFlags;
	int m_defaultSamplesCount;
	int m_defaultChannelsCount;
};


} // namespace imeas


