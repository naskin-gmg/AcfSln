#include "iqtmeas/CNumericParamsGuiComp.h"


// Qt includes
#include <QString>

// ACF includes
#include "istd/TChangeNotifier.h"

// ACF-Solutions includes
#include "imeas/IUnitInfo.h"
#include "imeas/INumericConstraints.h"


namespace iqtmeas
{


CNumericParamsGuiComp::CNumericParamsGuiComp()
:	m_widthScaleFactor(1),
	m_heightScaleFactor(1)
{
}


// reimplemented (imod::IModelEditor)

void CNumericParamsGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));

	imeas::INumericParams* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	imath::CVarVector filterLengths = objectPtr->GetValues();

	if (filterLengths.GetElementsCount() >= 1){
		filterLengths[0] = FilterWidthSlider->value() / m_widthScaleFactor;
	}

	if (filterLengths.GetElementsCount() >= 2){
		filterLengths[1] = FilterHeightSlider->value() / m_heightScaleFactor;
	}

	if (filterLengths != objectPtr->GetValues()){
		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SetValues(filterLengths);
	}
}


// protected methods

void CNumericParamsGuiComp::UpdateLabel()
{
	FilterWidthValueLabel->setText(QString::number(FilterWidthSlider->value()));
	FilterHeightValueLabel->setText(QString::number(FilterHeightSlider->value()));
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CNumericParamsGuiComp::OnGuiModelAttached()
{
	imeas::INumericParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		UpdateBlocker updateBlocker(this);

		const imeas::INumericConstraints* constraintsPtr = objectPtr->GetConstraints();

		int filterDimensionsCount = 0;
		if (constraintsPtr != NULL){
			filterDimensionsCount = constraintsPtr->GetFilterDimensionsCount();
		}
		else{
			const imath::CVarVector& filterLengths = objectPtr->GetValues();
			filterDimensionsCount = filterLengths.GetElementsCount();
		}

		if (filterDimensionsCount >= 1){
			QString unitName;
			if (constraintsPtr != NULL){
				FilterWidthLabel->setText(iqt::GetQString(constraintsPtr->GetFilterDescription(0)));

				const imeas::IUnitInfo& unitInfo = constraintsPtr->GetFilterUnitInfo(0);
				unitName = iqt::GetQString(unitInfo.GetUnitName());

				m_widthScaleFactor = unitInfo.GetDisplayMultiplicationFactor();
				istd::CRange valueRange = unitInfo.GetValueRange();

				FilterWidthSlider->setMinimum(int(valueRange.GetMinValue() * m_widthScaleFactor));
				FilterWidthSlider->setMaximum(int(valueRange.GetMaxValue() * m_widthScaleFactor));
			}
			else{
				FilterWidthSlider->setMinimum(1);
				FilterWidthSlider->setMaximum(100);

				m_widthScaleFactor = 1;
			}

			FilterWidthLabel->setVisible(true);
			FilterWidthSlider->setVisible(true);
			FilterWidthValueLabel->setVisible(true);
			WidthUnitNameLabel->setText(unitName);
			WidthUnitNameLabel->setVisible(!unitName.isEmpty());
		}
		else{
			FilterWidthLabel->setVisible(false);
			FilterWidthSlider->setVisible(false);
			FilterWidthValueLabel->setVisible(false);
			WidthUnitNameLabel->setVisible(false);
		}

		if (filterDimensionsCount >= 2){
			QString unitName;
			if (constraintsPtr != NULL){
				FilterHeightLabel->setText(iqt::GetQString(constraintsPtr->GetFilterDescription(1)));

				const imeas::IUnitInfo& unitInfo = constraintsPtr->GetFilterUnitInfo(1);
				unitName = iqt::GetQString(unitInfo.GetUnitName());

				m_heightScaleFactor = unitInfo.GetDisplayMultiplicationFactor();
				istd::CRange valueRange = unitInfo.GetValueRange();

				FilterHeightSlider->setMinimum(int(valueRange.GetMinValue() * m_heightScaleFactor));
				FilterHeightSlider->setMaximum(int(valueRange.GetMaxValue() * m_heightScaleFactor));
			}
			else{
				FilterHeightSlider->setMinimum(1);
				FilterHeightSlider->setMaximum(100);

				m_heightScaleFactor = 1;
			}

			FilterHeightLabel->setVisible(true);
			FilterHeightSlider->setVisible(true);
			FilterHeightValueLabel->setVisible(true);
			HeightUnitNameLabel->setText(unitName);
			HeightUnitNameLabel->setVisible(!unitName.isEmpty());
		}
		else{
			FilterHeightLabel->setVisible(false);
			FilterHeightSlider->setVisible(false);
			FilterHeightValueLabel->setVisible(false);
			HeightUnitNameLabel->setVisible(false);
		}
	}

	BaseClass::OnGuiModelAttached();
}


void CNumericParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	imeas::INumericParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const imath::CVarVector& filterLengths = objectPtr->GetValues();

		int filterDimensionsCount = filterLengths.GetElementsCount();

		if (filterDimensionsCount >= 1){
			FilterWidthSlider->setValue(int(filterLengths[0] * m_widthScaleFactor + 0.5));
		}

		if (filterDimensionsCount >= 2){
			FilterHeightSlider->setValue(int(filterLengths[1] * m_heightScaleFactor + 0.5));
		}

		UpdateLabel();
	}
}


// protected slots

void CNumericParamsGuiComp::on_FilterWidthSlider_valueChanged(int /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();

		UpdateLabel();
	}
}


void CNumericParamsGuiComp::on_FilterHeightSlider_valueChanged(int /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();

		UpdateLabel();
	}
}


} // namespace iqtmeas

