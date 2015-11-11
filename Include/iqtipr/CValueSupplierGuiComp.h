#ifndef iqtipr_CValueSupplierGuiComp_included
#define iqtipr_CValueSupplierGuiComp_included


// ACF includes
#include "ifile/IFilePersistence.h"
#include "imod/IObserver.h"
#include "imod/TModelWrap.h"
#include "imod/CSingleModelObserverBase.h"
#include "i2d/CCircle.h"
#include "imeas/INumericValueProvider.h"
#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtinsp/TSupplierGuiCompBase.h"

#include "iqtipr/iqtipr.h"

#include "GeneratedFiles/iqtipr/ui_CValueSupplierGuiComp.h"


namespace iqtipr
{


class CValueSupplierGuiComp: public iqtinsp::TSupplierGuiCompBase<Ui::CValueSupplierGuiComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CValueSupplierGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CValueSupplierGuiComp);
		I_ASSIGN(m_intermediateResultsGuiCompPtr, "IntermediateResultsGui", "GUI integrated in group 'Intermediate Results'", false, "IntermediateResultsGui");
	I_END_COMPONENT;
	
	CValueSupplierGuiComp();

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

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
	virtual void OnGuiDestroyed();
	virtual void OnGuiHidden();

	// reimplemented (icomp::IComponentBase)
	virtual void OnComponentDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_intermediateResultsGuiCompPtr);

	typedef imod::TModelWrap<i2d::CCircle> FoundCircleModel;
	typedef imod::TModelWrap<i2d::CLine2d> FoundLineModel;
	FoundCircleModel m_foundCircleModel;
	FoundLineModel m_foundLineModel;

	bool m_isResultVisible;
};


} // namespace iqtipr


#endif // !iqtipr_CValueSupplierGuiComp_included


