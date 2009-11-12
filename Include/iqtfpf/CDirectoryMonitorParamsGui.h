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
	virtual void UpdateEditor(int updateFlags = 0);

protected slots:
	void on_FilesCheck_toggled(bool isChecked);
	void on_DirsCheck_toggled(bool isChecked);
	void on_DrivesCheck_toggled(bool isChecked);
	void on_ModifiedCheck_toggled(bool isChecked);
	void on_AddedCheck_toggled(bool isChecked);
	void on_RemovedCheck_toggled(bool isChecked);
	void on_AttributesCheck_toggled(bool isChecked);
	void on_PoolingIntervallSpin_valueChanged(double value);
};


} // namespace iqtfpf


#endif // !iqtfpf_CDirectoryMonitorParamsGui_included


