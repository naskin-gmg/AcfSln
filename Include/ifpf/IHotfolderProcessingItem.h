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

	enum ChangeFlags
	{
		CF_STATE_CHANGED = 0x1000000
	};

	/**
		Return the unique identifier for a processing item.
	*/
	virtual std::string GetItemUuid() const = 0;

	/**
		Get the processing state for the item.

		\sa iproc::IProcessor::TaskState
	*/
	virtual int GetProcessingState() const = 0;

	/**
		Set the processing state for the item.

		\sa iproc::IProcessor::TaskState
	*/
	virtual void SetProcessingState(int processingState) = 0;
	
	/**
		Get file path, of the file which must be processed.
	*/
	virtual istd::CString GetInputFile() const = 0;

	/**
		Set input file path.
	*/
	virtual void SetInputFile(const istd::CString& inputFile) = 0;
	
	/**
		Get file path of the processing output.
	*/
	virtual istd::CString GetOutputFile() const = 0;

	/**
		Set file path of the processing output.
	*/
	virtual void SetOutputFile(const istd::CString& outputFile) = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderProcessingItem_included


