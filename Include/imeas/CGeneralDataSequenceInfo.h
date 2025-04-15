#pragma once


// ACF-Solutions includes
#include <imeas/IDataSequenceInfo.h>
#include <imeas/CGeneralNumericConstraints.h>


namespace imeas
{


/**
	Simple implementation of imeas::IDataSequenceInfo interface.
*/
class CGeneralDataSequenceInfo:
			public CGeneralNumericConstraints,
			virtual public IDataSequenceInfo
{
public:
	typedef imeas::CGeneralNumericConstraints BaseClass;

	CGeneralDataSequenceInfo(
				int defaultChannelsCount,
				int defaultSamplesCount,
                IDataSequenceInfo::WeightMode mode = WM_NONE,
				int sequenceFlags = 0);

	void SetSequenceInfoFlags(int flags);
	void SetDefaultSamplesCount(int samplesCount);
	void SetDefaultChannelsCount(int channelsCount);
	void SetWeightMode(WeightMode mode);

	// reimplemented (CGeneralNumericConstraints)
	virtual bool InsertValueInfo(
				const QString& name,
				const QString& description,
				const QByteArray& valueId,
				const imath::CGeneralUnitInfo& unitInfo,
				int index = -1) override;

	// reimplemented (imeas::IDataSequenceInfo)
	virtual int GetSequenceInfoFlags() const override;
	virtual int GetDefaultSamplesCount() const override;
	virtual int GetDefaultChannelsCount() const override;
	virtual int GetWeightMode() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	WeightMode m_weightMode;
	int m_sequenceInfoFlags;
	int m_defaultSamplesCount;
	int m_defaultChannelsCount;
};


} // namespace imeas


