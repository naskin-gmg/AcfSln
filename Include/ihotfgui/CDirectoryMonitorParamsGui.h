// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef ihotfgui_CDirectoryMonitorParamsGui_included
#define ihotfgui_CDirectoryMonitorParamsGui_included


// ACF includes
#include <iqtgui/TDesignerGuiObserverCompBase.h>


// AcfSln includes
#include <ihotf/IDirectoryMonitorParams.h>
#include <GeneratedFiles/ihotfgui/ui_CDirectoryMonitorParamsGui.h>


namespace ihotfgui
{


class CDirectoryMonitorParamsGui: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CDirectoryMonitorParamsGui,
			ihotf::IDirectoryMonitorParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CDirectoryMonitorParamsGui,
				ihotf::IDirectoryMonitorParams> BaseClass;

	I_BEGIN_COMPONENT(CDirectoryMonitorParamsGui);
	I_END_COMPONENT;

protected Q_SLOTS:
	void on_FilesCheck_toggled(bool isChecked);
	void on_DirsCheck_toggled(bool isChecked);
	void on_DrivesCheck_toggled(bool isChecked);
	void on_ModifiedCheck_toggled(bool isChecked);
	void on_AddedCheck_toggled(bool isChecked);
	void on_RemovedCheck_toggled(bool isChecked);
	void on_AttributesCheck_toggled(bool isChecked);
	void on_PollingIntervalSpin_valueChanged(double value);
	void on_AddAcceptPatternButton_clicked();
	void on_RemoveAcceptPatternButton_clicked();
	void on_AcceptPatternsList_itemSelectionChanged();
	void on_AcceptPatternsList_itemChanged(QTreeWidgetItem* item, int column);
	void on_AddIgnorePatternButton_clicked();
	void on_RemoveIgnorePatternButton_clicked();
	void on_IgnorePatternsList_itemSelectionChanged();
	void on_IgnorePatternsList_itemChanged(QTreeWidgetItem* item, int column);
	void OnModelUpdate();

protected:
	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;

private:
	void AddPattern(const QString& filter, QTreeWidget* treeView);
	void RemoveSelectedPatterns(QTreeWidget* treeView);
	void ResetEditor();
};


} // namespace ihotfgui


#endif // !ihotfgui_CDirectoryMonitorParamsGui_included


