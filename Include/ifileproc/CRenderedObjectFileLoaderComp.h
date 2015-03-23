#ifndef ifileproc_CRenderedObjectFileLoaderComp_included
#define ifileproc_CRenderedObjectFileLoaderComp_included


// Qt includes
#include <QtCore/QDateTime>

// ACF includes
#include "istd/TSmartPtr.h"
#include "iser/ISerializable.h"
#include "icomp/CComponentBase.h"
#include "ifile/IFilePersistence.h"
#include "iimg/CBitmap.h"
#include "iproc/IProcessor.h"


namespace ifileproc
{


class CRenderedObjectFileLoaderComp:
			public icomp::CComponentBase,
			virtual public ifile::IFilePersistence,
			virtual public iser::ISerializable
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CRenderedObjectFileLoaderComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN_MULTI_0(m_fileLoadersCompPtr, "FileLoaders", "List of the supported file loaders, which will be used to open the input file", true);
		I_ASSIGN_MULTI_0(m_objectsListCompPtr, "DataObjects", "List of data object which will be loaded for preview generation", true);
		I_ASSIGN_MULTI_0(m_previewGenerationProcessorsCompPtr, "PreviewGenerationProcessors", "Processor used for generation of the preview bitmap", true);
		I_ASSIGN(m_widthAttrPtr, "BitmapWidth", "Width of the generated preview bitmap", true, 128);
		I_ASSIGN(m_heightAttrPtr, "BitmapHeight", "Height of the generated preview bitmap", true, 128);
		I_ASSIGN(m_maxCacheSizeAttrPtr, "MaxCacheSize", "Maximal number of bitmaps in cache", false, 100);
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
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	ifile::IFilePersistence* GetLoaderForFile(const QString& filePath) const;
	istd::IChangeable* GetDataObjectForFile(const QString& filePath) const;
	iproc::IProcessor* GetPreviewGeneratorForFile(const QString& filePath) const;

private:
	I_MULTIREF(ifile::IFilePersistence, m_fileLoadersCompPtr);
	I_MULTIREF(istd::IChangeable, m_objectsListCompPtr);
	I_MULTIREF(iproc::IProcessor, m_previewGenerationProcessorsCompPtr);
	I_ATTR(int, m_widthAttrPtr);
	I_ATTR(int, m_heightAttrPtr);
	I_ATTR(int, m_maxCacheSizeAttrPtr);

	struct FileInfo
	{
		QDateTime fileTimeStamp;
		istd::TSmartPtr<iimg::CBitmap> fileBitmapPtr;
	};

	typedef QMap<QString, FileInfo> PreviewCache;

	mutable PreviewCache m_previewCache;
};


} // namespace ifileproc


#endif // !ifileproc_CRenderedObjectFileLoaderComp_included


