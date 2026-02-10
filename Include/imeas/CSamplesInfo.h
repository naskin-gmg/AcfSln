// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef imeas_CSamplesInfo_included
#define imeas_CSamplesInfo_included


// Qt includes
#include <QtCore/QDateTime>

// ACF includes
#include <imeas/IDataSequenceInfo.h>


namespace imeas
{


/**
	Stores additional data sequence data used to interpret samples value correctly.
*/
class CSamplesInfo:
			virtual public imeas::IDataSequenceInfo,
			protected iprm::IOptionsList
{
public:
	CSamplesInfo(const istd::CRange& logicalSamplesRange = istd::CRange::GetInvalid(), const QDateTime& samplingStartTime = QDateTime());

	/**
		Get logical position range (time axis) of this data sequence.
		For example, if you sample sound from second 1 to 5 it will be represented as range [1.0, 5.0].
		If this range is invalid, no logical range is provided.
	*/
	const istd::CRange& GetLogicalSamplesRange() const;
	/**
		Set logical position range (time axis) of this data sequence.
		\sa GetLogicalRange
	*/
	void SetLogicalSamplesRange(const istd::CRange& range);

	/**
		Get the absolute start time of the sampling.
		If the time object is invalid the
	*/
	QDateTime GetSamplingStartTime() const;

	/**
		Set absolute time of sample acquisition.
	*/
	void SetSamplingStartTime(const QDateTime& samplingStartTime);

	// reimplemented (imeas::IDataSequenceInfo)
	virtual int GetSequenceInfoFlags() const override;
	virtual int GetDefaultSamplesCount() const override;
	virtual int GetDefaultChannelsCount() const override;
	virtual int GetWeightMode() const override;

	// reimplemented (imeas::INumericConstraints)
	virtual const iprm::IOptionsList& GetValueListInfo() const override;
	virtual const imath::IUnitInfo* GetNumericValueUnitInfo(int index) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const override;
	virtual int GetOptionsCount() const override;
	virtual QString GetOptionName(int index) const override;
	virtual QString GetOptionDescription(int index) const override;
	virtual QByteArray GetOptionId(int index) const override;
	virtual bool IsOptionEnabled(int index) const override;

private:
	istd::CRange m_logicalSamplesRange;
	QDateTime m_samplingStartTime;
};


} // namespace imeas


#endif // !imeas_CSamplesInfo_included


