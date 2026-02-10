// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iqtmm_CMediaLoaderComp_included
#define iqtmm_CMediaLoaderComp_included


#include <ifile/IFilePersistence.h>

#include <icomp/CComponentBase.h>

#include <ilog/TLoggerCompWrap.h>

#include <iqtmm/iqtmm.h>


namespace iqtmm
{


class CMediaLoaderComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			virtual public ifile::IFilePersistence
{
	Q_OBJECT

public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMediaLoaderComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN(m_autoPlayAttrPtr, "AutoPlay", "If enabled, movie will be automatically played after loading", true, true);
	I_END_COMPONENT;

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual ifile::IFilePersistence::OperationState LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;
	virtual ifile::IFilePersistence::OperationState SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

private:
	I_ATTR(bool, m_autoPlayAttrPtr);
};


} // namespace iqtmm


#endif // !iqtmm_CMediaLoaderComp_included

