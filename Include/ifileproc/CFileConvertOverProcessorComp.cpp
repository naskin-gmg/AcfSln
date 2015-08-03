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


#include "ifileproc/CFileConvertOverProcessorComp.h"


// Qt includes
#include <QtCore/QMutexLocker>


namespace ifileproc
{


// public methods

// reimplemented (ifileproc::IFileConversion)

int CFileConvertOverProcessorComp::ConvertFiles(
			const QString& inputPath,
			QString& outputPath,
			const iprm::IParamsSet* paramsSetPtr,
			ibase::IProgressManager* progressManagerPtr) const
{
	QMutexLocker blocker(&m_mutex);

	if (!m_inputFileLoaderCompPtr.IsValid()){
		SendErrorMessage(0, "No file loader defined", "File processing component");

		return iproc::IProcessor::TS_INVALID;
	}

	if (!m_inputDataCompPtr.IsValid()){
		SendErrorMessage(0, "Input data definition was not set", "File processing component");

		return iproc::IProcessor::TS_INVALID;
	}

	if (!m_outputDataCompPtr.IsValid()){
		SendErrorMessage(0, "Output data definition was not set", "File processing component");

		return iproc::IProcessor::TS_INVALID;
	}

	if (!m_inputFileLoaderCompPtr->IsOperationSupported(
					m_inputDataCompPtr.GetPtr(),
					&inputPath,
					ifile::IFilePersistence::QF_LOAD | ifile::IFilePersistence::QF_FILE)){
		SendErrorMessage(0, "File could not be loaded", "File processing component");
		
		return iproc::IProcessor::TS_INVALID;
	}

	if (m_inputFileLoaderCompPtr->LoadFromFile(*m_inputDataCompPtr.GetPtr(), inputPath) != ifile::IFilePersistence::OS_OK){
		SendErrorMessage(0, "File could not be loaded", "File processing component");
		
		return iproc::IProcessor::TS_INVALID;
	}

	if (!m_processorCompPtr.IsValid()){
		SendErrorMessage(0, "File processor not set", "File processing component");

		return iproc::IProcessor::TS_INVALID;
	}

	const iprm::IParamsSet* processingParamsPtr = (paramsSetPtr != NULL) ? paramsSetPtr : m_processingParamsSetCompPtr.GetPtr();
	ibase::IProgressManager* processingProgressManagerPtr = (progressManagerPtr != NULL) ? progressManagerPtr : m_progressManagerCompPtr.GetPtr();

	int processingResult = m_processorCompPtr->DoProcessing(
				processingParamsPtr,
				m_inputDataCompPtr.GetPtr(),
				m_outputDataCompPtr.GetPtr(),
				processingProgressManagerPtr);
	if (processingResult != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "File conversion failed", "File processing component");

		return processingResult;
	}

	ifile::IFilePersistence* outputFileLoader = m_inputFileLoaderCompPtr.GetPtr();
	if (m_outputFileLoaderCompPtr.IsValid()){
		outputFileLoader = m_outputFileLoaderCompPtr.GetPtr(); 
	}

	Q_ASSERT(outputFileLoader != NULL);

	return (outputFileLoader->SaveToFile(*m_outputDataCompPtr.GetPtr(), outputPath) == ifile::IFilePersistence::OS_OK) ? iproc::IProcessor::TS_OK : iproc::IProcessor::TS_INVALID;
}


} // namespace ifileproc


