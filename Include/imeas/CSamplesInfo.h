#ifndef imeas_CSamplesInfo_included
#define imeas_CSamplesInfo_included


#include <imeas/IDataSequenceInfo.h>


namespace imeas
{


/**
	Stores additional data sequence data used to interpret samples value correctly.
*/
class CSamplesInfo: virtual public imeas::IDataSequenceInfo
{
public:
	CSamplesInfo(const istd::CRange& logicalSamplesRange = istd::CRange::GetInvalid());

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

private:

	istd::CRange m_logicalSamplesRange;
};


} // namespace imeas


#endif // !imeas_CSamplesInfo_included


