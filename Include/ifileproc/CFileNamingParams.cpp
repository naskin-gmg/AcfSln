#include <ifileproc/CFileNamingParams.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace ifileproc
{


// public methods

CFileNamingParams::CFileNamingParams()
	:m_overwriteStrategy(RM_NUMBERING)
{
}


// reimplemented (ifileproc::IFileNamingParams)

CFileNamingParams::OverwriteStrategy CFileNamingParams::GetOverwriteStrategy() const
{
	return m_overwriteStrategy;
}


void CFileNamingParams::SetOverwriteStrategy(OverwriteStrategy overwriteStrategy)
{
	if (m_overwriteStrategy != overwriteStrategy){
		istd::CChangeNotifier changePtr(this);

		m_overwriteStrategy = overwriteStrategy;
	}
}


QString CFileNamingParams::GetPrefix() const
{
	return m_prefix;
}


void CFileNamingParams::SetPrefix(const QString& prefix)
{
	if (m_prefix != prefix){
		istd::CChangeNotifier changePtr(this);

		m_prefix = prefix;
	}
}


QString CFileNamingParams::GetSuffix() const
{
	return m_suffix;
}


void CFileNamingParams::SetSuffix(const QString& suffix)
{
	if (m_suffix != suffix){
		istd::CChangeNotifier changePtr(this);

		m_suffix = suffix;
	}
}


QStringList CFileNamingParams::GetPatternsToRemove() const
{
	return m_patternsToRemove;
}


void CFileNamingParams::SetPatternsToRemove(const QStringList& patternsToRemove)
{
	if (m_patternsToRemove != patternsToRemove){
		istd::CChangeNotifier changePtr(this);

		m_patternsToRemove = patternsToRemove;
	}
}


// reimplemented (iser::ISerializable)

bool CFileNamingParams::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag overwriteStrategyTag("OverwriteStrategy", "Strategy for overwriting of existing files");
	static iser::CArchiveTag prefixTag("FilePrefix", "Prefix for the output file name");
	static iser::CArchiveTag suffixTag("FileSuffix", "Suffix for the output file name");

	istd::CChangeNotifier changePtr(!archive.IsStoring()? this : NULL);

	bool retVal = true;

	int overwriteMode = m_overwriteStrategy;

	retVal = retVal && archive.BeginTag(overwriteStrategyTag);
	retVal = retVal && archive.Process(overwriteMode);
	retVal = retVal && archive.EndTag(overwriteStrategyTag);

	if (retVal){
		m_overwriteStrategy = OverwriteStrategy(overwriteMode);
	}

	retVal = retVal && archive.BeginTag(prefixTag);
	retVal = retVal && archive.Process(m_prefix);
	retVal = retVal && archive.EndTag(prefixTag);

	retVal = retVal && archive.BeginTag(suffixTag);
	retVal = retVal && archive.Process(m_suffix);
	retVal = retVal && archive.EndTag(suffixTag);

	return retVal;
}


int CFileNamingParams::GetSupportedOperations() const
{
	return SO_COPY | SO_COMPARE | SO_CLONE;
}


bool CFileNamingParams::CopyFrom(const istd::IChangeable& object, istd::IChangeable::CompatibilityMode /*mode*/)
{
	const CFileNamingParams* sourcePtr = dynamic_cast<const CFileNamingParams*>(&object);
	if (sourcePtr != NULL){
		m_overwriteStrategy = sourcePtr->m_overwriteStrategy;
		m_suffix = sourcePtr->m_suffix;
		m_prefix = sourcePtr->m_prefix;
		m_patternsToRemove = sourcePtr->m_patternsToRemove;

		return true;
	}

	return false;
}


bool CFileNamingParams::IsEqual(const istd::IChangeable& object) const
{
	const CFileNamingParams* sourcePtr = dynamic_cast<const CFileNamingParams*>(&object);
	if (sourcePtr != NULL){
		return
					(m_overwriteStrategy == sourcePtr->m_overwriteStrategy) &&
					(m_suffix == sourcePtr->m_suffix) &&
					(m_prefix == sourcePtr->m_prefix) &&
					(m_patternsToRemove == sourcePtr->m_patternsToRemove);
	}

	return false;
}


istd::IChangeable* CFileNamingParams::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	istd::TDelPtr<CFileNamingParams> objectPtr(new CFileNamingParams);
	if (objectPtr->CopyFrom(*this, mode)){
		return objectPtr.PopPtr();
	}

	return NULL;
}


bool CFileNamingParams::ResetData(CompatibilityMode /*mode*/)
{
	istd::CChangeNotifier changeNotifier(this);

	m_overwriteStrategy = RM_NUMBERING;
	m_prefix.clear();
	m_suffix.clear();
	m_patternsToRemove.clear();

	return true;
}


} // namespace ifileproc


