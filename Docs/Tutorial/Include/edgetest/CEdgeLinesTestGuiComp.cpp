#include "edgetest/CEdgeLinesTestGuiComp.h" 


// ACF includes
#include "iqtgui/CGuiComponentDialog.h"

#include "iedgegui/CEdgeLineContainerShape.h"


namespace edgetest
{


CEdgeLinesTestGuiComp::CEdgeLinesTestGuiComp()
{
	GenerateLines();
}


// protected methods

void CEdgeLinesTestGuiComp::GenerateLines()
{
	m_model.Reset();

	for (int c=0; c<2; c++){
		iedge::CEdgeLine line;
		for (int p=0; p<6; p++){
			double x = rand() & 255;
			double y = rand() & 255;
			double dx = (rand() & 255) - 127.5;
			double dy = (rand() & 255) - 127.5;
			double w = rand() * 1.0 / RAND_MAX;
			
			i2d::CVector2d pos(x,y), dif(dx,dy);

			iedge::CEdgeNode node(pos, dif, w);
			line.InsertNode(node);
		}
		m_model.PushBack(line);
	}
}


// reimplemented (iqt2d::TViewExtenderCompBase)

void CEdgeLinesTestGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	iedgegui::CEdgeLineContainerShape* shapePtr = new iedgegui::CEdgeLineContainerShape;
	if (shapePtr != NULL){
		shapePtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);

		result.PushBack(shapePtr);

		m_model.AttachObserver(shapePtr);
	}
}


// private slots

void CEdgeLinesTestGuiComp::on_CreateLinesButton_clicked()
{
	GenerateLines();
}


} // namespace edgetest


