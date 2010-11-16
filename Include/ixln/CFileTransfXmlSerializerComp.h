#ifndef ixln_CFileTransfXmlSerializerComp_included
#define ixln_CFileTransfXmlSerializerComp_included


// ACF includes
#include "iser/IFileLoader.h"
#include "iser/IArchive.h"
#include "ibase/TLoggerCompWrap.h"
#include "ibase/CFileTypeInfoComp.h"
#include "iprm/IFileNameParam.h"

#include "ixln/ixln.h"


namespace ixln
{


/**
	Template implementation of file serializer using loading and storing archive implementation.
*/
class CFileTransfXmlSerializerComp:
			public ibase::TLoggerCompWrap<ibase::CFileTypeInfoComp>,
			virtual public iser::IFileLoader
{
public:
	typedef ibase::TLoggerCompWrap<ibase::CFileTypeInfoComp> BaseClass;

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xac10,
		MI_UNSUPPORTED_VERSION,
		MI_BAD_TRANSFORMATION
	};

	I_BEGIN_COMPONENT(CFileTransfXmlSerializerComp);
		I_REGISTER_INTERFACE(iser::IFileTypeInfo);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
		I_ASSIGN(m_transformFileNameParamCompPtr, "TransformFile", "Parameter storing file name of XSL transformation", true, "TransformFile");
	I_END_COMPONENT;

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = 0,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;

protected:
	/**
		Get working version info.
	*/
	virtual const iser::IVersionInfo* GetVersionInfo() const;

	/**
		Called if read error is occured.
	*/
	virtual void OnReadError(const iser::IArchive& archive, const istd::IChangeable& data, const istd::CString& filePath) const;

	/**
		Check if minimal version of some serializable object is supported by version info.
	*/
	bool CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const;

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
	I_REF(iprm::IFileNameParam, m_transformFileNameParamCompPtr);
};


} // namespace ixln


#endif // !ixln_CFileTransfXmlSerializerComp_included


