#include "iqtipr/CCaliperParamsGuiComp.h"


// ACF includes
#include "istd/CChangeGroup.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CCaliperParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	iipr::ICaliperParams* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	double threshold = ThresholdSB->value() * 0.01;
	if (qAbs(objectPtr->GetWeightThreshold() - threshold) > I_BIG_EPSILON){
		objectPtr->SetWeightThreshold(threshold);
	}

	int edgePolarity = EdgePolarityCB->currentIndex();
	if (objectPtr->GetPolarityMode() != edgePolarity){
		objectPtr->SetPolarityMode(edgePolarity);
	}

	int direction = DirectionCB->currentIndex();
	if (objectPtr->GetDirectionMode() != direction){
		objectPtr->SetDirectionMode(direction);
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CCaliperParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	iipr::ICaliperParams* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		double threshold = objectPtr->GetWeightThreshold();
		ThresholdSB->setValue(int(threshold * 100 + 0.5));

		int edgePolarity = objectPtr->GetPolarityMode();
		EdgePolarityCB->setCurrentIndex(edgePolarity);

		int direction = objectPtr->GetDirectionMode();
		DirectionCB->setCurrentIndex(direction);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CCaliperParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	EdgePolarityCB->setHidden(*m_hideEdgePolarityAttrPtr);
	EdgePolarityLabel->setHidden(*m_hideEdgePolarityAttrPtr);

	DirectionCB->setHidden(*m_hideDirectionAttrPtr);
	DirectionLabel->setHidden(*m_hideDirectionAttrPtr);
}


// protected slots

void CCaliperParamsGuiComp::on_ThresholdSB_valueChanged(int /*value*/)
{
	DoUpdateModel();
}


void CCaliperParamsGuiComp::on_EdgePolarityCB_currentIndexChanged(int /*index*/)
{
	DoUpdateModel();
}


void CCaliperParamsGuiComp::on_DirectionCB_currentIndexChanged(int /*index*/)
{
	DoUpdateModel();
}


} // namespace iqtipr


