#include "iqtmeas/CDataStatisticsGuiComp.h"


namespace iqtmeas
{


// reimplemenented (iqtgui::TGuiObserverWrap)

void CDataStatisticsGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	imeas::IDataStatistics* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		AverageLabel->setText(QString("%1").arg(objectPtr->GetAverage(), 4, 'f', 4));
		MedianLabel->setText(QString("%1").arg(objectPtr->GetMedian(), 4, 'f', 4));
		StandardDeviationLabel->setText(QString("%1").arg(objectPtr->GetStandardDeviation(), 4, 'f', 4));
		MinimumLabel->setText(QString("%1").arg(objectPtr->GetDataBoundaries().GetMinValue(), 4, 'f', 4));
		MaximumLabel->setText(QString("%1").arg(objectPtr->GetDataBoundaries().GetMaxValue(), 4, 'f', 4));
	}
}


} // namespace iqtmeas


