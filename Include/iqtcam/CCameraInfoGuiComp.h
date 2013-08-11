#ifndef iqtcam_CCameraInfoGuiComp_included
#define iqtcam_CCameraInfoGuiComp_included


// ACF includes
#include "iprm/ISelectionParam.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

// ACF-Solutions includes
#include "icam/ICameraInfo.h"

#include "iqtcam/Generated/ui_CCameraInfoGuiComp.h"


namespace iqtcam
{


class CCameraInfoGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
		Ui::CCameraInfoGuiComp,
		iprm::ISelectionParam>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
		Ui::CCameraInfoGuiComp,
		iprm::ISelectionParam> BaseClass;

	I_BEGIN_COMPONENT(CCameraInfoGuiComp);
		I_ASSIGN(m_cameraInfoProviderComp, "CameraInfo", "Camera information provider", true, "Camera");
	I_END_COMPONENT;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

private:
	void AddItem(int cameraIndex, icam::ICameraInfo::CameraAttribute attr, const QString& attrName);

	I_REF(icam::ICameraInfo, m_cameraInfoProviderComp);
};


} // namespace iqtcam


#endif // !iqtcam_CCameraInfoGuiComp_included
