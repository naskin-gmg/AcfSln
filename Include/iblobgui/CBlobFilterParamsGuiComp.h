// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
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
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqt::CGuiObjectBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;

private:
	void CreateFilter();
};


} // namespace iblobgui


#endif // !iblobgui_CBlobFilterParamsGuiComp_included
