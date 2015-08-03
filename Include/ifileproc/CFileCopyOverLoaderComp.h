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


#ifndef ifileproc_CFileCopyOverLoaderComp_included
#define ifileproc_CFileCopyOverLoaderComp_included


#include "istd/IChangeable.h"

#include "ifile/IFilePersistence.h"

#include "ifileproc/IFileConversion.h"
#include "ilog/TLoggerCompWrap.h"


namespace ifileproc
{


/**
	General file copy or transformation.
	This implementation uses general file loader, saver and any object instance.
*/
class CFileCopyOverLoaderComp:
			public ilog::CLoggerComponentBase,
			virtual public IFileConversion
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileCopyOverLoaderComp);
		I_REGISTER_INTERFACE(IFileConversion);

		I_ASSIGN(m_objectCompPtr, "Object", "Object used as representation of copied data", true, "Object");
		I_ASSIGN(m_inputLoaderCompPtr, "InputLoader", "input file loader", true, "InputLoader");
		I_ASSIGN(m_outputLoaderCompPtr, "OutputLoader", "output file loader", true, "OutputLoader");
	I_END_COMPONENT;

	// reimplemented (ifileproc::IFileConversion)
	virtual int ConvertFiles(
				const QString& inputPath,
				QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL,
				ibase::IProgressManager* progressManagerPtr = NULL) const;

private:
	I_REF(istd::IChangeable, m_objectCompPtr);
	I_REF(ifile::IFilePersistence, m_inputLoaderCompPtr);
	I_REF(ifile::IFilePersistence, m_outputLoaderCompPtr);
};


} // namespace ifileproc


#endif // !ifileproc_CFileCopyOverLoaderComp_included


