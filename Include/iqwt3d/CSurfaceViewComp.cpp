#include "iqwt3d/CSurfaceViewComp.h"


// Qwt3d includes
#include "qwt3d_surfaceplot.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqwt3d
{


// public methods

CSurfaceViewComp::CSurfaceViewComp()
	:m_plotCommands("", 100, ibase::ICommand::CF_GLOBAL_MENU, GI_QWT3D),
	m_enableLightingCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_ONOFF  | ibase::ICommand::CF_TOOLBAR, GI_QWT3D),
	m_enableOrthoCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_ONOFF, GI_QWT3D),
	m_enableAutoScaleCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_ONOFF, GI_QWT3D),
	m_enableMeshCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_ONOFF | ibase::ICommand::CF_TOOLBAR, GI_QWT3D),
	m_enableShaderCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_ONOFF, GI_QWT3D),
	m_enableLegendCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_ONOFF, GI_QWT3D)
{
	m_plotCommands.InsertChild(&m_enableOrthoCommand, false);
	m_plotCommands.InsertChild(&m_enableAutoScaleCommand, false);
	m_plotCommands.InsertChild(&m_enableLegendCommand, false);
	m_plotCommands.InsertChild(&m_enableShaderCommand, false);
	m_plotCommands.InsertChild(&m_enableLightingCommand, false);
	m_plotCommands.InsertChild(&m_enableMeshCommand, false);

	m_plotCommands.SetVisuals(tr("&View"), tr("&View"), tr("3D-plot menu"));

	m_enableLightingCommand.SetVisuals(tr("Enable Lighting"), tr("Enable Lighting"), tr("Enable lighting effects in the 3D-plot"), QIcon(":/Icons/Qwt3d"));
	m_enableOrthoCommand.SetVisuals(tr("Enable Orthogonal View"), tr("Enable Orthogonal View"), tr("Enable orthogonal view in the 3D-plot"));
	m_enableAutoScaleCommand.SetVisuals(tr("Enable Auto Scale"), tr("Enable Auto Scale"), tr("Enable auto scale of the plot"));
	m_enableMeshCommand.SetVisuals(tr("Show Mesh"), tr("Show Mesh"), tr("Enable mesh drawing on the plot"), QIcon(":/Icons/FilledMesh3d"));
	m_enableShaderCommand.SetVisuals(tr("Enable Shader"), tr("Enable Shader"), tr("Enable shader"));
	m_enableLegendCommand.SetVisuals(tr("Show Legend"), tr("Show Legend"), tr("Show plot legend"));

	m_rootCommand.InsertChild(&m_plotCommands, false);
}


// reimplemenented (iqtgui::TGuiObserverWrap)

void CSurfaceViewComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	imath::ISampledFunction2d* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		I_ASSERT(objectPtr->GetArgumentDimensionality() == 2);
		I_ASSERT(objectPtr->GetResultDimensionality() == 1);

		int width = objectPtr->GetGridSize(0);
		int height = objectPtr->GetGridSize(1);

		if ((width * height) <= 0){
			return;
		}

		double** dataPtr = new double*[height];
		for (int y = 0; y < height; y++){
			dataPtr[y] = new double[width];

			for (int x = 0; x < width; x++){
				imath::ISampledFunction2d::ArgumentType index;
				index.SetAt(0, x);
				index.SetAt(1, y);

				dataPtr[y][x] = objectPtr->GetValueAt(index).GetElement(0);
			}
		}

		istd::CRange xAxisRange = objectPtr->GetLogicalRange(0);
		istd::CRange yAxisRange = objectPtr->GetLogicalRange(1);

		m_surfacePlotPtr->loadFromData(
				dataPtr,
				height, 
				width,
				yAxisRange.GetMinValue(), 
				yAxisRange.GetMaxValue(),
				xAxisRange.GetMinValue(), 
				xAxisRange.GetMaxValue());

		for (int y = 0; y < height; y++){
			delete [] dataPtr[y];
		}
	
		delete [] dataPtr;

		m_surfacePlotPtr->updateData();
		m_surfacePlotPtr->updateGL();
	}
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CSurfaceViewComp::GetCommands() const
{
	return &m_rootCommand;
}


// protected methods

