#include <icomm/CResponsesManagerBase.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <icomm/IArchiveEx.h>
#include <icomm/CRequestHeader.h>
#include <icomm/CResponseHeader.h>


namespace icomm
{


CResponsesManagerBase::~CResponsesManagerBase()
{
	Reset();
}


void CResponsesManagerBase::Reset()
{
	ProvidersMap::iterator iterator = m_providersMap.begin();
	while (iterator != m_providersMap.end()){
		IResponse* providerPtr = iterator.value();
		Q_ASSERT(providerPtr != NULL);

		providerPtr->OnDetached(this);

		++iterator;
	}
	m_providersMap.clear();
}



bool CResponsesManagerBase::OnReceived(iser::IArchive& request, iser::IArchive& answer)
{
	Q_ASSERT(!request.IsStoring());
	Q_ASSERT(answer.IsStoring());

	CRequestHeader requestHeader;
	bool result = requestHeader.Serialize(request);

	if (result){
		CResponseHeader responseHeader;
		ProvidersMap::iterator iterator = m_providersMap.find(requestHeader.GetCommand());
		if (iterator != m_providersMap.end()){
			responseHeader.SetToKeepAliveMessage(requestHeader);
			result = result && responseHeader.Serialize(answer);
			SendAndClear(answer);

			IResponse* providerPtr = iterator.value();
			Q_ASSERT(providerPtr != NULL);

			responseHeader.SetOk(requestHeader);
			result = result && responseHeader.Serialize(answer);

			result = result && providerPtr->DoResponse(request, answer);
			if (!result){
				IArchiveEx* archivePtr = dynamic_cast<IArchiveEx*>(&answer);
				if (archivePtr != NULL){
					if (archivePtr->Clear()){
						static QString unableCall(QObject::tr("UnableCall"));

						responseHeader.SetError(CResponseHeader::ET_OPERATION, requestHeader, unableCall, false);

						result = responseHeader.Serialize(answer);
					}
				}
			}
		}
		else{
			static QString unknown(QObject::tr("UnknownCommand"));

			responseHeader.SetError(CResponseHeader::ET_OPERATION, requestHeader, unknown, false);

			result = responseHeader.Serialize(answer);
		}
	}

	if (result){
		SendAndClear(answer);
	}

	return result;
}



// reimplemented (icomm::IResponsesManager)

void CResponsesManagerBase::ConnectProvider(IResponse* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

	const QByteArray& command = providerPtr->GetCommand();

	ProvidersMap::iterator iterator = m_providersMap.find(command);
	Q_ASSERT(iterator == m_providersMap.end());

	m_providersMap[command] = providerPtr;
}



void CResponsesManagerBase::DisconnectProvider(IResponse* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

	const QByteArray& command = providerPtr->GetCommand();

	ProvidersMap::iterator iterator = m_providersMap.find(command);
	Q_ASSERT(iterator != m_providersMap.end());

	m_providersMap.erase(iterator);
}



} // namespace icomm



