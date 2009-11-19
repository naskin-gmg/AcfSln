#ifndef ifpf_IHotfolderParams_included
#define ifpf_IHotfolderParams_included


// ACF includes
#include "istd/INamed.h"


namespace ifpf
{


/**
	Interface for a hotfolder parameters.
*/
class IHotfolderParams: virtual public istd::INamed
{
public:
	/**
		Get observed input pathes.
	*/
	virtual istd::CStringList GetInputPathes() const = 0;

	/**
		Set input pathes, to be observed.
	*/
	virtual void SetInputPathes(const istd::CStringList& inputPathes) = 0;

	/**
		Set output path for a given processing state.
	*/
	virtual void SetOutputPath(int processingState, const istd::CString& outputPath) = 0;

	/**
		Get an output path for a given processing state.
	*/
	virtual istd::CString GetOutputPath(int processingState) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderParams_included


