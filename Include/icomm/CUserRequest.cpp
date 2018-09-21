#include <icomm/CUserRequest.h>


// ACF includes
#include <iser/IArchive.h>


#include <icomm/CResponseHeader.h>


namespace icomm{


CUserRequest::CUserRequest(const QByteArray& command, ISerializable* slavePtr, double timeout)
{
	Q_ASSERT(slavePtr != NULL);

	m_command = command;
	m_slavePtr = slavePtr;
	m_timeOut = timeout;
}


// reimplemented (icomm::IRequest)

double CUserRequest::GetTimeOut() const
{
	return m_timeOut;
}


QByteArray CUserRequest::GetCommand() const
{
	return m_command;
}


QString CUserRequest::ExtractMessage(const CResponseHeader& header) const
{
	return header.GetErrorMessage();
}


// reimplemented (iser::ISerializable)

bool CUserRequest::Serialize(iser::IArchive& archive)
{
	Q_ASSERT(m_slavePtr != NULL);

	if (archive.IsStoring()){
		return true;
	}
	else{
		return m_slavePtr->Serialize(archive);
	}
}


} // namespace icomm


