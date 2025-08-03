#ifndef iqtmm_CVideoPlayerGuiComp_included
#define iqtmm_CVideoPlayerGuiComp_included


// ACF includes
#include <imod/IModel.h>
#include <imod/CSingleModelObserverBase.h>
#include <ifile/IFileNameParam.h>
#include <iqtgui/TDesignerGuiCompBase.h>

// ACF-Solutions includes
#include <imm/IMediaController.h>

#include <GeneratedFiles/iqtmm/ui_CVideoPlayerGuiComp.h>


namespace iqtmm
{


/**
	GUI component used to display video player.
*/
class CVideoPlayerGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CVideoPlayerGuiComp>,
			protected imod::CSingleModelObserverBase
{
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CVideoPlayerGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CVideoPlayerGuiComp);
		I_ASSIGN(m_mediaControllerCompPtr, "MediaController", "Controller used to open and display media", true, "MediaController");
		I_ASSIGN(m_videoGuiCompPtr, "VideoGui", "View for a video display", true, "VideoGui");
		I_ASSIGN(m_urlParamsGuiCompPtr, "UrlGui", "Sequence directory selection GUI", false, "UrlGui");
		I_ASSIGN(m_controllerBarGuiCompPtr, "ControllerBarGui", "Displayed on the bottom used typically to controll of playback", false, "ControllerBarGui");
		I_ASSIGN(m_urlParamCompPtr, "UrlParams", "Parameter storing media URL or file or directory", true, "UrlParams");
		I_ASSIGN_TO(m_urlParamModelCompPtr, m_urlParamCompPtr, true);
	I_END_COMPONENT;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_REF(imm::IMediaController, m_mediaControllerCompPtr);
	I_REF(iqtgui::IGuiObject, m_videoGuiCompPtr);
	I_REF(iqtgui::IGuiObject, m_urlParamsGuiCompPtr);
	I_REF(iqtgui::IGuiObject, m_controllerBarGuiCompPtr);
	I_REF(ifile::IFileNameParam, m_urlParamCompPtr);
	I_REF(imod::IModel, m_urlParamModelCompPtr);
};


} // namespace iqtmm


#endif // !iqtmm_CVideoPlayerGuiComp_included

