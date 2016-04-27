#include "iqtipr/CColorPatternComparatorGuiComp.h"

// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include "i2d/CDirection2d.h"
#include "imeas/IDataSequenceProvider.h"
#include "iprm/IEnableableParam.h"
#include "iprm/TParamsPtr.h"


namespace iqtipr
{


// protected slots

void CColorPatternComparatorGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CColorPatternComparatorGuiComp::on_TaskEnabledCB_toggled(bool)
{
	UpdateModel();
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CColorPatternComparatorGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	TaskEnabledCB->hide();
}


void CColorPatternComparatorGuiComp::UpdateModel() const
{
	const imeas::INumericValueProvider* numericValueProviderPtr = CompCastPtr<const imeas::INumericValueProvider>(GetObservedObject());
	if (numericValueProviderPtr != NULL){
		const iinsp::ISupplier* supplierPtr = dynamic_cast<const iinsp::ISupplier*>(numericValueProviderPtr);
		if (supplierPtr != NULL){
			iprm::TParamsPtr<iprm::IEnableableParam> checkEnabledPtr(supplierPtr->GetModelParametersSet(), *m_taskEnabledIdAttrPtr); 
			if (checkEnabledPtr.IsValid()){
				(const_cast<iprm::IEnableableParam*>(checkEnabledPtr.GetPtr()))->SetEnabled(TaskEnabledCB->isChecked());
			}
		}
	}
}


void CColorPatternComparatorGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::UpdateGui(changeSet);

	while (ColorTable->rowCount()){
		ColorTable->removeRow(0);
	}

	Q_ASSERT(IsGuiCreated());

	const imeas::INumericValueProvider* numericValueProviderPtr = CompCastPtr<const imeas::INumericValueProvider>(GetObservedObject());
	if (numericValueProviderPtr != NULL){
		const iinsp::ISupplier* supplierPtr = dynamic_cast<const iinsp::ISupplier*>(numericValueProviderPtr);
		if (supplierPtr != NULL){
			// if the task is disabled: no check in this case
			iprm::TParamsPtr<iprm::IEnableableParam> checkEnabledPtr(supplierPtr->GetModelParametersSet(), *m_taskEnabledIdAttrPtr); 
			if (checkEnabledPtr.IsValid()){
				TaskEnabledCB->show();
				TaskEnabledCB->setChecked(checkEnabledPtr->IsEnabled());
				if (!checkEnabledPtr->IsEnabled()){
					return;
				}
			}
		}

		int valuesCount = numericValueProviderPtr->GetValuesCount();
		if (valuesCount > 0){
			const imeas::INumericValue& value = numericValueProviderPtr->GetNumericValue(0);

			const imath::CVarVector& colorValues = value.GetValues();
			int colorValuesCount = colorValues.GetElementsCount();

			if (colorValuesCount == 3){
				QTableWidgetItem* workingColorItem = new QTableWidgetItem(QString::number(colorValues[0]));
				QTableWidgetItem* taughtColorItem = new QTableWidgetItem(QString::number(colorValues[1]));
				workingColorItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
				taughtColorItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

				double delta = colorValues[1] - colorValues[0];
				double deltaRel = abs(delta) / 255;
				QTableWidgetItem* diffItem = new QTableWidgetItem(QString("%1").arg(delta));
				QTableWidgetItem* diffProcItem = new QTableWidgetItem(QString("%1%").arg(deltaRel * 100, 2, 'g', 2));

				double errorValue = colorValues[2];
				diffProcItem->setForeground(errorValue < 0 ? Qt::green : QColor("orange"));
				
				ColorTable->setRowCount(1);
				ColorTable->setItem(0, 0, taughtColorItem);
				ColorTable->setItem(0, 1, workingColorItem);
				ColorTable->setItem(0, 2, diffItem);
				ColorTable->setItem(0, 3, diffProcItem);

				ColorTable->verticalHeaderItem(0)->setText(tr("Grayvalue"));
			}
			else if(colorValuesCount == 9){
				ColorTable->setRowCount(3);

			#if QT_VERSION < 0x050000
				ColorTable->verticalHeader()->setResizeMode(QHeaderView::Stretch);
			#else
				ColorTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
			#endif

				ColorTable->setVerticalHeaderItem(0, new QTableWidgetItem(tr("Hue")));
				ColorTable->setVerticalHeaderItem(1, new QTableWidgetItem(tr("Saturation")));
				ColorTable->setVerticalHeaderItem(2, new QTableWidgetItem(tr("Value")));

				for (int i = 0; i < 3; i++){
					double currentValue = colorValues[i];
					double taughtValue = colorValues[i + 3];

					double delta = taughtValue - currentValue;
					double deltaRel = abs(delta) / 255;

					if (i == 0){ // Hue difference
						i2d::CDirection2d taughtHue = i2d::CDirection2d::FromDegree(taughtValue);
						i2d::CDirection2d workingHue = i2d::CDirection2d::FromDegree(currentValue);

						deltaRel = workingHue.DistInDegree(taughtHue) / 360.0;
					}

					QTableWidgetItem* workingColorItem = new QTableWidgetItem(QString::number(currentValue, 'f', 2));
					QTableWidgetItem* taughtColorItem = new QTableWidgetItem(QString::number(taughtValue, 'f', 2));
					workingColorItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
					taughtColorItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

					QTableWidgetItem* diffItem = new QTableWidgetItem(QString("%1").arg(delta, 2, 'f', 2));
					QTableWidgetItem* diffProcItem = new QTableWidgetItem(QString("%1%").arg(deltaRel * 100, 2, 'f', 2));
					
					double errorValue = colorValues[i + 6];
					diffProcItem->setForeground(errorValue < 0 ? Qt::green : QColor("orange"));

					ColorTable->setItem(i, 0, taughtColorItem);
					ColorTable->setItem(i, 1, workingColorItem);
					ColorTable->setItem(i, 2, diffItem);
					ColorTable->setItem(i, 3, diffProcItem);
				}
			}
		}
	}
}


// reimplemented (iqtinsp::TSupplierGuiCompBase)

void CColorPatternComparatorGuiComp::OnSupplierParamsChanged()
{
	if (IsGuiCreated() && AutoUpdateButton->isChecked()){
		QTimer::singleShot(1, this, SLOT(on_TestButton_clicked()));
	}
}


QWidget* CColorPatternComparatorGuiComp::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParametersFrame;
}


} // namespace iqtipr


