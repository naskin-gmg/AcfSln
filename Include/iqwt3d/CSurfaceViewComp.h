#ifndef iqwt3d_CSurfaceViewComp_included
#define iqwt3d_CSurfaceViewComp_included


#include "imath/TISampledFunction.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

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
			imath::ISampledFunction2d>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSurfaceViewComp,
				imath::ISampledFunction2d> BaseClass;

	I_BEGIN_COMPONENT(CSurfaceViewComp);
		I_ASSIGN(m_unitNameAttrPtr, "UnitName", "Name of geometric units e.g. mm", false, "mm");
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected slots:
	void OnParamsChanged(double value);
	void OnToggleAutoScale(bool value);
	void OnToggleShader(bool value);
	void OnPolygonOffset(int value);
	void OnNormalsQuality(int value);
	void OnMeshEnabled(bool isMeshEnabled);

private:
	I_ATTR(istd::CString, m_unitNameAttrPtr);

	Qwt3D::SurfacePlot* m_surfacePlotPtr;
};


} // namespace iqwt3d


#endif // !iqwt3d_CSurfaceViewComp_included


