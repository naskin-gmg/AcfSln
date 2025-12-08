#ifndef icalibgui_CPerspectiveCalibEditorComp_included
#define icalibgui_CPerspectiveCalibEditorComp_included


// ACF includes
#include <imod/CMultiModelDispatcherBase.h>
#include <iqt2d/TShapeParamsGuiCompBase.h>

// ACF-Solutions includes
#include <icalib/CPerspectiveCalibration2d.h>
#include <GeneratedFiles/icalibgui/ui_CPerspectiveCalibEditorComp.h>


namespace icalibgui
{


class CPerspectiveCalibEditorComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CPerspectiveCalibEditorComp, icalib::CPerspectiveCalibration2d>,
			protected imod::CMultiModelDispatcherBase,
			virtual public iqt2d::IViewExtender
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CPerspectiveCalibEditorComp, icalib::CPerspectiveCalibration2d> BaseClass;

	I_BEGIN_COMPONENT(CPerspectiveCalibEditorComp);
		I_REGISTER_INTERFACE(iqt2d::IViewExtender);
		I_ASSIGN(m_calibProviderCompPtr, "CalibrationProvider", "Optional calibration provider for calibrate button", false, "CalibrationProvider");
		I_ASSIGN_TO(m_calibProviderModelCompPtr, m_calibProviderCompPtr, false);
	I_END_COMPONENT;

	CPerspectiveCalibEditorComp();

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags) override;
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr) override;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet) override;

protected Q_SLOTS:
	void on_CalibrateButton_clicked();
	void OnParamsChanged(double value);

private:
	I_REF(i2d::ICalibrationProvider, m_calibProviderCompPtr);
	I_REF(imod::IModel, m_calibProviderModelCompPtr);

	istd::TUniqueInterfacePtr<i2d::ICalibration2d> m_calibrationCopyPtr;
	iview::CCalibratedViewBase* m_currentViewPtr;
};


} // namespace icalibgui


#endif // !icalibgui_CPerspectiveCalibEditorComp_included


