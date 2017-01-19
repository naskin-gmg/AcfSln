#ifndef iqtipr_CCircleFinderParamsGuiComp_included
#define iqtipr_CCircleFinderParamsGuiComp_included


#include <iipr/ICircleFinderParams.h>

#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/iqtipr/ui_CCircleFinderParamsGuiComp.h>


namespace iqtipr
{


class CCircleFinderParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CCircleFinderParamsGuiComp,
			iipr::ICircleFinderParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CCircleFinderParamsGuiComp,
				iipr::ICircleFinderParams> BaseClass;

	I_BEGIN_COMPONENT(CCircleFinderParamsGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void on_EnableOutliersElimination_toggled(bool value);
	void on_MinOutliersDistance_valueChanged(double value);
	void on_MaxRaysCount_valueChanged(int value);
	void on_CaliperMode_currentIndexChanged(int index);
};


} // namespace iqtipr


#endif // !iqtipr_CCircleFinderParamsGuiComp_included