void CSurfaceViewComp::SetupLighting()
{
	m_surfacePlotPtr->setShininess(1);
	m_surfacePlotPtr->setMaterialComponent(GL_EMISSION, 0.1);
	m_surfacePlotPtr->setMaterialComponent(GL_SPECULAR, 0.1);
	m_surfacePlotPtr->setMaterialComponent(GL_AMBIENT_AND_DIFFUSE, 0.1);
    m_surfacePlotPtr->setCoordinateStyle(Qwt3D::BOX);

	m_surfacePlotPtr->setLightShift(200, 0, 0, 0);
	m_surfacePlotPtr->setLightShift(0, 200, 0, 1);
	m_surfacePlotPtr->setLightShift(0, 0, 200, 2);
	m_surfacePlotPtr->setLightShift(-200, 0, 0, 3);
	m_surfacePlotPtr->setLightShift(0, -200, 0, 4);
	m_surfacePlotPtr->setLightShift(0, 0, -200, 5);

	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0, 0.5, 0.5, 0.2, 0);
	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0.5, 0, 0.5, 0.2, 1);
	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0, 0.5, 0.5, 0.2, 2);
	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0, 0.5, 0.5, 0.2, 3);
	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0.5, 0, 0.5, 0.2, 4);
	m_surfacePlotPtr->setLightComponent(GL_LINEAR_ATTENUATION, 0, 0.5, 0.5, 0.2, 5);

	m_surfacePlotPtr->illuminate(0);
	m_surfacePlotPtr->illuminate(1);
	m_surfacePlotPtr->illuminate(2);
	m_surfacePlotPtr->illuminate(3);
	m_surfacePlotPtr->illuminate(4);
	m_surfacePlotPtr->illuminate(5);

	m_surfacePlotPtr->updateGL();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSurfaceViewComp::OnGuiCreated()
{
	m_surfacePlotPtr = new Qwt3D::SurfacePlot(SurfacePlotFrame);
	QLayout* layoutPtr = SurfacePlotFrame->layout();
	if (layoutPtr == NULL){
		layoutPtr = new QVBoxLayout(SurfacePlotFrame);
	}
	layoutPtr->addWidget(m_surfacePlotPtr);

	m_surfacePlotPtr->setBackgroundColor(Qwt3D::RGBA(0, 0, 0));

	QString axisXName = "X"; 
	QString axisYName = "Y";
	QString axisZName = "Z";

	Qwt3D::RGBA axisColor(192,192,192);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X1].setColor(axisColor);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X2].setColor(axisColor);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X3].setColor(axisColor);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X4].setColor(axisColor);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y1].setColor(axisColor);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y2].setColor(axisColor);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y3].setColor(axisColor);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y4].setColor(axisColor);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z1].setColor(axisColor);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z2].setColor(axisColor);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z3].setColor(axisColor);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z4].setColor(axisColor);

	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X1].setLabelString(axisXName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X2].setLabelString(axisXName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X3].setLabelString(axisXName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::X4].setLabelString(axisXName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y1].setLabelString(axisYName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y2].setLabelString(axisYName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y3].setLabelString(axisYName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Y4].setLabelString(axisYName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z1].setLabelString(axisZName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z2].setLabelString(axisZName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z3].setLabelString(axisZName);
	m_surfacePlotPtr->coordinates()->axes[Qwt3D::Z4].setLabelString(axisZName);

	SetupLighting();

	if (m_enableAutoScaleAttrPtr.IsValid()){
		OnToggleAutoScale(*m_enableAutoScaleAttrPtr);

		m_enableAutoScaleCommand.setChecked(*m_enableAutoScaleAttrPtr);
	}

	if (m_enableMeshAttrPtr.IsValid()){
		OnMeshEnabled(*m_enableMeshAttrPtr);

		m_enableMeshCommand.setChecked(*m_enableMeshAttrPtr);
	}

	if (m_enableOrthoAttrPtr.IsValid()){
		m_surfacePlotPtr->setOrtho(*m_enableOrthoAttrPtr);

		m_enableOrthoCommand.setChecked(m_surfacePlotPtr->ortho());
	}

	if (m_enableLegendAttrPtr.IsValid()){
		m_surfacePlotPtr->showColorLegend(*m_enableLegendAttrPtr);

		m_enableLegendCommand.setChecked(*m_enableLegendAttrPtr);
	}

	if (m_enableLightingAttrPtr.IsValid()){
		m_surfacePlotPtr->enableLighting(*m_enableLightingAttrPtr);

		m_enableLightingCommand.setChecked(*m_enableLightingAttrPtr);
	}

	if (m_enableShaderAttrPtr.IsValid()){
		OnToggleShader(*m_enableShaderAttrPtr);

		m_enableShaderCommand.setChecked(*m_enableShaderAttrPtr);
	}

	connect(&m_enableOrthoCommand, SIGNAL(toggled(bool)), m_surfacePlotPtr, SLOT(setOrtho(bool)));
	connect(&m_enableAutoScaleCommand, SIGNAL(toggled(bool)), this, SLOT(OnToggleAutoScale(bool)));
	connect(&m_enableLegendCommand, SIGNAL(toggled(bool)), this, SLOT(OnShowColorLegend(bool)));
	connect(&m_enableShaderCommand, SIGNAL(toggled(bool)), this, SLOT(OnToggleShader(bool)));
	connect(&m_enableLightingCommand, SIGNAL(toggled(bool) ), m_surfacePlotPtr, SLOT(enableLighting(bool)));
	connect(&m_enableMeshCommand, SIGNAL(toggled(bool) ), this, SLOT(OnMeshEnabled(bool)));

	BaseClass::OnGuiCreated();
}


// protected slots

void CSurfaceViewComp::OnParamsChanged(double /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


void CSurfaceViewComp::OnToggleAutoScale(bool value)
{
	I_ASSERT(m_surfacePlotPtr != NULL);

	m_surfacePlotPtr->coordinates()->setAutoScale(value);

	m_surfacePlotPtr->updateGL();
}


void CSurfaceViewComp::OnToggleShader(bool value)
{
	I_ASSERT(m_surfacePlotPtr != NULL);

	Qwt3D::SHADINGSTYLE shadingStyle = value ? Qwt3D::GOURAUD : Qwt3D::FLAT;

	m_surfacePlotPtr->setShading(shadingStyle);
}


void CSurfaceViewComp::OnMeshEnabled(bool isMeshEnabled)
{
	I_ASSERT(m_surfacePlotPtr != NULL);

	m_surfacePlotPtr->setSmoothMesh(isMeshEnabled);

	if (isMeshEnabled){
		m_surfacePlotPtr->setMeshColor(Qwt3D::RGBA(0.6, 0.6, 0.6, 0.5));
	}
	else{
		m_surfacePlotPtr->setMeshColor(Qwt3D::RGBA(0, 0, 0, 0));
	}

	m_surfacePlotPtr->updateData();
    m_surfacePlotPtr->updateGL();
}


void CSurfaceViewComp::OnShowColorLegend(bool showColorLegend)
{
	I_ASSERT(m_surfacePlotPtr != NULL);

	m_surfacePlotPtr->showColorLegend(showColorLegend);
}


} // namespace iqwt3d


