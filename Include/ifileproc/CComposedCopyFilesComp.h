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


#ifndef ifileproc_CComposedCopyFilesComp_included
#define ifileproc_CComposedCopyFilesComp_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QDir>

// ACF includes
#include "ifileproc/IFileConversion.h"
#include "ilog/TLoggerCompWrap.h"


namespace ifileproc
{


/**
	Composed files copy.
*/
class CComposedCopyFilesComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			virtual public ifileproc::IFileConversion
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_CRITICAL = 0xa8760
	};

	I_BEGIN_COMPONENT(CComposedCopyFilesComp);
		I_REGISTER_INTERFACE(ifileproc::IFileConversion);
		I_ASSIGN_MULTI_0(m_fileCopiersCompPtr, "FileCopiers", "Set of copiers will be used for input and output path", true);
	I_END_COMPONENT;

	// reimplemented (ifileproc::IFileConversion)
	virtual int ConvertFiles(
				const QString& inputPath,
				QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL,
				ibase::IProgressManager* progressManagerPtr = NULL) const;

private:
	I_MULTIREF(ifileproc::IFileConversion, m_fileCopiersCompPtr);
};


} // namespace ifileproc


#endif // !ifileproc_CComposedCopyFilesComp_included


