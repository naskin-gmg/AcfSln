#ifndef iqtsig_CPulseParamsGuiComp_included
#define iqtsig_CPulseParamsGuiComp_included


// ACF includes
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <isig/IPulseParams.h>

#include <iqtsig/iqtsig.h>

#include <GeneratedFiles/iqtsig/ui_CPulseParamsGuiComp.h>


namespace iqtsig
{


class CPulseParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CPulseParamsGuiComp,
			isig::IPulseParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CPulseParamsGuiComp,
				isig::IPulseParams> BaseClass;

	I_BEGIN_COMPONENT(CPulseParamsGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void on_DelayTimeSB_valueChanged(double value);
	void on_PulseTimeSB_valueChanged(double value);
	void on_RelaxingTimeSB_valueChanged(double value);
};


} // namespace iqtsig


#endif // !iqtsig_CPulseParamsGuiComp_included


