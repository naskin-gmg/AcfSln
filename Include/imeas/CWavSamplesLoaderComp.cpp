#include "imeas/CWavSamplesLoaderComp.h"


// STL includes
#include <fstream>

// ACF includes
#include "istd/CRange.h"

#include "imeas/ISamplesSequence.h"


namespace imeas
{


#pragma pack(push, 1)


struct PrefixHeader
{
	char code[4];
	I_DWORD size;
};

struct WavHeader: public PrefixHeader
{
	char wave[4];
};

struct FmtHeader: public PrefixHeader
{
	I_WORD formatTag;
	I_WORD channelsCount;
	I_DWORD samplesPerSecond;
	I_DWORD bytesPerSecond;
	I_WORD blockAlign;
};

struct PcmFmtHeader: public FmtHeader
{
	I_WORD bitsPerSample;
};


#pragma pack(pop)


// reimplemented (iser::IFileLoader)

bool CWavSamplesLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool /*beQuiet*/) const
{
	if ((filePathPtr != NULL) && filePathPtr->IsEmpty()){
		return false;
	}

	return		((dataObjectPtr == NULL) || (dynamic_cast<const ISamplesSequence*>(dataObjectPtr) != NULL)) &&
				((flags & QF_ANONYMOUS_ONLY) == 0) &&
				((flags & QF_NO_SAVING) == 0);
}


int CWavSamplesLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_NO_SAVING, false)){
		return StateFailed;
	}

	return StateFailed;
}


int CWavSamplesLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_NO_LOADING, false)){
		return StateFailed;
	}

	std::ofstream fileStream(filePath.ToString().c_str(), std::ios::out | std::ios_base::binary);
	if (!fileStream.is_open()){
		return StateFailed;
	}

	int retVal = StateFailed;

	const ISamplesSequence* sequencePtr = dynamic_cast<const ISamplesSequence*>(&data);
	I_ASSERT(sequencePtr != NULL);

	if (sequencePtr != NULL){
		int samplesCount = sequencePtr->GetTimeSamplesCount();
		int channelsCount = sequencePtr->GetChannelsCount();
		double samplingPeriod = sequencePtr->GetSamplingPeriod();

		WavHeader header;
		memcpy(header.code, "RIFF", 4);
		memcpy(header.wave, "WAVE", 4);
		header.size = I_DWORD(sizeof(WavHeader) + sizeof(PcmFmtHeader) + sizeof(PrefixHeader) + BYTES_PER_SAMPLE * samplesCount * channelsCount) - 8;

		fileStream.write((const char*)&header, sizeof(header));

		PcmFmtHeader pcmFormat;
		memcpy(pcmFormat.code, "fmt ", 4);
		pcmFormat.size = sizeof(pcmFormat) - 8;
		pcmFormat.formatTag = WAVE_FORMAT_PCM;
		pcmFormat.channelsCount = I_WORD(channelsCount);
		pcmFormat.samplesPerSecond = (samplingPeriod > 0)? I_DWORD(1 / samplingPeriod): 1000;
		pcmFormat.bytesPerSecond = I_DWORD(pcmFormat.samplesPerSecond * BYTES_PER_SAMPLE * channelsCount);
		pcmFormat.blockAlign = I_WORD(channelsCount * BYTES_PER_SAMPLE);
		pcmFormat.bitsPerSample = BITS_PER_SAMPLE;

		fileStream.write((const char*)&pcmFormat, sizeof(pcmFormat));

		PrefixHeader dataPrefix;
		memcpy(dataPrefix.code, "data", 4);
		dataPrefix.size = I_DWORD(BYTES_PER_SAMPLE * samplesCount * channelsCount);

		fileStream.write((const char*)&dataPrefix, sizeof(dataPrefix));

		istd::CRange range = istd::CRange::GetInvalid();
		for (int sampleIndex = 0; sampleIndex < samplesCount; ++sampleIndex){
			for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
				double sample = sequencePtr->GetSample(sampleIndex, channelIndex);

				range.Unite(sample);
			}
		}

		double offset = pow(2.0, BITS_PER_SAMPLE - 1);
		double amplitude = pow(2.0, BITS_PER_SAMPLE - 1) - I_BIG_EPSILON;
		for (int sampleIndex = 0; sampleIndex < samplesCount; ++sampleIndex){
			for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
				double sample = sequencePtr->GetSample(sampleIndex, channelIndex);

				I_SDWORD rawSample = I_SDWORD(range.GetAlphaFromValue(sample) * amplitude + offset);

				fileStream.write((const char*)&rawSample, BYTES_PER_SAMPLE);
			}
		}

		retVal = fileStream.good() ? StateOk : StateFailed;
	}

	fileStream.close();

	return retVal;
}


bool CWavSamplesLoaderComp::GetFileExtensions(istd::CStringList& result, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	result.push_back("wav");

	return true;
}


istd::CString CWavSamplesLoaderComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if ((extensionPtr != NULL) && (*extensionPtr != "wav")){
		return "";
	}

	return "Sound file";
}


} // namespace imeas


