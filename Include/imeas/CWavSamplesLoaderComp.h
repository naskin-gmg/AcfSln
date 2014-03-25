#ifndef imeas_CWavSamplesLoaderComp_included
#define imeas_CWavSamplesLoaderComp_included


// ACF includes
#include "ifile/IFilePersistence.h"
#include "icomp/CComponentBase.h"


namespace imeas
{


class CWavSamplesLoaderComp:
			public icomp::CComponentBase,
			virtual public ifile::IFilePersistence
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CWavSamplesLoaderComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
	I_END_COMPONENT;

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual int LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;
	virtual int SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	enum WavContaints
	{
		WC_BYTES_PER_SAMPLE = 2,
		WC_BITS_PER_SAMPLE = WC_BYTES_PER_SAMPLE * 8,
		WC_WAVE_FORMAT_PCM = 1
	};
};


} // namespace imeas


#endif // !imeas_CWavSamplesLoaderComp_included


