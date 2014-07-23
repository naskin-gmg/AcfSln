#include "ihotf/CDirectoryMonitorParams.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ihotf
{


// public methods

CDirectoryMonitorParams::CDirectoryMonitorParams()
	:m_poolingIntervall(5),
	m_observedItemTypes(OI_ALL),
	m_observedChanges(OC_ALL)
{
}


// reimüplemented (ihotf::IDirectoryMonitorParams)

double CDirectoryMonitorParams::GetPoolingIntervall() const
{
	return m_poolingIntervall;
}


void CDirectoryMonitorParams::SetPoolingIntervall(double poolingIntervall)
{
	if (m_poolingIntervall != poolingIntervall){
		istd::CChangeNotifier changePtr(this);

		m_poolingIntervall = poolingIntervall;
	}
}


int CDirectoryMonitorParams::GetObservedItemTypes() const
{
	return m_observedItemTypes;
}


void CDirectoryMonitorParams::SetObservedItemTypes(int observedItemTypes)
{
	if (m_observedItemTypes != observedItemTypes){
		istd::CChangeNotifier changePtr(this);

		m_observedItemTypes = observedItemTypes;
	}
}


int CDirectoryMonitorParams::GetObservedChanges() const
{
	return m_observedChanges;
}


void CDirectoryMonitorParams::SetObservedChanges(int ovservedChanges)
{
	if (m_observedChanges != ovservedChanges){
		istd::CChangeNotifier changePtr(this);

		m_observedChanges = ovservedChanges;
	}
}


QStringList CDirectoryMonitorParams::GetAcceptPatterns() const
{
	return m_acceptPatterns;
}


void CDirectoryMonitorParams::SetAcceptPatterns(const QStringList& acceptPatterns)
{
	if (acceptPatterns != m_acceptPatterns){
		istd::CChangeNotifier changePtr(this);

		m_acceptPatterns = acceptPatterns;
	}
}



QStringList CDirectoryMonitorParams::GetIgnorePatterns() const
{
	return m_ignorePatterns;
}


void CDirectoryMonitorParams::SetIgnorePatterns(const QStringList& ignorePatterns)
{
	if (ignorePatterns != m_acceptPatterns){
		istd::CChangeNotifier changePtr(this);

		m_ignorePatterns = ignorePatterns;
	}
}


// reimplemented (iser::ISerializable)

bool CDirectoryMonitorParams::Serialize(iser::IArchive& archive)
{		
	static iser::CArchiveTag poolingIntervallTag("PoolingIntervall", "Intervall for state update by pooling of file system infos", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag observedItemTypesTag("ObservedItemTypes", "Item types to be observed", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag observedChangesTag("ObservedChanges", "Changes in file system to be observed", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag acceptPatternsTag("AcceptPatterns", "List of accepted file name patterns", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag acceptPatternTag("AcceptPattern", "Single accepted file name pattern", iser::CArchiveTag::TT_LEAF, &acceptPatternsTag);
	static iser::CArchiveTag ignorePatternsTag("IgnorePatterns", "List of ingored file name patterns", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag ignorePatternTag("IgnorePattern", "Single ignored file name pattern", iser::CArchiveTag::TT_LEAF, &ignorePatternsTag);

	istd::CChangeNotifier changePtr(!archive.IsStoring()? this : NULL);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(poolingIntervallTag);
	retVal = retVal && archive.Process(m_poolingIntervall);
	retVal = retVal && archive.EndTag(poolingIntervallTag);

	retVal = retVal && archive.BeginTag(observedItemTypesTag);
	retVal = retVal && archive.Process(m_observedItemTypes);
	retVal = retVal && archive.EndTag(observedItemTypesTag);

	retVal = retVal && archive.BeginTag(observedChangesTag);
	retVal = retVal && archive.Process(m_observedChanges);
	retVal = retVal && archive.EndTag(observedChangesTag);

	int acceptPatternsCount = m_acceptPatterns.size();
	retVal = retVal && archive.BeginMultiTag(acceptPatternsTag, acceptPatternTag, acceptPatternsCount);
	if (archive.IsStoring()){
		for (int acceptPatternIndex = 0; acceptPatternIndex < acceptPatternsCount; acceptPatternIndex++){
			retVal = retVal && archive.BeginTag(acceptPatternTag);
			retVal = retVal && archive.Process(m_acceptPatterns[acceptPatternIndex]);
			retVal = retVal && archive.EndTag(acceptPatternTag);		
		}
	}
	else{
		m_acceptPatterns.clear();

		for (int acceptPatternIndex = 0; acceptPatternIndex < acceptPatternsCount; acceptPatternIndex++){
			QString acceptPattern;
			retVal = retVal && archive.BeginTag(acceptPatternTag);
			retVal = retVal && archive.Process(acceptPattern);
			retVal = retVal && archive.EndTag(acceptPatternTag);

			if (retVal){
				m_acceptPatterns.push_back(acceptPattern);
			}
		}
	}

	retVal = retVal && archive.EndTag(acceptPatternsTag);

	int ignorePatternsCount = m_ignorePatterns.size();
	retVal = retVal && archive.BeginMultiTag(ignorePatternsTag, ignorePatternTag, ignorePatternsCount);
	if (archive.IsStoring()){
		for (int ignorePatternIndex = 0; ignorePatternIndex < ignorePatternsCount; ignorePatternIndex++){
			retVal = retVal && archive.BeginTag(ignorePatternTag);
			retVal = retVal && archive.Process(m_ignorePatterns[ignorePatternIndex]);
			retVal = retVal && archive.EndTag(ignorePatternTag);		
		}
	}
	else{
		m_ignorePatterns.clear();

		for (int ignorePatternIndex = 0; ignorePatternIndex < ignorePatternsCount; ignorePatternIndex++){
			QString ignorePattern;
			retVal = retVal && archive.BeginTag(ignorePatternTag);
			retVal = retVal && archive.Process(ignorePattern);
			retVal = retVal && archive.EndTag(ignorePatternTag);

			if (retVal){
				m_ignorePatterns.push_back(ignorePattern);
			}
		}	
	}

	retVal = retVal && archive.EndTag(ignorePatternsTag);

	return retVal;
}


} // namespace ihotf


