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
		I_ASSIGN(m_snapButtonTextAttrPtr, "SnapButtonText", "Snap button text", false, "Snap");
		I_ASSIGN(m_snapButtonTooltipAttrPtr, "SnapButtonTooltip", "Snap button tooltip", false, "Snap next image");
		I_ASSIGN(m_bitmapFactPtr, "BitmapFactory", "Factory used for bitmap instantiation", false, "BitmapFactory");
		I_ASSIGN(m_showControlPanelAttrPtr, "ShowControlPanel", "If enabled, the control button panel is shown", true, true);

		I_REGISTER_SUBELEMENT(OriginalBitmapProvider);
		I_REGISTER_SUBELEMENT_INTERFACE(OriginalBitmapProvider, iimg::IBitmapProvider, ExtractOriginalBitmapProvider);
		I_REGISTER_SUBELEMENT_INTERFACE(OriginalBitmapProvider, imod::IModel, ExtractOriginalBitmapProvider);
		I_REGISTER_SUBELEMENT_INTERFACE(OriginalBitmapProvider, istd::IChangeable, ExtractOriginalBitmapProvider);

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

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const override;

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result) override;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached() override;
	virtual void OnGuiModelDetached() override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiHidden() override;
	virtual void OnGuiRetranslate() override;
	virtual void OnGuiDesignChanged() override;

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	void UpdateViewShapes();

	// OriginalBitmapProvider subclass
	class OriginalBitmapProvider : public iimg::IBitmapProvider
	{
	public:
		// reimplemented (iimg::IBitmapProvider)
		virtual const iimg::IBitmap* GetBitmap() const
		{
			return &m_bitmap;
		}

		iimg::CBitmap m_bitmap;
	};

	// static template methods for sub-element access
	template <class InterfaceType>
	static InterfaceType* ExtractOriginalBitmapProvider(CBitmapSupplierGuiComp& component)
	{
		return &component.m_originalBitmapProvider;
	}

	imod::TModelWrap<OriginalBitmapProvider> m_originalBitmapProvider;

private:
	I_REF(ifile::IFilePersistence, m_bitmapLoaderCompPtr);
	I_FACT(iimg::IBitmap, m_bitmapFactPtr);
	I_ATTR(int, m_snapIntervalAttrPtr);
	I_ATTR(QByteArray, m_snapButtonTextAttrPtr);
	I_ATTR(QByteArray, m_snapButtonTooltipAttrPtr);
	I_ATTR(bool, m_showControlPanelAttrPtr);

	typedef imod::TModelWrap<iimg::CBitmap> BitmapImpl;

	iimg::IBitmapSharedPtr m_bitmapPtr;

	QTimer m_timer;
};


} // namespace iqtcam


#endif // !iqtcam_CBitmapSupplierGuiComp_included


