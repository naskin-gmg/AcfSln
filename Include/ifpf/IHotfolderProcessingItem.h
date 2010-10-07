#ifndef ifpf_IHotfolderProcessingItem_included
#define ifpf_IHotfolderProcessingItem_included


// ACF includes
#include "istd/CString.h"

#include "isys/IDateTime.h"

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

	/**
		Get processing time.
	*/
	virtual double GetProcessingTime() const = 0;

	/**
		Set processing time.
	*/
	virtual void SetProcessingTime(double processingTime) = 0;

	/**
		Get start time of the job.
	*/
	virtual const isys::IDateTime& GetStartTime() const = 0;

	/**
		Set start time of the job.
	*/
	virtual void SetStartTime(const isys::IDateTime& startTime) = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderProcessingItem_included


