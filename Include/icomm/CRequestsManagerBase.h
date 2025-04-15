#pragma once


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CGeneralTimeStamp.h>
#include <iser/IArchive.h>
#include <icomm/IRequestsManager.h>
#include <icomm/IRequest.h>


namespace icomm
{


/**
	Standard implementation of icomm::IRequestsManager interface.
 */
class CRequestsManagerBase: public IRequestsManager
{
public:
	/**
		Removes all container from this manager.
	*/
	void Reset();

	/**
		This mathod should be called from end implementation when new message is received.
	*/
	bool OnReceived(iser::IArchive& archive);

	// reimplemented (icomm::IRequestsManager)
	virtual QByteArray AddRequest(IRequest* requestPtr) override;
	virtual RequestState WaitRequestFinished(
				const QByteArray& requestId = QByteArray(),
				bool killOnTimeout = true,
				RequestInfo* requestInfoPtr = NULL) const override;
	virtual void Flush() override;

protected:
	struct RequestData
	{
		RequestState state;
		QString errorMessage;
		double validityTime;
		istd::TDelPtr<IRequest> requestPtr;
	};

	typedef QMap<QByteArray, RequestData> RequestsMap;

	// abstract methods
	/**
		Get access to internal archive object.
		Every archive created with this method should be removed
		using RemoveSendArchive() method.
		\return		Pointer to created archive object, or NULL if error.
	*/
	virtual iser::IArchive* CreateSendArchive() = 0;

	/**
		Flush internal archive object.
		\param	archive		archive object created using CreateSendArchive() method.
		\param	wasError	signalize that an error occured during processing of this archive and it shouldn't be sent (only remove).
		\return				true, if no erorrs occured during sending.
	*/
	virtual bool RemoveSendArchive(iser::IArchive& archive, bool wasError = false) = 0;

private:
	mutable RequestsMap m_requestsMap;
	istd::CGeneralTimeStamp m_timer;
};


} // namespace icomm

