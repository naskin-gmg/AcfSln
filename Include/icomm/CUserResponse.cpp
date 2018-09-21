#include <icomm/CUserResponse.h>


// ACF includes
#include <istd/istd.h>


namespace icomm
{


CUserResponse::CUserResponse(const QByteArray& command, iser::ISerializable* slavePtr)
:	m_command(command),
	m_slavePtr(slavePtr),
	m_managerPtr(NULL)
{
}


// reimplemented (icomm::IResponse)

QByteArray CUserResponse::GetCommand() const
{
	return m_command;
}


bool CUserResponse::DoResponse(iser::IArchive& request, iser::IArchive& response)
{
	Q_UNUSED(request);
	Q_ASSERT(!request.IsStoring());
	Q_ASSERT(response.IsStoring());

	return m_slavePtr->Serialize(response);
}


bool CUserResponse::OnAttached(IResponsesManager* managerPtr)
{
	Q_ASSERT(managerPtr != NULL);

	if (m_managerPtr == NULL){
		m_managerPtr = managerPtr;

		return true;
	}

	return false;
}


bool CUserResponse::OnDetached(IResponsesManager* managerPtr)
{
	Q_ASSERT(managerPtr != NULL);

	if (m_managerPtr == managerPtr){
		m_managerPtr = NULL;
		return true;
	}

	return false;
}


} // namespace icomm


