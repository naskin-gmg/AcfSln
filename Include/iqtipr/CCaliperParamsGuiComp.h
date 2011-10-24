#ifndef iqtipr_CCaliperParamsGuiComp_included
#define iqtipr_CCaliperParamsGuiComp_included


#include "iipr/ICaliperParams.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtipr/Generated/ui_CCaliperParamsGuiComp.h"


namespace iqtipr
{


class CCaliperParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CCaliperParamsGuiComp,
			iipr::ICaliperParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CCaliperParamsGuiComp,
				iipr::ICaliperParams> BaseClass;

	I_BEGIN_COMPONENT(CCaliperParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

protected Q_SLOTS:
	void on_ThresholdSB_valueChanged(int value);
	void on_EdgePolarityCB_currentIndexChanged(int index);
	void on_DirectionCB_currentIndexChanged(int index);
};


} // namespace iqtipr


#endif // !iqtipr_CCaliperParamsGuiComp_included


