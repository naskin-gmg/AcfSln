#pragma once


// ACF includes
#include <istd/IPolymorphic.h>

// ACF-Solutions includes
#include <iauth/CLogoutEvent.h>


namespace iauth
{


template<class BaseClass>
class TLogoutEventHandlerWrap: public BaseClass, virtual public istd::IPolymorphic
{
public:
	virtual bool OnTryToLogout();

	// reimplemented (QObject)
	virtual bool event(QEvent* e) override;
};


template<class BaseClass>
bool TLogoutEventHandlerWrap<BaseClass>::OnTryToLogout()
{
	return true;
}


// reimplemented (QObject)

template<class BaseClass>
bool TLogoutEventHandlerWrap<BaseClass>::event(QEvent* e)
{
	CLogoutEvent* logoutEventPtr = dynamic_cast<CLogoutEvent*>(e);
	if (logoutEventPtr != nullptr) {
		if (!OnTryToLogout()){
			logoutEventPtr->ignore();
		}

		return true;
	}

	return BaseClass::event(e);
}


}	// namespace iauth


