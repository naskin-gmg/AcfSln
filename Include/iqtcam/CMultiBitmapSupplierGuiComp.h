#pragma once


// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include "iimg/CBitmap.h"
#include "iimg/TMultiPageBitmap.h"

// ACF-Solutions includes
#include "iimg/IMultiBitmapProvider.h"
#include "iqtinsp/TSupplierGuiCompBase.h"
#include "GeneratedFiles/iqtcam/ui_CMultiBitmapSupplierGuiComp.h"


namespace iqtcam
{


class CMultiBitmapSupplierGuiComp:
			public iqtinsp::TSupplierGuiCompBase<Ui::CMultiBitmapSupplierGuiComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CMultiBitmapSupplierGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CMultiBitmapSupplierGuiComp);
		I_ASSIGN(m_singleBitmapPersistenceCompPtr, "SingleBitmapPersistence", "Persistence for saving a single bitmap", false, "SingleBitmapPersistence");
		I_ASSIGN(m_multiBitmapPersistenceCompPtr, "MultiBitmapPersistence", "Persistence for saving all camera images in a document", false, "MultiBitmapPersistence");
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Maximum icon size in preview in px. Use 0 if you want to disable bitmap preview", true, 48);
		I_ASSIGN(m_filePathFormatAttrPtr, "FilePathFormat", "Format of file path used to save bitmap, used only if save folder is used.\n\t- %1 will be replaced with current date\n\t- %2 with time\n\t- %3 with channel index\n\t- %4 with image resolution", true, "%1%2_%3.png");
	I_END_COMPONENT;

	CMultiBitmapSupplierGuiComp();

protected Q_SLOTS:
	void on_SnapImageButton_clicked();
	void on_LiveImageButton_toggled(bool checked);
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
	void on_BitmapPreview_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void on_ExportSelectedImageButton_clicked();
	void on_ExportBitmapDocumentButton_clicked();
	void OnTimerReady();

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiHidden();

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

private:
	/**
		Fetches a selected bitmap from IMultiBitmapProvider
	*/
	void SelectBitmap(int bitmapIdx);

	void ExportSelectedBitmap(const QString& filePath = QString());
	void ExportBitmapDocument(const QString& filePath = QString());
	void UpdateCommands();

	static QString CreateFileFilterForPersistence(const ifile::IFilePersistence& persistence);

private:
	I_REF(ifile::IFilePersistence, m_singleBitmapPersistenceCompPtr);
	I_REF(ifile::IFilePersistence, m_multiBitmapPersistenceCompPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(QString, m_filePathFormatAttrPtr);

	imod::TModelWrap<iimg::CBitmap> m_bitmap;
	iimg::CQImageBasedMultiPageBitmap m_bitmapDocument;

	QList<QIcon>m_icons;

	QTimer m_timer;
};


} // namespace iqtcam


