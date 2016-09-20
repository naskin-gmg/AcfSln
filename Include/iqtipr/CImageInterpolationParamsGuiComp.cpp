#include <iqtipr/CImageInterpolationParamsGuiComp.h>


// ACF includes
#include <istd/CChangeGroup.h>


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CImageInterpolationParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	iipr::IImageInterpolationParams* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		istd::CChangeGroup changeGroup(objectPtr);
		Q_UNUSED(changeGroup);

		int currentIndex = InterpolationModeCombo->currentIndex();

		objectPtr->SetInterpolationMode(InterpolationModeCombo->itemData(currentIndex).toInt());
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CImageInterpolationParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	iipr::IImageInterpolationParams* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int mode = objectPtr->GetInterpolationMode();
		switch (mode)
		{
		case iipr::IImageInterpolationParams::IM_NO_INTERPOLATION:
			InterpolationModeCombo->setCurrentIndex(0);
			break;

		case iipr::IImageInterpolationParams::IM_BILINEAR:
			InterpolationModeCombo->setCurrentIndex(1);
			break;

		case iipr::IImageInterpolationParams::IM_BICUBIC:
			InterpolationModeCombo->setCurrentIndex(2);
			break;
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CImageInterpolationParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
}


void CImageInterpolationParamsGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	InterpolationModeCombo->setItemData(0, iipr::IImageInterpolationParams::IM_NO_INTERPOLATION);
	InterpolationModeCombo->setItemData(1, iipr::IImageInterpolationParams::IM_BILINEAR);
	InterpolationModeCombo->setItemData(2, iipr::IImageInterpolationParams::IM_BICUBIC);
}


// protected slots

void CImageInterpolationParamsGuiComp::on_InterpolationModeCombo_currentIndexChanged(int /*index*/)
{
	DoUpdateModel();
}


} // namespace iqtipr


