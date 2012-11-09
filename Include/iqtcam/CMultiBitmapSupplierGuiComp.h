#ifndef iqtcam_CMultiBitmapSupplierGuiComp_included
#define iqtcam_CMultiBitmapSupplierGuiComp_included


// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include "iimg/CBitmap.h"

// ACF-Solutions includes
#include "iipr/IMultiBitmapProvider.h"
#include "iqtinsp/TSupplierGuiCompBase.h"

#include "iqtcam/Generated/ui_CMultiBitmapSupplierGuiComp.h"


namespace iqtcam
{


class CMultiBitmapSupplierGuiComp
: public iqtinsp::TSupplierGuiCompBase<Ui::CMultiBitmapSupplierGuiComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CMultiBitmapSupplierGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CMultiBitmapSupplierGuiComp);
		I_ASSIGN(m_bitmapLoaderCompPtr, "BitmapLoader", "Saves bitmap to file", false, "BitmapLoader");
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Maximum icon size in preview [px]", true, 48);
		I_ASSIGN(m_filePathFormatAttrPtr, "FilePathFormat", "Format of file path used to save bitmap, used only if save folder is used.\n\t- %1 will be replaced with current date\n\t- %2 with time\n\t- %3 with channel index\n\t- %4 with image resolution", false, "%1%2_%3.png");
	I_END_COMPONENT;

	CMultiBitmapSupplierGuiComp();

	protected Q_SLOTS:
	void on_SnapImageButton_clicked();
	void on_LiveImageButton_toggled(bool checked);
	void on_SaveImageButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
	void on_IconsView_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void OnTimerReady();

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(int updateFlags = 0);

private:
	I_REF(iser::IFileLoader, m_bitmapLoaderCompPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(QString, m_filePathFormatAttrPtr);

	/** Fetches a selected bitmap from IMultiBitmapProvider */
	void SelectBitmap(int bitmapIdx);

	imod::TModelWrap<iimg::CBitmap> m_bitmap;
	QList<QIcon>m_icons;

	QTimer m_timer;

};


} // namespace iqtcam


#endif // !iqtcam_CMultiBitmapSupplierGuiComp_included


