#ifndef iqwt3d_CSurfaceViewComp_included
#define iqwt3d_CSurfaceViewComp_included


// ACF includes
#include "imath/TISampledFunction.h"

#include "ibase/ICommandsProvider.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iqwt3d/Generated/ui_CSurfaceViewComp.h"


namespace Qwt3D
{
	class SurfacePlot;
}


namespace iqwt3d
{


/**
	3D-View based on Qwt3D library.
*/
class CSurfaceViewComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CSurfaceViewComp,
			imath::ISampledFunction2d>,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSurfaceViewComp,
				imath::ISampledFunction2d> BaseClass;

	enum{
		GI_QWT3D = 177
	};

	I_BEGIN_COMPONENT(CSurfaceViewComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_enableLightingAttrPtr, "EnableLighting", "Enable lighting for the scene", false, true);
		I_ASSIGN(m_enableOrthoAttrPtr, "EnableOrtho", "EnableOrtho", false, true);
		I_ASSIGN(m_enableAutoScaleAttrPtr, "EnableAutoScale", "Enable auto scaling of scene axes", false, true);
		I_ASSIGN(m_enableMeshAttrPtr, "EnableMesh", "Enable mesh view for the scene", false, true);
		I_ASSIGN(m_enableShaderAttrPtr, "EnableShader", "Enable shader", false, true);
		I_ASSIGN(m_enableLegendAttrPtr, "EnableLegend", "Enable and show the color legend", false, true);
	I_END_COMPONENT;

	CSurfaceViewComp();
	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	virtual void SetupLighting();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
	void OnParamsChanged(double value);
	void OnToggleAutoScale(bool value);
	void OnToggleShader(bool value);
	void OnMeshEnabled(bool isMeshEnabled);
	void OnShowColorLegend(bool showColorLegend);

private:
	I_ATTR(bool, m_enableLightingAttrPtr);
	I_ATTR(bool, m_enableOrthoAttrPtr);
	I_ATTR(bool, m_enableAutoScaleAttrPtr);
	I_ATTR(bool, m_enableMeshAttrPtr);
	I_ATTR(bool, m_enableShaderAttrPtr);
	I_ATTR(bool, m_enableLegendAttrPtr);

	Qwt3D::SurfacePlot* m_surfacePlotPtr;

	iqtgui::CHierarchicalCommand m_rootCommand;
	iqtgui::CHierarchicalCommand m_plotCommands;
	iqtgui::CHierarchicalCommand m_enableLightingCommand;
	iqtgui::CHierarchicalCommand m_enableOrthoCommand;
	iqtgui::CHierarchicalCommand m_enableAutoScaleCommand;
	iqtgui::CHierarchicalCommand m_enableMeshCommand;
	iqtgui::CHierarchicalCommand m_enableShaderCommand;
	iqtgui::CHierarchicalCommand m_enableLegendCommand;
};


} // namespace iqwt3d


#endif // !iqwt3d_CSurfaceViewComp_included


