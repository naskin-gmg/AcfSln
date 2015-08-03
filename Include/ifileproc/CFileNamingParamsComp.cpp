/********************************************************************************
**
**	Copyright (c) 2007-2014 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF-Solutions Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org, write info@imagingtools.de or contact
**	by Skype to ACF_infoline for further information about the ACF-Solutions.
**
********************************************************************************/


// AcfSln includes
#include "ifileproc/CFileNamingParamsComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ifileproc
{


CFileNamingParamsComp::CFileNamingParamsComp()
	:m_overwriteStrategy(RM_NUMBERING)
{
}

// reimplemented (ifileproc::IFileNamingParams)

CFileNamingParamsComp::OverwriteStrategy CFileNamingParamsComp::GetOverwriteStrategy() const
{
	return m_overwriteStrategy;
}


void CFileNamingParamsComp::SetOverwriteStrategy(OverwriteStrategy overwriteStrategy)
{
	if (m_overwriteStrategy != overwriteStrategy){
		istd::CChangeNotifier changePtr(this);

		m_overwriteStrategy = overwriteStrategy;
	}
}


QString CFileNamingParamsComp::GetPrefix() const
{
	return m_prefix;
}


void CFileNamingParamsComp::SetPrefix(const QString& prefix)
{
	if (m_prefix != prefix){
		istd::CChangeNotifier changePtr(this);

		m_prefix = prefix;
	}
}


QString CFileNamingParamsComp::GetSuffix() const
{
	return m_suffix;
}


void CFileNamingParamsComp::SetSuffix(const QString& suffix)
{
	if (m_suffix != suffix){
		istd::CChangeNotifier changePtr(this);

		m_suffix = suffix;
	}
}


QStringList CFileNamingParamsComp::GetPatternsToRemove() const
{
	return m_patternsToRemove;
}


void CFileNamingParamsComp::SetPatternsToRemove(const QStringList& patternsToRemove)
{
	if (m_patternsToRemove != patternsToRemove){
		istd::CChangeNotifier changePtr(this);

		m_patternsToRemove = patternsToRemove;
	}
}


// reimplemented (iser::ISerializable)

bool CFileNamingParamsComp::Serialize(iser::IArchive& archive)
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


// protected methods

// reimplemented (icomp::CComponentBase)

void CFileNamingParamsComp::OnComponentCreated()
{
	m_prefix = *m_prefixAttrPtr;
	m_suffix = *m_suffixAttrPtr;
	m_overwriteStrategy = OverwriteStrategy(*m_overwriteStrategyAttrPtr);

	BaseClass::OnComponentCreated();
}


} // namespace ifileproc


