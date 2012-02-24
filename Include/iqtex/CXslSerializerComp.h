#ifndef iqtex_CXslSerializerComp_included
#define iqtex_CXslSerializerComp_included


// Qt includes
#include <QObject>


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"
#include "istd/itr.h"
#include <QString>

#include "isys/IFileSystem.h"

#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"
#include "ibase/CFileTypeInfoComp.h"

#include "iprm/IFileNameParam.h"

#include "iqtex/CXslTransformationReadArchive.h"
#include "iqtex/CXslTransformationWriteArchive.h"


namespace iqtex
{


/**
	Template implementation of file serializer using loading and storing archive implementation.
*/
class CXslSerializerComp:
	public ibase::TLoggerCompWrap<ibase::CFileTypeInfoComp>,
			virtual public iser::IFileLoader
{
	I_DECLARE_TR_FUNCTION(CXslSerializerComp);
public:	
	typedef ibase::TLoggerCompWrap<ibase::CFileTypeInfoComp> BaseClass;

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xac10,
		MI_UNSUPPORTED_VERSION
	};

	I_BEGIN_COMPONENT(CXslSerializerComp);
		I_REGISTER_INTERFACE(iser::IFileTypeInfo);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
		I_ASSIGN(m_xslReadFilePath, "ReadTransformationFile", "Transformation file for reading action", true, "ReadTransformationFile");
		I_ASSIGN(m_xslWriteFilePath, "WriteTransformationFile", "Transformation file for writing action", true, "WriteTransformationFile");
	I_END_COMPONENT;

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath = QString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath = QString()) const;

protected:
	/**
		Get working version info.
	*/
	virtual const iser::IVersionInfo* GetVersionInfo() const;

	/**
		Called if read error is occured.
	*/
	virtual void OnReadError(const iser::IArchive& archive, const istd::IChangeable& data, const QString& filePath) const;

	/**
		Check if minimal version of some serializable object is supported by version info.
	*/
	bool CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const;

	bool CheckInputFilePath(const QString filePath) const;

	bool CheckTargetDirectory(const QString dirPath) const;

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
	I_REF(iprm::IFileNameParam, m_xslReadFilePath);
	I_REF(iprm::IFileNameParam, m_xslWriteFilePath);
};


} // namespace iqtex


#endif // !iqtex_CXslSerializerComp_included


