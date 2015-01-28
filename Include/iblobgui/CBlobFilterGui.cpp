#include "iblobgui/CBlobFilterGui.h"


// ACF includes
#include "iqt/CSignalBlocker.h"

// ACF-Solutions includes
#include "iblob/CBlobDescriptorInfo.h"


namespace iblobgui
{


CBlobFilterGui::CBlobFilterGui(QWidget* /*parentPtr*/)
{
	setupUi(this);

	ConditionCombo->setCurrentIndex(0);
	ParameterCombo->setCurrentIndex(0);

	connect(ParameterCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(OnFilterParameterChanged()));
	connect(ConditionCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(OnFilterParameterChanged()));
	connect(Value1SpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnFilterParameterChanged()));
	connect(Value2SpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnFilterParameterChanged()));

	UpdateComboBoxes();
}


void CBlobFilterGui::retranslateUi(QWidget *widgetPtr)
{
	int ConditionComboIndex = ConditionCombo->currentIndex();
	int ParameterComboIndex = ParameterCombo->currentIndex();

	Ui::CFilterWidget::retranslateUi(widgetPtr);

	UpdateComboBoxes();

	ConditionCombo->setCurrentIndex(ConditionComboIndex);
	ParameterCombo->setCurrentIndex(ParameterComboIndex);
}


void CBlobFilterGui::SetFilterInfo(const iblob::IBlobFilterParams::Filter& info)
{
	int paramIndex = ParameterCombo->findData(info.operation);
	ParameterCombo->setCurrentIndex(paramIndex);
	int conditionIndex = ConditionCombo->findData(info.condition);
	ConditionCombo->setCurrentIndex(conditionIndex);
	Value1SpinBox->setValue(info.valueRange.GetMinValue());
	Value2SpinBox->setValue(info.valueRange.GetMaxValue());

	UpdateFilter();
}


iblob::IBlobFilterParams::Filter CBlobFilterGui::GetFilterInfo() const
{
	iblob::IBlobFilterParams::Filter info;

	info.operation = iblob::IBlobFilterParams::FilterOperation(ParameterCombo->itemData(ParameterCombo->currentIndex()).toInt());
	info.condition = iblob::IBlobFilterParams::FilterCondition(ConditionCombo->itemData(ConditionCombo->currentIndex()).toInt());
	info.valueRange.SetMinValue(Value1SpinBox->value());
	info.valueRange.SetMaxValue(Value2SpinBox->value());

	return info;
}


void CBlobFilterGui::SetFeatures(const iblob::BlobDescriptorInfoList& features)
{
	iqt::CSignalBlocker block(ParameterCombo);

	ParameterCombo->clear();

	for (int i = 0; i < features.count(); i++){
		const iblob::CBlobDescriptorInfo& info = features.at(i);
		ParameterCombo->addItem(info.GetFullName(), (int)info.GetDescriptorType());
		ParameterCombo->setItemData(ParameterCombo->count()-1, info.GetDescription(), Qt::ToolTipRole);
	}
}


// protected

void CBlobFilterGui::UpdateComboBoxes()
{
	ConditionCombo->setItemData(0, iblob::IBlobFilterParams::FC_BETWEEN);
	ConditionCombo->setItemData(1, iblob::IBlobFilterParams::FC_OUTSIDE);
	ConditionCombo->setItemData(2, iblob::IBlobFilterParams::FC_EQUAL);
	ConditionCombo->setItemData(3, iblob::IBlobFilterParams::FC_NOT_EQUAL);
	ConditionCombo->setItemData(4, iblob::IBlobFilterParams::FC_GREATER);
	ConditionCombo->setItemData(5, iblob::IBlobFilterParams::FC_GREATER_EQUAL);
	ConditionCombo->setItemData(6, iblob::IBlobFilterParams::FC_LESS);
	ConditionCombo->setItemData(7, iblob::IBlobFilterParams::FC_LESS_EQUAL);
}


void CBlobFilterGui::UpdateFilter()
{
	iqt::CSignalBlocker block(ParameterCombo);

	// only show value2 for BETWEEN and OUTSIDE filters
	int condition = ConditionCombo->itemData(ConditionCombo->currentIndex()).toInt();
	bool showValue2 = 
		(condition == iblob::IBlobFilterParams::FC_BETWEEN) ||
		(condition == iblob::IBlobFilterParams::FC_OUTSIDE);

	AndLabel->setVisible(showValue2);
	Value2SpinBox->setVisible(showValue2);
}


void CBlobFilterGui::OnFilterParameterChanged()
{
	UpdateFilter();

	emit FilterChanged();
}


void CBlobFilterGui::on_RemoveButton_clicked()
{
	emit RemoveFilter(this);
}


} // namespace iblobgui

