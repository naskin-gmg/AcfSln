#include "edgetest/CEdgeLinesTestGuiComp.h" 


// ACF includes
#include "iqtgui/CGuiComponentDialog.h"

#include "iedgegui/CEdgeLineContainerShape.h"


namespace edgetest
{


// protected methods

// reimplemented (iqt2d::TViewExtenderCompBase)

CEdgeLinesTestGuiComp::CEdgeLinesTestGuiComp(){
	initModel();
}

void CEdgeLinesTestGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	iedgegui::CEdgeLineContainerShape* shapePtr = new iedgegui::CEdgeLineContainerShape;
	if (shapePtr != NULL){
		shapePtr->AssignToLayer(iview::IViewLayer::LT_BACKGROUND);

		result.PushBack(shapePtr);

		m_model.AttachObserver(shapePtr);
	}
}


void CEdgeLinesTestGuiComp::initModel(){
//clean the container

	m_model.Reset();

//fill the container with random lines

	for(int c=0; c<2; c++){
		iedge::CEdgeLine line;
		for(int p=0; p<6; p++){
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


// private slots

void CEdgeLinesTestGuiComp::on_CreateLinesButton_clicked()
{
    // QMessageBox::information(this->GetQtWidget(),"HelloWorld","HelloWorld");

	initModel();
}


} // namespace edgetest


