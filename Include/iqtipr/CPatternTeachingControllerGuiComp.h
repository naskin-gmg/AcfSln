#ifndef iqtipr_CPatternTeachingControllerGuiComp_included
#define iqtipr_CPatternTeachingControllerGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"

// ACF-Solutions includes
#include "iipr/IPatternController.h"

#include "Generated/ui_CPatternTeachingControllerGuiComp.h"


namespace iqtipr
{


class CPatternTeachingControllerGuiComp:
		public iqtgui::TDesignerGuiObserverCompBase<
				Ui::CPatternTeachingControllerGuiComp,
				iipr::IPatternController>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
			Ui::CPatternTeachingControllerGuiComp,
			iipr::IPatternController> BaseClass;

	I_BEGIN_COMPONENT(CPatternTeachingControllerGuiComp);
	I_END_COMPONENT;

protected Q_SLOTS:
	void on_LearnButton_clicked();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqt::CGuiObjectBase)
	virtual void OnGuiCreated();
};


} // namespace iqtipr


#endif // !iqtipr_CPatternTeachingControllerGuiComp_included


