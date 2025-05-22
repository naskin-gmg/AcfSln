#pragma once


// ACF includes
#include <icam/ICameraCommand.h>


namespace icam
{


/**
	Simple implementation of qcam::ICameraCommand.
*/
class CCameraCommand: virtual public icam::ICameraCommand
{
public:
	CCameraCommand(int commandId);

	// reimplemented (qcam::ICameraCommand)
	virtual int GetCommandId() const;

protected:
	int m_commandId;
};


// reimplemented (qcam::ICameraCommand)

inline int CCameraCommand::GetCommandId() const
{
	return m_commandId;
}


} // namespace qcam


