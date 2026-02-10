// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iqtsig_CTriggerParamsGuiComp_included
#define iqtsig_CTriggerParamsGuiComp_included


#include <isig/ITriggerParams.h>

#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <iqtsig/iqtsig.h>

#include <GeneratedFiles/iqtsig/ui_CTriggerParamsGuiComp.h>


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

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached() override;
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

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


