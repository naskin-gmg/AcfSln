#ifndef ifpf_IHotfolderProcessingItem_included
#define ifpf_IHotfolderProcessingItem_included


// ACF includes
#include "istd/CString.h"

#include "iser/ISerializable.h"

#include "iimg/IBitmap.h"



namespace ifpf
{


/**
	Interface for a single processing item in the hotfolder.
*/
class IHotfolderProcessingItem: virtual public iser::ISerializable
{
public:

	enum ChangeFlags
	{
		CF_STATE_CHANGED = 0x1000000,
		CF_PROGRESS_CHANGED = 0x2000000,
		CF_INPUT_PREVIEW_CHANGED = 0x4000000,
		CF_OUTPUT_PREVIEW_CHANGED = 0x8000000
	};

	/**
		Get the processing state for the item.

		\sa iproc::IProcessor::TaskState
	*/
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


