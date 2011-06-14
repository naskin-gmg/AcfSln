#ifndef iqtmeas_CDataStatisticsGuiComp_included
#define iqtmeas_CDataStatisticsGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"


// ACF-Solutions includes
#include "imeas/IDataStatistics.h"

#include "iqtmeas/Generated/ui_CDataStatisticsGuiComp.h"


namespace iqtmeas
{


class CDataStatisticsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CDataStatisticsGuiComp,
			imeas::IDataStatistics>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CDataStatisticsGuiComp,
				imeas::IDataStatistics> BaseClass;

	I_BEGIN_COMPONENT(CDataStatisticsGuiComp);
	I_END_COMPONENT;

	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);
};


} // namespace iqtmeas


#endif // !iqtmeas_CDataStatisticsGuiComp_included


