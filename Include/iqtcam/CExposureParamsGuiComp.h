#ifndef iqtcam_CExposureParamsGuiComp_included
#define iqtcam_CExposureParamsGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "icam/IExposureParams.h"

#include "iqtcam/Generated/ui_CExposureParamsGuiComp.h"


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

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected Q_SLOTS:
	void OnParamsChanged(double value);
};


} // namespace iqtcam


#endif // !iqtcam_CExposureParamsGuiComp_included


