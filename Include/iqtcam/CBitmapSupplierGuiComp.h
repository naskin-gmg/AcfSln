#ifndef iqtcam_CBitmapSupplierGuiComp_included
#define iqtcam_CBitmapSupplierGuiComp_included


// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include <ifile/IFilePersistence.h>
#include <imod/IObserver.h>
#include <imod/TModelWrap.h>
#include <iimg/CBitmap.h>
#include <iqtgui/IGuiObject.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <iimg/IBitmapProvider.h>

#include <iqtinsp/TSupplierGuiCompBase.h>

#include <iqtcam/iqtcam.h>

#include <GeneratedFiles/iqtcam/ui_CBitmapSupplierGuiComp.h>


namespace iqtcam
{


class CBitmapSupplierGuiComp:
			public ilog::TLoggerCompWrap<
						iqtinsp::TSupplierGuiCompBase<Ui::CBitmapSupplierGuiComp> >,
			virtual public iimg::IBitmapProvider
{
	Q_OBJECT

public:
	typedef ilog::TLoggerCompWrap<
				iqtinsp::TSupplierGuiCompBase<Ui::CBitmapSupplierGuiComp> > BaseClass;

	I_BEGIN_COMPONENT(CBitmapSupplierGuiComp);
		I_REGISTER_INTERFACE(iimg::IBitmapProvider);
		I_ASSIGN(m_bitmapLoaderCompPtr, "BitmapLoader", "Saves bitmap to file", false, "BitmapLoader");
		I_ASSIGN(m_snapIntervalAttrPtr, "SnapInterval", "Snap interval in ms", true, 40);
	I_END_COMPONENT;

	CBitmapSupplierGuiComp();

	// reimplemented (iimg::IBitmapProvider)
	virtual const iimg::IBitmap* GetBitmap() const;

protected Q_SLOTS:
	void on_SnapImageButton_clicked();
	void on_LiveImageButton_toggled(bool checked);
	void on_SaveImageButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
	void OnTimerReady();

protected:
	void DoSnap(bool noGui = false);

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

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

private:
	I_REF(ifile::IFilePersistence, m_bitmapLoaderCompPtr);
	I_ATTR(int, m_snapIntervalAttrPtr);

	imod::TModelWrap<iimg::CBitmap> m_bitmap;

	QTimer m_timer;
};


} // namespace iqtcam


#endif // !iqtcam_CBitmapSupplierGuiComp_included


