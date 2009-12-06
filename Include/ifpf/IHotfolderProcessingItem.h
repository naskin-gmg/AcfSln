#ifndef ifpf_IHotfolderProcessingItem_included
#define ifpf_IHotfolderProcessingItem_included


// ACF includes
#include "istd/CString.h"

#include "iser/ISerializable.h"


namespace ifpf
{


/**
	Interface for a single processing item in the hotfolder.
*/
class IHotfolderProcessingItem: virtual public iser::ISerializable
{
public:	
	virtual int GetProcessingState() const = 0;
	virtual double GetProgress() const = 0;
	virtual istd::CString GetInputFile() const = 0;
	virtual istd::CString GetOutputFile() const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderProcessingItem_included


