#ifndef edgetest_CHelloWorldGuiComp_included
#define edgetest_CHelloWorldGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiCompBase.h" 
#include "iqt2d/TViewExtenderCompBase.h"
#include "iqt2d/IViewExtender.h"
#include "iedge/CEdgeLineContainer.h"

#include "Generated/ui_CEdgeLinesTestGuiComp.h" 
 

namespace edgetest
{


/**
	Component used to test visualization of edge lines.
*/
class CEdgeLinesTestGuiComp: 
			public iqt2d::TViewExtenderCompBase< iqtgui::TDesignerGuiCompBase<Ui::CEdgeLinesTestGuiComp> >
{
 
	 Q_OBJECT
 public:
	 typedef iqt2d::TViewExtenderCompBase< iqtgui::TDesignerGuiCompBase<Ui::CEdgeLinesTestGuiComp> > BaseClass;
 
	 I_BEGIN_COMPONENT(CEdgeLinesTestGuiComp);
	 I_REGISTER_INTERFACE(iqt2d::IViewExtender);
		//place for attributes, references, factories
		I_ASSIGN(m_aboutDialogPtr, "AboutDialog", "Gui object shown as about window", false, "AboutDialog");
	 I_END_COMPONENT;

	 CEdgeLinesTestGuiComp();
protected:
	/**
		Fill the container with random lines.
	*/
	void GenerateLines();

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);
 
 private Q_SLOTS:
	 /**
		The function will be triggered on clicking the button.
	*/
	void on_CreateLinesButton_clicked();

private:
	I_REF(IGuiObject, m_aboutDialogPtr); 

	imod::TModelWrap<iedge::CEdgeLineContainer> m_model;
};


} // namespace edgetest


#endif // !edgetest_CHelloWorldGuiComp_included


