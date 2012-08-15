#ifndef iqtipr_CSearchBasedFeaturesSupplierGuiComp_included
#define iqtipr_CSearchBasedFeaturesSupplierGuiComp_included


// ACF includes
#include "imod/IObserver.h"
#include "imod/TModelWrap.h"
#include "imod/CSingleModelObserverBase.h"
#include "i2d/CCircle.h"
#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iview/CInteractiveCircleShape.h"

// ACF-Solutions includes
#include "iqtinsp/TSupplierGuiCompBase.h"

#include "iqtipr/iqtipr.h"

#include "iqtipr/Generated/ui_CSearchBasedFeaturesSupplierGuiComp.h"


namespace iqtipr
{


class CSearchBasedFeaturesSupplierGuiComp: 
			public iqtinsp::TSupplierGuiCompBase<Ui::CSearchBasedFeaturesSupplierGuiComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CSearchBasedFeaturesSupplierGuiComp> BaseClass;

	enum ColumnType
	{
		CT_ID,
		CT_SCORE,
		CT_X,
		CT_Y,
		CT_ANGLE,
		CT_X_SCALE,
		CT_Y_SCALE
	};

	I_BEGIN_COMPONENT(CSearchBasedFeaturesSupplierGuiComp);
		I_ASSIGN(m_intermediateResultsGuiCompPtr, "IntermediateResultsGui", "GUI integrated into group 'Intermediate Results'", false, "IntermediateResultsGui");
	I_END_COMPONENT;

	CSearchBasedFeaturesSupplierGuiComp();

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;
	virtual void OnSupplierParamsChanged();

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnGuiCreated();

	// reimplemented (icomp::IComponentBase)
	virtual void OnComponentDestroyed();

private:
	void ConnectShapes(iview::IShapeView& view);
	void DisconnectShapes(iview::IShapeView& view);

private:
	I_REF(iqtgui::IGuiObject, m_intermediateResultsGuiCompPtr);

	typedef imod::TModelWrap<i2d::CCircle> PositionModel;

	struct VisualObject
	{
		istd::TDelPtr<PositionModel> model;
		istd::TDelPtr<iview::CInteractiveCircleShape> shape;
	};

	typedef istd::TPointerVector<VisualObject> VisualObjects;

	VisualObjects m_visualPositions;

	iview::IShapeView* m_lastViewPtr;
};


} // namespace iqtipr


#endif // !iqtipr_CSearchBasedFeaturesSupplierGuiComp_included


