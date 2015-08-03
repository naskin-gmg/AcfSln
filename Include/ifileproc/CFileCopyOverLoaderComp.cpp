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


#include "ifileproc/CFileCopyOverLoaderComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>


namespace ifileproc
{


// reimplemented (ifileproc::IFileConversion)

int CFileCopyOverLoaderComp::ConvertFiles(
			const QString& inputPath,
			QString& outputPath,
			const iprm::IParamsSet* /*paramsSetPtr*/,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (!m_inputLoaderCompPtr.IsValid()){
		SendErrorMessage(0, "Input data loader is not defined", "FileCopyOverLoader");

		return iproc::IProcessor::TS_INVALID;
	}

	if (!m_outputLoaderCompPtr.IsValid()){
		SendErrorMessage(0, "Output data loader is not defined", "FileCopyOverLoader");

		return iproc::IProcessor::TS_INVALID;
	}

	if (!m_objectCompPtr.IsValid()){
		SendErrorMessage(0, "Data object for copy operaration is not set", "FileCopyOverLoader");

		return iproc::IProcessor::TS_INVALID;
	}

	QString usedOutputPath = outputPath;

	if (usedOutputPath.isEmpty()){
		QStringList extensions;
		m_outputLoaderCompPtr->GetFileExtensions(extensions);

		if (extensions.isEmpty()){
			SendErrorMessage(0, "File extension list is empty", "FileCopyOverLoader");
	
			return iproc::IProcessor::TS_INVALID;
		}

		int pointPos = inputPath.lastIndexOf(".");
		if (pointPos != -1){
			usedOutputPath = inputPath.left(pointPos + 1) + extensions.front();
		}
		else{
			usedOutputPath = inputPath + extensions.front();
		}
	}

	QFileInfo inputFileInfo(inputPath);
	if (!inputFileInfo.exists()){
		SendWarningMessage(0, QString("Input file %1 doesn't exist").arg(inputPath));
	}

	int loadState = m_inputLoaderCompPtr->LoadFromFile(*m_objectCompPtr, inputPath);
	if (loadState != ifile::IFilePersistence::OS_OK){
		SendErrorMessage(0, "Data could not be loaded", "FileCopyOverLoader");

		return iproc::IProcessor::TS_INVALID;
	}

	int saveState = m_outputLoaderCompPtr->SaveToFile(*m_objectCompPtr, usedOutputPath);
	if (saveState != ifile::IFilePersistence::OS_OK){
		SendErrorMessage(0, "Data could not be saved", "FileCopyOverLoader");

		return iproc::IProcessor::TS_INVALID;
	}

	return iproc::IProcessor::TS_OK;
}


} // namespace ifileproc


