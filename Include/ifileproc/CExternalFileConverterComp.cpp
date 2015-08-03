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


#include "ifileproc/CExternalFileConverterComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

// ACF includes
#include "iprm/TParamsPtr.h"


namespace ifileproc
{


// reimplemented (ifileproc::IFileConversion)

int CExternalFileConverterComp::ConvertFiles(
			const QString& inputPath,
			QString& outputPath,
			const iprm::IParamsSet* paramsSetPtr,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (!m_executablePathCompPtr.IsValid()){
		SendErrorMessage(0, "Path for an executable was not set");

		return iproc::IProcessor::TS_INVALID;
	}

	if (m_executablePathCompPtr->GetPathType() != ifile::IFileNameParam::PT_FILE){
		SendErrorMessage(0, "Wrong executable path type. Must be path to a file.");

		return iproc::IProcessor::TS_INVALID;
	}

	QStringList arguments;

	// setup command line arguments:
	if (!m_defaultProcessArgumentsAttrPtr.IsValid()){
		if (paramsSetPtr != NULL){
			iprm::TParamsPtr<iprm::INameParam> commandLineParamPtr(paramsSetPtr, *m_processArgumentsParamsIdAttrPtr);
			if (commandLineParamPtr.IsValid()){
				arguments = commandLineParamPtr->GetName().split(" ");
			}
		}
		else{
			arguments.push_back(inputPath);
			arguments.push_back(outputPath);
		}
	}
	else{
		QString applicationArguments = *m_defaultProcessArgumentsAttrPtr;

		arguments = applicationArguments.split(" ");
	}

	QFileInfo outputFileInfo(outputPath);

	QString outputDirectory = outputFileInfo.dir().absolutePath();

	for (int argIndex = 0; argIndex < int(arguments.size()); argIndex++){
		arguments[argIndex].replace("$(Input)", inputPath);
		arguments[argIndex].replace("$(Output)", outputPath);
		arguments[argIndex].replace("$(OutputDir)", outputDirectory);
	}

	if (m_additionalArgumentsCompPtr.IsValid()){
		int additionalArgumentsCount = m_additionalArgumentsCompPtr.GetCount();

		for (int addIndex = 0; addIndex < additionalArgumentsCount; addIndex++){
			iprm::INameParam* argumentPtr = m_additionalArgumentsCompPtr[addIndex];
			if (argumentPtr != NULL){
				QString argument = argumentPtr->GetName();

				bool placeHolderFound = false;
				for (int argIndex = 0; argIndex < int(arguments.size()); argIndex++){
					QString& existingArgument = arguments[argIndex];
					QString expressionString = QString("$(A%1)").arg(addIndex + 1);

					iprm::INameParam* additionalArgumentPtr = m_additionalArgumentsCompPtr[addIndex];
					if (additionalArgumentPtr != NULL){
						QString replacement = additionalArgumentPtr->GetName();

						if (existingArgument.contains(expressionString)){
							existingArgument.replace(expressionString, replacement);
							
							placeHolderFound = true;
						}
					}
				}

				if (!placeHolderFound){
					arguments.push_back(argument);
				}
			}
		}
	}

	SendVerboseMessage(QString("Process started as %1 %2").arg(m_executablePathCompPtr->GetPath()).arg(arguments.join(" ")));

	m_conversionProcess.start(m_executablePathCompPtr->GetPath(), arguments);

	m_conversionProcess.waitForFinished(-1);

	if (m_conversionProcess.error() != QProcess::UnknownError){
		return iproc::IProcessor::TS_INVALID;
	}

	return (m_conversionProcess.exitCode() == 0) ? iproc::IProcessor::TS_OK : iproc::IProcessor::TS_INVALID;
}


// protected methods

// reimplemented (icomp::CComponentBase)
	
void CExternalFileConverterComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	connect(&m_conversionProcess, SIGNAL(readyReadStandardError()), this, SLOT(OnReadyReadStandardError()));
	connect(&m_conversionProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(OnReadyReadStandardOutput()));
}


// private slots

void CExternalFileConverterComp::OnReadyReadStandardError()
{
	QString errorOutput = m_conversionProcess.readAllStandardError();
	
	errorOutput = errorOutput.simplified();

	SendErrorMessage(0, errorOutput);
}


void CExternalFileConverterComp::OnReadyReadStandardOutput()
{
	QString standardOutput = m_conversionProcess.readAllStandardOutput();

	standardOutput = standardOutput.simplified();

	SendInfoMessage(0, standardOutput);
}



} // namespace ifileproc


