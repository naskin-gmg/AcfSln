// AcfSln includes
#include "ifpf/CFileNamingParamsComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ifpf
{


CFileNamingParamsComp::CFileNamingParamsComp()
	:m_renamingMode(RM_NUMBERING)
{
}

// reimplemented (ifpf::IFileNamingParams)

int CFileNamingParamsComp::GetRenamingMode() const
{
	return m_renamingMode;
}


void CFileNamingParamsComp::SetRenamingMode(int renamingMode)
{
	if (m_renamingMode != renamingMode){
		istd::CChangeNotifier changePtr(this);

		m_renamingMode = renamingMode;
	}
}


istd::CString CFileNamingParamsComp::GetPrefix() const
{
	return m_prefix;
}


void CFileNamingParamsComp::SetPrefix(const istd::CString& prefix)
{
	if (m_prefix != prefix){
		istd::CChangeNotifier changePtr(this);

		m_prefix = prefix;
	}
}


istd::CString CFileNamingParamsComp::GetSuffix() const
{
	return m_suffix;
}


void CFileNamingParamsComp::SetSuffix(const istd::CString& suffix)
{
	if (m_suffix != suffix){
		istd::CChangeNotifier changePtr(this);

		m_suffix = suffix;
	}
}


// reimplemented (iser::ISerializable)

bool CFileNamingParamsComp::Serialize(iser::IArchive& archive)
{		
	bool retVal = true;

	static iser::CArchiveTag renamingModeTag("RenamingMode", "Mode for the file renaming");
	retVal = retVal && archive.BeginTag(renamingModeTag);
	retVal = retVal && archive.Process(m_renamingMode);
	retVal = retVal && archive.EndTag(renamingModeTag);

	static iser::CArchiveTag prefixTag("FilePrefix", "File prefix");
	retVal = retVal && archive.BeginTag(prefixTag);
	retVal = retVal && archive.Process(m_prefix);
	retVal = retVal && archive.EndTag(prefixTag);

	static iser::CArchiveTag suffixTag("FileSuffix", "FileSuffix");
	retVal = retVal && archive.BeginTag(suffixTag);
	retVal = retVal && archive.Process(m_suffix);
	retVal = retVal && archive.EndTag(suffixTag);

	return retVal;
}


} // namespace ifpf


