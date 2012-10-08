#ifndef ihotf_IHotfolderProcessingItem_included
#define ihotf_IHotfolderProcessingItem_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QDateTime.h>

// ACF includes
#include "iser/ISerializable.h"


namespace ihotf
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
	virtual QByteArray GetItemUuid() const = 0;

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
	virtual QString GetInputFile() const = 0;

	/**
		Set input file path.
	*/
	virtual void SetInputFile(const QString& inputFile) = 0;
	
	/**
		Get file path of the processing output.
	*/
	virtual QString GetOutputFile() const = 0;

	/**
		Set file path of the processing output.
	*/
	virtual void SetOutputFile(const QString& outputFile) = 0;

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
	virtual const QDateTime& GetStartTime() const = 0;

	/**
		Set start time of the job.
	*/
	virtual void SetStartTime(const QDateTime& startTime) = 0;
};


} // namespace ihotf


#endif // !ihotf_IHotfolderProcessingItem_included


