#ifndef ifileproc_IFileConvertCopy_included
#define ifileproc_IFileConvertCopy_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "iprm/IParamsSet.h"
#include "ibase/IProgressManager.h"


namespace ifileproc
{


/**
	General interface for conversion of files.
*/
class IFileConversion: virtual public istd::IPolymorphic
{
public:
	/**
		Types of messages which can be produced during execution of ConvertFiles method.
	*/
	enum MessageId
	{
		/**
			Message ID if input path is invalid
		*/
		MI_INPUT_PATH = 0x5cd40,
		/**
			Message ID if output path is invalid
		*/
		MI_OUTPUT_PATH
	};

	/**
		Copy a file or files from \c inputPath to destination \c outputPath.

		\param inputPath	Path to the input file or directory.
		\param outputPath	Path to the output file or directory.
		\param paramsPtr	File conversion parameters
		\param progressManagerPtr	Progress manager used for getting information about file conversion progress state and canceling.

		\return \c true if the file conversion was successful or \c false otherwise.
	*/
	virtual bool ConvertFiles(
				const QString& inputPath,
				const QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL,
				ibase::IProgressManager* progressManagerPtr = NULL) const = 0;
};


} // namespace ifileproc


#endif // !ifileproc_IFileConvertCopy_included


