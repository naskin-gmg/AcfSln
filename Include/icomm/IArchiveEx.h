#pragma once


// ACF includes
#include <iser/IArchive.h>


namespace icomm
{


/**
	Extension of archive interface allowing free archive position access.
*/
class IArchiveEx: virtual public iser::IArchive
{
public:
	/**
		Try to clear the buffer.
		\return	true, if it was possible to clear the buffer or it was empty.
	*/
	virtual bool Clear() = 0;

	/**
		Get actual position in archive.
		Returned value can be only used to rewind archive position to the stored position.
		Please dont make any calculations using this value.
		The relation between two returned position is undefined.
		\return	position value, or -1 if this function is not supported.
	*/
	virtual long GetArchivePosition() const = 0;

	/**
		Set actual position in archive.
		Please use unchanged values returned by method GetArchivePosition().
		Please note, not all implementations supports this method. In this case false will be returned.
		\param	position	new archive position.
		\return	true if success.
	*/
	virtual bool SetArchivePosition(long position) = 0;
};


} // namespace icomm


