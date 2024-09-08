#pragma once


// Qt includes
#include <QtCore/QObject>

// ACF-Solutions includes
#include <iauth/CLogoutEvent.h>


namespace iauth
{


template<class BaseClass>
class TLogoutEventHandlerWrap: public BaseClass
{
public:
	TLogoutEventHandlerWrap();

	void EnableLogoutHandler(bool enable = true);

protected:
	// return false if logout should be canceled
	virtual bool OnTryToLogout() = 0;

private:
	class EventFilter: public QObject
	{
	public:
		EventFilter(TLogoutEventHandlerWrap& parent);

	protected:
		// reimplemented (QObject)
		bool eventFilter(QObject* object, QEvent* event) override;

	private:
		TLogoutEventHandlerWrap& m_parent;
	};

private:
	EventFilter m_eventFilter;
	bool m_isEnabled;
};


// public methods

template<class BaseClass>
TLogoutEventHandlerWrap<BaseClass>::TLogoutEventHandlerWrap()
	:m_eventFilter(*this),
	m_isEnabled(false)
{
}


template<class BaseClass>
void TLogoutEventHandlerWrap<BaseClass>::EnableLogoutHandler(bool enable)
{
	if (m_isEnabled != enable){
		if (enable == true){
			QCoreApplication::instance()->installEventFilter(&m_eventFilter);
		}
		else {
			QCoreApplication::instance()->removeEventFilter(&m_eventFilter);
		}

		m_isEnabled = enable;
	}
}


// public methods of the embedded class EventFilter

template<class BaseClass>
TLogoutEventHandlerWrap<BaseClass>::EventFilter::EventFilter(TLogoutEventHandlerWrap& parent)
	:m_parent(parent)
{
}


// protected methods of the embedded class EventFilter

// reimplemented (QObject)

template<class BaseClass>
bool TLogoutEventHandlerWrap<BaseClass>::EventFilter::eventFilter(QObject* /*object*/, QEvent* event)
{
	if (event->type() == CLogoutEvent::type()){
		CLogoutEvent* logoutEventPtr = dynamic_cast<CLogoutEvent*>(event);
		if (logoutEventPtr != nullptr){
			if (!m_parent.OnTryToLogout()){
				logoutEventPtr->accept();
				logoutEventPtr->CancelLogout();

				return true;
			}
		}
	}

	return false;
}


}	// namespace iauth


