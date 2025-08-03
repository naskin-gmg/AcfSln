#ifndef iqtcam_CExposureParamsGuiComp_included
#define iqtcam_CExposureParamsGuiComp_included


// ACF includes
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <icam/IExposureParams.h>
#include <GeneratedFiles/iqtcam/ui_CExposureParamsGuiComp.h>


namespace iqtcam
{


class CExposureParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CExposureParamsGuiComp,
			icam::IExposureParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CExposureParamsGuiComp,
				icam::IExposureParams> BaseClass;

	I_BEGIN_COMPONENT(CExposureParamsGuiComp);
		I_ASSIGN(m_exposureLimitAttrPtr, "ExposureLimit", "Limit Exposure if necessary, [usec]", false, 1000);
		I_ASSIGN(m_fixedLabelSizeAttrPtr, "FixedLabelSize", "Fix label sizes to the value", false, 100);
		I_ASSIGN(m_hideUnusedParamsAttrPtr, "HideUnusedParams", "By default, unused params are hidden. If set to false, they will be greyed out.", true, true);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiCreated() override;
	virtual void OnGuiModelAttached() override;
	virtual void OnGuiModelDetached() override;
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

protected Q_SLOTS:
	void OnParamsChanged(double value);
	void OnParamsChanged(int value);

private:
	void AdjustVisibility();

	I_ATTR(int, m_exposureLimitAttrPtr);
	I_ATTR(int, m_fixedLabelSizeAttrPtr);
	I_ATTR(bool, m_hideUnusedParamsAttrPtr);
};


} // namespace iqtcam


#endif // !iqtcam_CExposureParamsGuiComp_included


