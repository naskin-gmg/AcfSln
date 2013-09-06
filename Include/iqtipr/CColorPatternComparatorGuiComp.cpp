#include "iqtipr/CColorPatternComparatorGuiComp.h"


// ACF includes
#include "imeas/IDataSequenceProvider.h"


namespace iqtipr
{


// protected slots

void CColorPatternComparatorGuiComp::on_TestButton_clicked()
{
	DoTest();
}


// protected methods

// reimplemented (iqtgui::IGuiObject)

void CColorPatternComparatorGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
}


void CColorPatternComparatorGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CColorPatternComparatorGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();
}


void CColorPatternComparatorGuiComp::OnGuiModelDetached()
{
	BaseClass::OnGuiModelDetached();
}


void CColorPatternComparatorGuiComp::UpdateGui(int updateFlags)
{
	BaseClass::UpdateGui(updateFlags);

	while (ColorTable->rowCount()){
		ColorTable->removeRow(0);
	}

	Q_ASSERT(IsGuiCreated());

	const imeas::INumericValueProvider* numericValueProviderPtr = CompCastPtr<const imeas::INumericValueProvider>(GetObjectPtr());
	if (numericValueProviderPtr != NULL){
		int valuesCount = numericValueProviderPtr->GetValuesCount();
		if (valuesCount > 0){
			const imeas::INumericValue& value = numericValueProviderPtr->GetNumericValue(0);

			const imath::CVarVector& colorValues = value.GetValues();
			int colorValuesCount = colorValues.GetElementsCount();

			if (colorValuesCount == 2){
				QTableWidgetItem* workingColorItem = new QTableWidgetItem(QString::number(colorValues[0]));
				QTableWidgetItem* teachedColorItem = new QTableWidgetItem(QString::number(colorValues[1]));
				workingColorItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
				teachedColorItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

				double delta = colorValues[1] - colorValues[0];
				double deltaRel = abs(delta) / 255;
				QTableWidgetItem* diffItem = new QTableWidgetItem(QString("%1 (%2%)").arg(delta).arg(deltaRel*100, 0, 'g', 2));

				ColorTable->setRowCount(1);
				ColorTable->setItem(0, 0, workingColorItem);
				ColorTable->setItem(0, 1, teachedColorItem);
				ColorTable->setItem(0, 2, diffItem);

				ColorTable->verticalHeaderItem(0)->setText(tr("Grayvalue"));
			}
			else if(colorValuesCount == 6){
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
					double teachedValue = colorValues[i + 3];

					QTableWidgetItem* workingColorItem = new QTableWidgetItem(QString::number(currentValue));
					QTableWidgetItem* teachedColorItem = new QTableWidgetItem(QString::number(teachedValue));
					workingColorItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
					teachedColorItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

					double delta = teachedValue - currentValue;
					double deltaRel = abs(delta) / 255;
					QTableWidgetItem* diffItem = new QTableWidgetItem(QString("%1 (%2%)").arg(delta).arg(deltaRel*100, 0, 'g', 2));

					ColorTable->setItem(i, 0, workingColorItem);
					ColorTable->setItem(i, 1, teachedColorItem);
					ColorTable->setItem(i, 2, diffItem);
				}
			}
		}
	}
}


// reimplemented (iqtinsp::TSupplierGuiCompBase)

void CColorPatternComparatorGuiComp::OnSupplierParamsChanged()
{
	if (IsGuiCreated()){
		if (AutoUpdateButton->isChecked()){
			on_TestButton_clicked();
		}
	}
}


QWidget* CColorPatternComparatorGuiComp::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


} // namespace iqtipr


