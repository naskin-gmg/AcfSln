#ifndef iqtipr_CPatternTeachingControllerGuiComp_included
#define iqtipr_CPatternTeachingControllerGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"

// ACF-Solutions includes
#include "iipr/IPatternController.h"

#include "GeneratedFiles/iqtipr/ui_CPatternTeachingControllerGuiComp.h"


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
		I_ASSIGN(m_patternEditorCompPtr, "PatternEditor", "Pattern editor", false, "PatternEditor");
		I_ASSIGN_TO(m_patternEditorObserverCompPtr, m_patternEditorCompPtr, false);

		I_ASSIGN(m_patternViewCompPtr, "PatternView", "GUI representing learned pattern", false, "PatternView");
		I_ASSIGN(m_patternViewObserverCompPtr, "PatternObserver", "Pattern GUI observer", false, "PatternObserver");
	I_END_COMPONENT;

protected Q_SLOTS:
	void on_LearnButton_clicked();
	void on_EditButton_clicked();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqt::CGuiObjectBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_patternEditorCompPtr);
	I_REF(imod::IObserver, m_patternEditorObserverCompPtr);

	I_REF(iqtgui::IGuiObject, m_patternViewCompPtr);
	I_REF(imod::IObserver, m_patternViewObserverCompPtr);
};


} // namespace iqtipr


#endif // !iqtipr_CPatternTeachingControllerGuiComp_included


