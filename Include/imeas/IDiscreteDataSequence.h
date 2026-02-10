// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef imeas_IDiscreteDataSequence_included
#define imeas_IDiscreteDataSequence_included


#include <imeas/IDataSequence.h>


namespace imeas
{


/**
	General definition of sequence contains discrete sample values in regular time grid.
	All samples has the same number of channels components.
*/
class IDiscreteDataSequence: virtual public IDataSequence
{
public:
	/**
		Create container for sample sequence using external samples buffer.
		\param	samplesCount	number of samples.
		\param	dataPtr			pointer to external buffer.
		\param	releaseFlag		if its true, external buffer will be managed (removed) by this object.
		\param	sampleDiff		address difference between next and previos sample. If it equals 0, the value will be taken from number of samples, number of channels and \c channelDiff.
		\param	channelDiff		address difference between next and previos sample. If it equals 0, the value will be calculated from number of bits per single sample.
		\param	sampleDepth		number of bits used for representation of a single sample.
		\param	channelsCount	number of channels.
		\return					true if sample sequence was created correctly.
	*/
	virtual bool CreateDiscreteSequence(
				int samplesCount,
				void* dataPtr,
				bool releaseFlag,
				int sampleDiff,
				int channelDiff,
				int sampleDepth,
				int channelsCount = 1) = 0;
	/**
		Create container for sample sequence with additional sequence info using external samples buffer.
		\param	infoPtr			sequence info object.
		\param	samplesCount	number of samples.
		\param	dataPtr			pointer to external buffer.
		\param	releaseFlag		if its true, external buffer will be managed (removed) by this object.
		\param	sampleDiff		address difference between next and previos sample. If it equals 0, the value will be taken from number of samples, number of channels and \c channelDiff.
		\param	channelDiff		address difference between next and previos sample. If it equals 0, the value will be calculated from number of bits per single sample.
		\param	sampleDepth		number of bits used for representation of a single sample.
		\param	channelsCount	number of channels.
		\return					true if sample sequence was created correctly.
	*/
	virtual bool CreateDiscreteSequenceWithInfo(
				const istd::TTransPtr<const IDataSequenceInfo>& infoPtr,
				int samplesCount,
				void* dataPtr,
				bool releaseFlag,
				int sampleDiff,
				int channelDiff,
				int sampleDepth,
				int channelsCount = 1) = 0;
	/**
		Get number of bits used to represent single sample.
	*/
	virtual int GetSampleDepth() const = 0;

	/**
		Get discrete sample at specified position.
		\param	position	position, must be inside of returned size.
	*/
	virtual quint32 GetDiscreteSample(int position, int channel = 0) const = 0;

	/**
		Set discrete sample at specified position.
		\param	position	position, must be inside of returned size.
		\param	channel		sample channel.
		\param	sample		sample value will be set.
		\return				true, if sample was changed.
	*/
	virtual bool SetDiscreteSample(int position, int channel, quint32 sample) = 0;
};


} // namespace imeas


#endif // !imeas_IDiscreteDataSequence_included


