#ifndef iqtipr_CProcessedBitmapView_included
#define iqtipr_CProcessedBitmapView_included


// ACF includes
#include "iview/IShapeStatusInfo.h"
#include "iview/CImageShape.h"
#include "imod/TModelWrap.h"
#include "imod/CSingleModelObserverBase.h"
#include "iimg/CBitmap.h"
#include "iqtgui/CHierarchicalCommand.h"


// Local includes
#include "GeneratedFiles/iqtipr/ui_CProcessedBitmapView.h"


namespace iqtipr
{


class CProcessedBitmapView:
			public QWidget/*,
			public iview::IShapeStatusInfo*/
{
	Q_OBJECT
public:
	CProcessedBitmapView(QWidget* parent = NULL);

	void SetBitmap(const iimg::IBitmap* bitmapPtr);
	void SetTitle(const QString& title);

	//// reimplemented (iview::IShapeStatusInfo)
	//virtual QString GetInfoText() const;
	//virtual void SetInfoText(const QString& infoText);

protected Q_SLOTS:
	void on_SaveButton_clicked();

protected:
	Ui::CProcessedBitmapView m_ui;

	imod::TModelWrap<iimg::CBitmap> m_sourceImageModel;
	iview::CImageShape m_sourceImageShape;

	QString m_lastImagePath;

	iqtgui::CHierarchicalCommand m_toolBarCommands;
};


} // namespace iqtipr


#endif // !iqtipr_CProcessedBitmapView_included
