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
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void OnParamsChanged(double value);
};


} // namespace iqtcam


#endif // !iqtcam_CExposureParamsGuiComp_included


