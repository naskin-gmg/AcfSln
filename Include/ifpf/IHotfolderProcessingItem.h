#ifndef ifpf_IHotfolderProcessingItem_included
#define ifpf_IHotfolderProcessingItem_included


// ACF includes
#include "istd/CString.h"

#include "iser/ISerializable.h"

#include "iimg/CGeneralBitmap.h"



namespace ifpf
{


/**
	Interface for a single processing item in the hotfolder.
*/
class IHotfolderProcessingItem: virtual public iser::ISerializable
{
public:	
	virtual int GetProcessingState() const = 0;
	virtual void SetProcessingState(int processingState) = 0;

	virtual double GetProgress() const = 0;
	virtual void SetProgress(double progress) = 0;
	
	virtual istd::CString GetInputFile() const = 0;
	virtual void SetInputFile(const istd::CString& inputFile) = 0;
	
	virtual istd::CString GetOutputFile() const = 0;
	virtual void SetOutputFile(const istd::CString& outputFile) = 0;

	virtual const iimg::IBitmap& GetInputPreview() const = 0;
	virtual void SetInputPreview(const iimg::IBitmap& inputPreview) = 0;

	virtual const iimg::IBitmap& GetOutputPreview() const = 0;
	virtual void SetOutputPreview(const iimg::IBitmap& outputPreview) = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderProcessingItem_included


