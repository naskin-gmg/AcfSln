#ifndef iedgegui_CEdgeLinesSupplierEditorComp_included
#define iedgegui_CEdgeLinesSupplierEditorComp_included


// ACF includes
#include <imod/IObserver.h>
#include <imod/TModelWrap.h>
#include <imod/CSingleModelObserverBase.h>
#include <ifile/IFilePersistence.h>
#include <iqtgui/IGuiObject.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <imeas/INumericValueProvider.h>
#include <iedge/CEdgeLineContainer.h>
#include <iqtinsp/TSupplierGuiCompBase.h>
#include <iqtipr/iqtipr.h>

#include <GeneratedFiles/iedgegui/ui_CEdgeLinesSupplierEditorComp.h>



namespace iedgegui
{


class CEdgeLinesSupplierEditorComp: public iqtinsp::TSupplierGuiCompBase<Ui::CEdgeLinesSupplierEditorComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CEdgeLinesSupplierEditorComp> BaseClass;

	I_BEGIN_COMPONENT(CEdgeLinesSupplierEditorComp);
		I_ASSIGN(m_resultsSaverCompPtr, "ResultsSaver", "Saves found edge lines", false, "ResultsSaver");
		I_ASSIGN(m_contoursColorSchemaCompPtr, "EdgeLineColorSchema", "Defines colors for edge lines on view", false, "EdgeLineColorSchema");
	I_END_COMPONENT;

	CEdgeLinesSupplierEditorComp();

protected:
	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;
	virtual void OnSupplierParamsChanged();

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnGuiCreated();
	virtual void OnGuiHidden();

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void on_ProcessButton_clicked();
	void on_SaveDataButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

private:
	I_REF(ifile::IFilePersistence, m_resultsSaverCompPtr);
	I_REF(iview::IColorSchema, m_contoursColorSchemaCompPtr);

	imod::TModelWrap<iedge::CEdgeLineContainer> m_container;

	iview::CColorSchema m_edgesColorSchema;
};


}//namespace iedgegui


#endif // !iedgegui_CEdgeLinesSupplierEditorComp_included


