#ifndef ifileproc_IFileNaming_included
#define ifileproc_IFileNaming_included


// ACF includes
#include "istd/IPolymorphic.h"


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
				const ifileproc::IFileNamingParams* fileNamingParamsPtr) const = 0;
};


} // namespace ifileproc


#endif // !ifileproc_IFileNaming_included


