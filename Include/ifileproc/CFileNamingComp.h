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


#ifndef ifileproc_CFileNamingComp_included
#define ifileproc_CFileNamingComp_included


// ACF includes
#include "ifile/IFilePersistence.h"

#include "ilog/TLoggerCompWrap.h"

#include "ifile/IFileNameParam.h"


// AcfSln includes
#include "ifileproc/IFileNaming.h"
#include "ifileproc/IFileNamingParams.h"


namespace ifileproc
{


/**
	Component for calculating of the new file name.
	\sa ifileproc::IFileNaming
*/
class CFileNamingComp:
			public ilog::CLoggerComponentBase,
			virtual public ifileproc::IFileNaming
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileNamingComp);
		I_REGISTER_INTERFACE(ifileproc::IFileNaming);
		I_ASSIGN(m_fileTypeInfoCompPtr, "OutputFileInfo", "Output file info, which will be used for file name calculation", false, "OutputFileInfo");
		I_ASSIGN(m_directoryPathCompPtr, "DirectoryPath", "Default path of the output directory", false, "DirectoryPath"); 
		I_ASSIGN(m_fileNamingParamsCompPtr, "FileNamingParams", "Rules for the file naming", false, "FileNamingParams"); 
	I_END_COMPONENT;

	// reimplemented (ifileproc::IFileNaming)
	virtual QString CalculateFileName(
				const QString& inputFileName,
				const QString& outputDirectoryPath,
				const ifileproc::IFileNamingParams* fileNamingParamsPtr) const;

private:
	I_REF(ifile::IFileTypeInfo, m_fileTypeInfoCompPtr);
	I_REF(ifile::IFileNameParam, m_directoryPathCompPtr);
	I_REF(ifileproc::IFileNamingParams, m_fileNamingParamsCompPtr);
};


} // namespace ifileproc


#endif // !ifileproc_CFileNamingComp_included

