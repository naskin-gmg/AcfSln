#pragma once


// ACF includes
#include <iprm/IParamsSet.h>


namespace icam
{


/**
	Allows to perform additional actions on the camera.
	Typically, in order to execute camera command,
	you need to call camera's DoProcessing() with ICameraCommand implementation
	as input parameter.
*/
class ICameraCommand: virtual public istd::IPolymorphic
{
public:
	enum CommandIds
	{
		CI_RESET_BUFFERS = 0,			// Force reset of image buffers
		CI_RESCAN_CAMERAS,
		CI_READ_SETTINGS,
		CI_STORE_SETTINGS,
		CI_SET_PARAMETERS,
		CI_START_ACQUISITION,
		CI_STOP_ACQUISITION,
		CI_READ_BUFFER,
		CI_READ_LAST_IN_BUFFER,
		CI_USER = 1024					// User defined
	};

	enum CommandStatus
	{
		CS_NONE = 0,			// no command recognized
		CS_OK,					// command executed successfully
		CS_ERROR,				// command executed with errors
		CS_UNKNOWN				// unknown command
	};

	/**
		Returns ID of the command.
	*/
	virtual int GetCommandId() const = 0;
};


} // namespace qcam


