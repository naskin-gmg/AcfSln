#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>

// ACF includes
#include <iser/IArchive.h>
#include <icomm/IResponsesManager.h>
#include <icomm/IResponse.h>


namespace icomm
{


/**
	This is base implementation of icomm::IResponsesManager interface.
	This is abstract class, to use it you have to implement abstract methods.
*/
class CResponsesManagerBase: public IResponsesManager
{
public:
	~CResponsesManagerBase();

	/**
		Remove all providers from this manager.
	*/
	virtual void Reset();

	/**
		This method should be called from end implementation when new message is received.
	*/
	virtual bool OnReceived(iser::IArchive& request, iser::IArchive& answer);

	// reimplemented (icomm::IResponsesManager)
	virtual void ConnectProvider(IResponse* providerPtr);
	virtual void DisconnectProvider(IResponse* providerPtr);

protected:
	typedef QMap<QByteArray, IResponse*> ProvidersMap;

	// abstract methods
	/**
		Send answer and clear archive to be ready for next operation.
	*/
	virtual bool SendAndClear(iser::IArchive& answer) = 0;

private:
	ProvidersMap m_providersMap;
};


} // namespace icomm


