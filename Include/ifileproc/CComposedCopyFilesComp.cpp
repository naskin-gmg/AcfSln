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


#include "ifileproc/CComposedCopyFilesComp.h"


// Qt includes
#include <QtCore/QRegExp>


// ACF includes
#include "istd/CSystem.h"


namespace ifileproc
{


// reimplemented (ifileproc::IFileConversion)

int CComposedCopyFilesComp::ConvertFiles(
			const QString& inputPath,
			QString& outputPath,
			const iprm::IParamsSet* paramsPtr,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	int retVal = iproc::IProcessor::TS_NONE;

	int copiersCount = m_fileCopiersCompPtr.GetCount();
	for (int i = 0; i < copiersCount; ++i){
		ifileproc::IFileConversion* copierPtr = m_fileCopiersCompPtr[i];
		if (copierPtr != NULL){
			retVal = qMax(retVal, copierPtr->ConvertFiles(inputPath, outputPath, paramsPtr));
		}
		else{
			SendErrorMessage(MI_CRITICAL, tr("File copy provider is not present"));

			return iproc::IProcessor::TS_INVALID;
		}
	}

	return retVal;
}


} // namespace ifileproc


