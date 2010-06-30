#ifndef ifpf_IProcessingItemPreviewProvider_included
#define ifpf_IProcessingItemPreviewProvider_included


// ACF includes
#include "istd/CString.h"

#include "iser/ISerializable.h"

#include "iimg/IBitmap.h"


namespace ifpf
{


/**
	Interface for a single processing item in the hotfolder.
*/
class IProcessingItemPreviewProvider: virtual public iser::ISerializable
{
public:
	/**
		Get preview for the input file.
	*/
	virtual const iimg::IBitmap* GetInputFilePreview(const ifpf::IHotfolderProcessingItem& item, bool ensureCreated = false) const = 0;

	/**
		Get preview for the output file.
	*/
	virtual const iimg::IBitmap* GetOutputFilePreview(const ifpf::IHotfolderProcessingItem& item, bool ensureCreated = false) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IProcessingItemPreviewProvider_included


