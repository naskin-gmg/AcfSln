#ifndef ifileproc_IFileNamingParams_included
#define ifileproc_IFileNamingParams_included


// ACF includes
#include "iser/ISerializable.h"


namespace ifileproc
{


/**
	Interface for calculation parameters of the new file name for any file item.
	\sa IFileNaming
*/
class IFileNamingParams: virtual public iser::ISerializable
{
public:
	/**
		Strategy mode
	*/
	enum OverwriteStrategy
	{
		/**
			Output file name is equals the input file name.
		*/
		RM_OVERWRITE,

		/**
			Output file name will be automatic numbered if the file already exists.
		*/
		RM_NUMBERING
	};

	/**
		Get used strategy for possible overwriting of existing files.
		\sa OverwriteStrategy
	*/
	virtual OverwriteStrategy GetOverwriteStrategy() const = 0;

	/**
		Set renaming mode.
		\sa OverwriteStrategy
	*/
	virtual void SetOverwriteStrategy(OverwriteStrategy overwriteStrategy) = 0;

	/**
		Get prefix of the file
	*/
	virtual QString GetPrefix() const = 0;

	/**
		Set prefix of the file
	*/
	virtual void SetPrefix(const QString& prefix) = 0;

	/**
		Set suffix of the file
	*/
	virtual QString GetSuffix() const = 0;

	/**
		Get suffix of the file
	*/
	virtual void SetSuffix(const QString& suffix) = 0;

	/**
		Get list of text patterns, that must be removed from the output file name.
		\sa SetPatternsToRemove
	*/
	virtual QStringList GetPatternsToRemove() const = 0;
	
	/**
		Set list of text patterns, that must be removed from the output file name.
		For example, the file name \c foo_some.txt will be transformed into \c foo.txt if \c _some pattern is set.
		\sa GetPatternsToRemove
	*/
	virtual void SetPatternsToRemove(const QStringList& patternsToRemove) = 0;
};


} // namespace ifileproc


#endif // !ifileproc_IFileNamingParams_included


