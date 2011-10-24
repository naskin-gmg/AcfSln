#ifndef iqtipr_CValueSupplierGuiComp_included
#define iqtipr_CValueSupplierGuiComp_included


// ACF includes
#include "iser/IFileLoader.h"
#include "imod/IObserver.h"
#include "imod/TModelWrap.h"
#include "imod/CSingleModelObserverBase.h"
#include "i2d/CCircle.h"
#include "iproc/IValueProvider.h"
#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtinsp/TSupplierGuiCompBase.h"

#include "iqtipr/iqtipr.h"

#include "iqtipr/Generated/ui_CValueSupplierGuiComp.h"


namespace iqtipr
{


class CValueSupplierGuiComp: public iqtinsp::TSupplierGuiCompBase<Ui::CValueSupplierGuiComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CValueSupplierGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CValueSupplierGuiComp);
		I_ASSIGN(m_intermediateResultsGuiCompPtr, "IntermediateResultsGui", "GUI integreted in group 'Intermediate Results'", false, "IntermediateResultsGui");
	I_END_COMPONENT;

	CValueSupplierGuiComp();

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	class ParamsObserver: public imod::CSingleModelObserverBase
	{
	public:
		ParamsObserver(CValueSupplierGuiComp* parentPtr);

		using imod::CSingleModelObserverBase::EnsureModelDetached;

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CValueSupplierGuiComp& m_parent;
	};

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

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

	imod::TModelWrap<i2d::CCircle> m_foundModel;

	ParamsObserver m_paramsObserver;
};


} // namespace iqtipr


#endif // !iqtipr_CValueSupplierGuiComp_included


