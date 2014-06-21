#ifndef iqtsig_CTriggerParamsGuiComp_included
#define iqtsig_CTriggerParamsGuiComp_included


#include "isig/ITriggerParams.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtsig/iqtsig.h"

#include "GeneratedFiles/iqtsig/ui_CTriggerParamsGuiComp.h"


namespace iqtsig
{


class CTriggerParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CTriggerParamsGuiComp,
			isig::ITriggerParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CTriggerParamsGuiComp,
				isig::ITriggerParams> BaseClass;

	I_BEGIN_COMPONENT(CTriggerParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void on_NoTriggerRB_toggled(bool state);
	void on_ContinuousRB_toggled(bool state);
	void on_RisingEdgeRB_toggled(bool state);
	void on_FallingEdgeRB_toggled(bool state);
	void on_PositiveLevelRB_toggled(bool state);
	void on_NegativeLevelRB_toggled(bool state);
	void on_SoftwareTriggerRB_toggled(bool state);
};


} // namespace iqtsig


#endif // !iqtsig_CTriggerParamsGuiComp_included


