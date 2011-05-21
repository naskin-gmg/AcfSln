#ifndef iqtex_CTcpTerminalGuiComp_included
#define iqtex_CTcpTerminalGuiComp_included


// QT includes
#include <QTimer>
#include <QTcpSocket>

// ACF includes
#include "imod/IModel.h"
#include "imod/CSingleModelObserverBase.h"
#include "iprm/IFileNameParam.h"
#include "iqtgui/TDesignerGuiCompBase.h"

#include "iqtex/Generated/Ui_CTcpTerminalGuiComp.h"


namespace iqtex
{


/**
	Implementation of simple TCP terminal.
*/
class CTcpTerminalGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CTcpTerminalGuiComp>,
			protected imod::CSingleModelObserverBase
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CTcpTerminalGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CTcpTerminalGuiComp);
		I_ASSIGN(m_urlParamCompPtr, "UrlParam", "URL parameter used to find out address and socket for TCP connection", true, "UrlParam");
		I_ASSIGN_TO(m_urlParamModelCompPtr, m_urlParamCompPtr, false);
		I_ASSIGN_MULTI_0(m_presetTextsAttrPtr, "PresetTexts", "Set of preset texts", false);
	I_END_COMPONENT;

	CTcpTerminalGuiComp();

protected:
	void UpdateStateIcon();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected slots:
	void on_SendButton_clicked();
	void on_ClearSentListButton_clicked();
	void on_ClearReceivedListButton_clicked();
	void OnReadyRead();
	void OnConectTimer();
	void OnStateChanged(QAbstractSocket::SocketState socketState);

private:
	I_REF(iprm::IFileNameParam, m_urlParamCompPtr);
	I_REF(imod::IModel, m_urlParamModelCompPtr);
	I_MULTIATTR(istd::CString, m_presetTextsAttrPtr);

	QTimer m_connectTimer;
	QTcpSocket m_socket;

	QIcon m_connectedIcon;
	QIcon m_disconnectedIcon;
};


} // namespace iqtex


#endif // !iqtex_CTcpTerminalGuiComp_included


