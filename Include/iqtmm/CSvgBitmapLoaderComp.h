#ifndef iqtmm_CSvgBitmapLoaderComp_included
#define iqtmm_CSvgBitmapLoaderComp_included


#include "ifile/IFilePersistence.h"

#include "icomp/CComponentBase.h"

#include "ilog/TLoggerCompWrap.h"

#include "iimg/CBitmap.h"

#include "iqtmm/iqtmm.h"


namespace iqtmm
{


/**
	Load SVG files to bitmap.
*/
class CSvgBitmapLoaderComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			virtual public ifile::IFilePersistence
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSvgBitmapLoaderComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN(m_bitmapWidthAttrPtr, "BitmapWidth", "Width of rendered bitmap in pixels", true, 100);
		I_ASSIGN(m_bitmapHeightAttrPtr, "BitmapHeight", "Height of rendered bitmap in pixels", true, 100);
	I_END_COMPONENT;

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xa7e0,
		MI_FILE_NOT_EXIST,
		MI_BAD_FORMAT,
		MI_BITMAP_TYPE
	};

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

private:
	I_ATTR(int, m_bitmapWidthAttrPtr);
	I_ATTR(int, m_bitmapHeightAttrPtr);

	mutable QString m_lastFilePath;
	mutable iimg::CBitmap m_lastBitmap;
};


} // namespace iqtmm


#endif // !iqtmm_CSvgBitmapLoaderComp_included


