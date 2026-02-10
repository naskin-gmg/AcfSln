// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef ifileproc_IFileNaming_included
#define ifileproc_IFileNaming_included


// ACF includes
#include <istd/IPolymorphic.h>


namespace ifileproc
{


class IFileNamingParams;


/**
	Interface for calculation of the new file path for an existing file.
*/
class IFileNaming: virtual public istd::IPolymorphic
{
public:
	/**
		Calculate the new file path for a given input file name.
	*/
	virtual QString CalculateFileName(
				const QString& inputFileName,
				const QString& outputDirectoryPath,
				const ifileproc::IFileNamingParams* fileNamingParamsPtr) const = 0;
};


} // namespace ifileproc


#endif // !ifileproc_IFileNaming_included


