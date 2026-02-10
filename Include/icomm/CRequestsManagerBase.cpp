// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomm/CRequestsManagerBase.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/CSystem.h>
#include <icomm/IRequest.h>
#include <icomm/CRequestHeader.h>
#include <icomm/CResponseHeader.h>


namespace icomm
{


void CRequestsManagerBase::Reset()
{
	m_requestsMap.clear();
}


bool CRequestsManagerBase::OnReceived(iser::IArchive& archive)
{
	Q_ASSERT (!archive.IsStoring());

	CResponseHeader responseHeader;
	bool retVal = responseHeader.Serialize(archive);
	if (!retVal){
		return false;
	}

	QByteArray requestId = responseHeader.GetRequestId();
	RequestsMap::iterator iterator = m_requestsMap.find(requestId);
	if (iterator != m_requestsMap.end()){
		RequestData& data = iterator.value();
		Q_ASSERT(data.requestPtr != NULL);

		data.errorMessage = "";
		IRequest& request = *data.requestPtr;

		CResponseHeader::ResponseFlags headerFlags = responseHeader.GetResponseFlags();

		if ((headerFlags & CResponseHeader::RF_KEEP_ALIVE) != 0){
			data.validityTime = m_timer.GetElapsed() + request.GetTimeOut();
		}
		else{
			data.state = RS_OK;
		}
		if ((headerFlags & CResponseHeader::RF_ERROR) != 0){
			data.state = RS_FAILED;
			data.errorMessage = request.ExtractMessage(responseHeader);
		}
		if ((headerFlags & CResponseHeader::RF_SERIALIZED) != 0){
			if (!request.Serialize(archive)){
				data.state = RS_DAMAGED;
				static QString message = QObject::tr("ReceivedDamagedData");

				data.errorMessage = message;
			}
		}

		return true;
	}

	return false;
}


QByteArray CRequestsManagerBase::AddRequest(IRequest* requestPtr)
{
	if ((requestPtr == NULL) || !IsRequestsManagerActive()){
		return QByteArray();
	}

	QByteArray command = requestPtr->GetCommand();

	iser::IArchive* archivePtr = CreateSendArchive();

	if (archivePtr != NULL){
		Q_ASSERT(archivePtr->IsStoring());

		CRequestHeader header(command);
		bool isOk = header.Serialize(*archivePtr);

		isOk = isOk && requestPtr->Serialize(*archivePtr);

		RemoveSendArchive(*archivePtr, !isOk);

		if (isOk){
			RequestData& data = m_requestsMap[header.GetRequestId()];
			data.requestPtr.SetPtr(requestPtr);
			data.state = RS_WAIT;
			data.validityTime = m_timer.GetElapsed() + requestPtr->GetTimeOut();

			return header.GetRequestId();
		}
	}

	return QByteArray();
}


CRequestsManagerBase::RequestState CRequestsManagerBase::WaitRequestFinished(
			const QByteArray& requestId,
			bool killOnTimeout,
			RequestInfo* requestInfoPtr) const
{
	RequestState retVal = RS_NONE;
	QString errorMessage = "";

	static QString timeOutStr(QObject::tr("TimeOut"));

	if (!requestId.isEmpty()){
		RequestsMap::iterator iterator = m_requestsMap.find(requestId);
		if (iterator != m_requestsMap.end()){
			const RequestData& data = iterator.value();
			for (;;){
				Q_ASSERT(data.state > RS_NONE);
				Q_ASSERT(data.state <= RS_DAMAGED);

				if (data.state != RS_WAIT){
					errorMessage = data.errorMessage;
					retVal = data.state;
					m_requestsMap.erase(iterator);

					break;
				}
				else if (m_timer.GetElapsed() > data.validityTime){
					errorMessage = timeOutStr;
					if (killOnTimeout){
						retVal = RS_FAILED;
						m_requestsMap.erase(iterator);
					}
					else{
						retVal = RS_WAIT;
					}
					break;
				}

				istd::CSystem::Sleep(0.01);

				DispatchMessagesQueue();
			}
		}
	}
	else{
		bool waitMore;
		do{
			waitMore = false;

			RequestsMap::iterator iterator = m_requestsMap.begin();
			while (iterator != m_requestsMap.end()){
				const RequestData& data = iterator.value();
				Q_ASSERT(data.state > RS_NONE);
				Q_ASSERT(data.state <= RS_DAMAGED);

				if (data.state != RS_WAIT){
					if (data.state > RS_OK){
						if (data.state > retVal){
							errorMessage = data.errorMessage;
							retVal = data.state;
						}
					}
					iterator = m_requestsMap.erase(iterator);
					continue;
				}
				else if (m_timer.GetElapsed() < data.validityTime){
					waitMore = true;
				}

				++iterator;
			}

			istd::CSystem::Sleep(0.01);

			DispatchMessagesQueue();
		} while (waitMore);

		if (retVal <= RS_OK){
			if (m_requestsMap.isEmpty()){
				errorMessage = timeOutStr;
				if (killOnTimeout){
					m_requestsMap.clear();

					retVal = RS_FAILED;
				}
				else{
					retVal = RS_WAIT;
				}
			}
			else{
				retVal = RS_OK;
			}
		}
	}

	if (requestInfoPtr != NULL){
		requestInfoPtr->state = retVal;
		requestInfoPtr->errorMessage = errorMessage;
	}

	return retVal;
}


void CRequestsManagerBase::Flush()
{
}


} // namespace icomm


