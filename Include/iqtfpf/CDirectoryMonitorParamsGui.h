#ifndef iqtfpf_CDirectoryMonitorParamsGui_included
#define iqtfpf_CDirectoryMonitorParamsGui_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"


// AcfSln includes
#include "ifpf/IDirectoryMonitorParams.h"

#include "iqtfpf/Generated/ui_CDirectoryMonitorParamsGui.h"


namespace iqtfpf
{


class CDirectoryMonitorParamsGui: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CDirectoryMonitorParamsGui,
			ifpf::IDirectoryMonitorParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CDirectoryMonitorParamsGui,
				ifpf::IDirectoryMonitorParams> BaseClass;

	I_BEGIN_COMPONENT(CDirectoryMonitorParamsGui)
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
	void on_FilesCheck_toggled(bool isChecked);
	void on_DirsCheck_toggled(bool isChecked);
	void on_DrivesCheck_toggled(bool isChecked);
	void on_ModifiedCheck_toggled(bool isChecked);
	void on_AddedCheck_toggled(bool isChecked);
	void on_RemovedCheck_toggled(bool isChecked);
	void on_AttributesCheck_toggled(bool isChecked);
	void on_PoolingIntervallSpin_valueChanged(double value);
	void on_AddAcceptPatternButton_clicked();
	void on_RemoveAcceptPatternButton_clicked();
	void on_AcceptPatternsList_itemSelectionChanged();
	void on_AcceptPatternsList_itemChanged(QTreeWidgetItem* item, int column);
	void on_AddIgnorePatternButton_clicked();
	void on_RemoveIgnorePatternButton_clicked();
	void on_IgnorePatternsList_itemSelectionChanged();
	void on_IgnorePatternsList_itemChanged(QTreeWidgetItem* item, int column);

private:
	void AddPattern(const QString& filter, QTreeWidget* treeView);
	void RemoveSelectedPatterns(QTreeWidget* treeView);
	void ResetEditor();
	void OnModelUpdate();
};


} // namespace iqtfpf


#endif // !iqtfpf_CDirectoryMonitorParamsGui_included


