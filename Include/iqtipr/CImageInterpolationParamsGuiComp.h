#ifndef iqtipr_CImageInterpolationParamsGuiComp_included
#define iqtipr_CImageInterpolationParamsGuiComp_included


// ACF includes
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <iipr/IImageInterpolationParams.h>
#include <GeneratedFiles/iqtipr/ui_CImageInterpolationParamsGuiComp.h>


namespace iqtipr
{


class CImageInterpolationParamsGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CImageInterpolationParamsGuiComp,
						iipr::IImageInterpolationParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CImageInterpolationParamsGuiComp,
				iipr::IImageInterpolationParams> BaseClass;

	I_BEGIN_COMPONENT(CImageInterpolationParamsGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiRetranslate() override;

protected Q_SLOTS:
	void on_InterpolationModeCombo_currentIndexChanged(int index);
};


} // namespace iqtipr


#endif // !iqtipr_CImageInterpolationParamsGuiComp_included


