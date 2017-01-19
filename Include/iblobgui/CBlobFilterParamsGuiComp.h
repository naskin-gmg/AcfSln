#ifndef iblobgui_CBlobFilterParamsGuiComp_included
#define iblobgui_CBlobFilterParamsGuiComp_included


// ACF includes
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <iblob/IBlobFilterParams.h>
#include <GeneratedFiles/iblobgui/ui_CBlobFilterParamsGuiComp.h>


namespace iblobgui
{


class CBlobFilterParamsGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CBlobFilterParamsGuiComp, iblob::IBlobFilterParams>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CBlobFilterParamsGuiComp, iblob::IBlobFilterParams> BaseClass;

	I_BEGIN_COMPONENT(CBlobFilterParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void OnFilterParameterChanged();

	void on_ActivateFilterCheckBox_toggled(bool);
	void on_AddFilterButton_clicked();
	void on_RemoveAllButton_clicked();
	void OnRemoveFilter(QWidget* filterGui);

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqt::CGuiObjectBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	void CreateFilter();
};


} // namespace iblobgui


#endif // !iblobgui_CBlobFilterParamsGuiComp_included
