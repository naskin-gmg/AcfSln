#ifndef iqtipr_CCaliperParamsGuiComp_included
#define iqtipr_CCaliperParamsGuiComp_included


#include <iipr/ICaliperParams.h>

#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/iqtipr/ui_CCaliperParamsGuiComp.h>


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
		I_ASSIGN(m_hideEdgePolarityAttrPtr, "HideEdgePolarityParameterEditor", "If enabled, edge polarity selection editor will be hidden", true, false);
		I_ASSIGN(m_hideDirectionAttrPtr, "HideDirectionParameterEditor", "If enabled, edge search direction editor will be hidden", true, false);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
	void on_ThresholdSB_valueChanged(int value);
	void on_EdgePolarityCB_currentIndexChanged(int index);
	void on_DirectionCB_currentIndexChanged(int index);

private:
	I_ATTR(bool, m_hideEdgePolarityAttrPtr);
	I_ATTR(bool, m_hideDirectionAttrPtr);
};


} // namespace iqtipr


#endif // !iqtipr_CCaliperParamsGuiComp_included


