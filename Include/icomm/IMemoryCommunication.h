#pragma once


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <istd/IPolymorphic.h>


namespace icomm
{

	
/**
	Interface for communication over memory blocks.
	There are two separated data blocks: for input and for output.
	This blocks has some fixed size.
*/
class IMemoryCommunication: virtual public istd::IPolymorphic
{
public:
	/**
		Interface for event handler.
	*/
	class Handler: virtual public istd::IPolymorphic
	{
	public:
		/**
			Called when input memory block was changed.
		*/
		virtual void OnInputBlockChanged(IMemoryCommunication& communication, int firstByte, int bytesCount) = 0;
	};

	/**
		Get size of input memory block.
	*/
	virtual int GetInputBlockSize() const = 0;

	/**
		Set size of input memory block.
	*/
	virtual void SetInputBlockSize(int size) = 0;

	/**
		Get size of output memory block.
	*/
	virtual int GetOutputBlockSize() const = 0;

	/**
		Set size of output memory block.
	*/
	virtual void SetOutputBlockSize(int size) = 0;

	/**
		Read new incoming data.
		\param	data	pointer to data buffer.
		\param	size	as input provide size of data buffer, as output provide number of bytes read from communication.
		\return	true if data available.
	*/
	virtual bool GetInputBlock(int offset, int size, void* dataPtr) const = 0;

	/**
		Set output memory block.
		\param	data	pointer to data block.
		\param	size	size of data block in bytes.
		\param	offset	offset in memory block.
		\param	doSend	if true, memory changes will be sent immediately.
		\return true if data could be sent successful.
	*/
	virtual bool SetOutputBlock(int offset, int size, const void* dataPtr, bool doSend = true) = 0;

	/**
		Connect handler for receive event.
		\param	handlerPtr	Pointer to user events handler.
		\param	allowOtherThread	if true, it allows calls of handler in other than current caller thread.
		\note If \c allowOtherThread == false then only when first CommandsBlockSize bytes are changed this event handler will be called. This is an optimization to avoid cross threading calls for each robot position.
	*/
	virtual bool RegisterEventHandler(Handler* handlerPtr, bool allowOtherThread = false) = 0;

	/**
		Disconnect handler connected with \c RegisterEventHandler.
		\param	handlerPtr	Pointer to user events handler.
	*/
	virtual bool UnregisterEventHandler(Handler* handlerPtr) = 0;
};


} // namespace icomm


