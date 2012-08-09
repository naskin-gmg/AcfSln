#ifndef iedgegui_CEdgeLinesSupplierGuiComp_included
#define iedgegui_CEdgeLinesSupplierGuiComp_included


// ACF includes
#include "iser/IFileLoader.h"
#include "imod/IObserver.h"
#include "imod/TModelWrap.h"
#include "imod/CSingleModelObserverBase.h"
#include "imeas/INumericValueProvider.h"
#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtinsp/TSupplierGuiCompBase.h"

#include "iqtipr/iqtipr.h"

#include "Generated/ui_CEdgeLinesSupplierGuiComp.h"

#include "iedge/CEdgeLine.h"



namespace iedgegui{

class CEdgeLinesSupplierGuiComp: public iqtinsp::TSupplierGuiCompBase<Ui::CEdgeLinesSupplierGuiComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CEdgeLinesSupplierGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CEdgeLinesSupplierGuiComp);
		I_ASSIGN(m_intermediateResultsGuiCompPtr, "IntermediateResultsGui", "GUI integreted in group 'Intermediate Results'", false, "IntermediateResultsGui");
	I_END_COMPONENT;

	CEdgeLinesSupplierGuiComp();

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	class ParamsObserver: public imod::CSingleModelObserverBase
	{
	public:
		ParamsObserver(CEdgeLinesSupplierGuiComp* parentPtr);

		using imod::CSingleModelObserverBase::EnsureModelDetached;

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CEdgeLinesSupplierGuiComp& m_parent;
	};

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (icomp::IComponentBase)
	virtual void OnComponentDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_intermediateResultsGuiCompPtr);

	//imod::TModelWrap<i2d::CCircle> m_foundModel;
	imod::TModelWrap<iedge::CEdgeLine::Container> m_foundModel;

	ParamsObserver m_paramsObserver;
};

}//namespace iedgegui



#endif // !iedgegui_CEdgeLinesSupplierGuiComp_included


