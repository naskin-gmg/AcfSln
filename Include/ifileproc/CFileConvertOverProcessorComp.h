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


#ifndef ifileproc_CFileConvertOverProcessorComp_included
#define ifileproc_CFileConvertOverProcessorComp_included


// Qt includes
#include <QtCore/QMutex>

// ACF includes
#include "istd/TSmartPtr.h"

#include "ifile/IFilePersistence.h"

#include "ifileproc/IFileConversion.h"
#include "ilog/TLoggerCompWrap.h"

#include "ibase/IProgressManager.h"
#include "iproc/IProcessor.h"


namespace ifileproc
{


/**
	Component for a file conversion over a data processor.
*/
class CFileConvertOverProcessorComp:
			public ilog::CLoggerComponentBase,
			virtual public ifileproc::IFileConversion
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileConvertOverProcessorComp);
		I_REGISTER_INTERFACE(ifileproc::IFileConversion);
		I_ASSIGN(m_inputFileLoaderCompPtr, "InputFileLoader", "Loader for the input file", true, "InputFileLoader");
		I_ASSIGN(m_outputFileLoaderCompPtr, "OutputFileLoader", "Persistence for the output file. If not set, the input file loader will be used", false, "OutputFileLoader");
		I_ASSIGN(m_processorCompPtr, "DataProcessor", "Processor for the file", true, "FileProcessor");
		I_ASSIGN(m_inputDataCompPtr, "InputData", "Input data definition", true, "InputData");
		I_ASSIGN(m_outputDataCompPtr, "OutputData", "Output data definition", true, "OutputData");
		I_ASSIGN(m_progressManagerCompPtr, "ProgressManager", "Processing progress observer", false, "ProgressManager");
		I_ASSIGN(m_processingParamsSetCompPtr, "ProcessingParams", "Processing parameters", false, "ProcessingParams");
	I_END_COMPONENT;

	// reimplemented (ifileproc::IFileConversion)
	virtual int ConvertFiles(
				const QString& inputPath,
				QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL,
				ibase::IProgressManager* progressManagerPtr = NULL) const;

private:
	I_REF(ifile::IFilePersistence, m_inputFileLoaderCompPtr);
	I_REF(ifile::IFilePersistence, m_outputFileLoaderCompPtr);
	I_REF(iproc::IProcessor, m_processorCompPtr);
	I_REF(istd::IChangeable, m_inputDataCompPtr);
	I_REF(istd::IChangeable, m_outputDataCompPtr);
	I_REF(ibase::IProgressManager, m_progressManagerCompPtr);
	I_REF(iprm::IParamsSet, m_processingParamsSetCompPtr);

	mutable QMutex m_mutex;
};


} // namespace ifileproc


#endif // !ifileproc_CFileConvertOverProcessorComp_included


