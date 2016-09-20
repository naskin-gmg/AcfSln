#ifndef iqtex_CXslSerializerComp_included
#define iqtex_CXslSerializerComp_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <ifile/CFileSerializerCompBase.h>
#include <ifile/IFileNameParam.h>


namespace iqtex
{


/**
	Template implementation of file serializer using loading and storing archive implementation.
*/
class CXslSerializerComp: public ifile::CFileSerializerCompBase
{
public:	
	typedef ifile::CFileSerializerCompBase BaseClass;

	I_BEGIN_COMPONENT(CXslSerializerComp);
		I_ASSIGN(m_xslReadFilePath, "ReadTransformationFile", "Transformation file for reading action", true, "ReadTransformationFile");
		I_ASSIGN(m_xslWriteFilePath, "WriteTransformationFile", "Transformation file for writing action", true, "WriteTransformationFile");
	I_END_COMPONENT;

	// reimplemented (ifile::IFilePersistence)
	virtual int LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;
	virtual int SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;

protected:
	/**
		Called if read error is occured.
	*/
	virtual void OnReadError(const iser::IArchive& archive, const istd::IChangeable& data, const QString& filePath) const;

private:
	I_REF(ifile::IFileNameParam, m_xslReadFilePath);
	I_REF(ifile::IFileNameParam, m_xslWriteFilePath);
};


} // namespace iqtex


#endif // !iqtex_CXslSerializerComp_included


