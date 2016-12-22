#ifndef iblobgui_CBlobFilterGui_included
#define iblobgui_CBlobFilterGui_included


// QSF includes
#include <iblob/IBlobFilterParams.h>

#include <GeneratedFiles/iblobgui/ui_CBlobFilterGui.h>


namespace iblobgui
{


class CBlobFilterGui: public QWidget, public Ui::CFilterWidget
{
	Q_OBJECT
public:
	CBlobFilterGui(QWidget* parentPtr = 0);

	virtual void retranslateUi(QWidget* widgetPtr);

	void SetFilterInfo(const iblob::IBlobFilterParams::Filter& info);
	iblob::IBlobFilterParams::Filter GetFilterInfo() const;

	void SetSupportedProperties(const iprm::IOptionsList& features);

Q_SIGNALS:
	void FilterChanged();
	void RemoveFilter(QWidget* filterGui);

protected:
	void UpdateComboBoxes();
	void UpdateFilter();

protected Q_SLOTS:
	void OnFilterParameterChanged();
	void on_RemoveButton_clicked();
};


} // namespace iblobgui


#endif // !iblobgui_CFilterWidget_included
