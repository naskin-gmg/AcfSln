// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iqtipr_CSearchBasedFeaturesSupplierGuiComp_included
#define iqtipr_CSearchBasedFeaturesSupplierGuiComp_included


// ACF includes
#include <imod/IObserver.h>
#include <imod/TModelWrap.h>
#include <imod/CSingleModelObserverBase.h>
#include <i2d/CCircle.h>
#include <iqtgui/IGuiObject.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <iview/TVisualObjects.h>
#include <iview/IShapeFactory.h>
#include <iview/CCircleShape.h>

// ACF-Solutions includes
#include <iqtinsp/TSupplierGuiCompBase.h>
#include <iqtipr/iqtipr.h>

#include <GeneratedFiles/iqtipr/ui_CSearchBasedFeaturesSupplierGuiComp.h>


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
		CT_POSITION,
		CT_ANGLE,
		CT_SCALE,
		CT_LAST = CT_SCALE
	};

	I_BEGIN_COMPONENT(CSearchBasedFeaturesSupplierGuiComp);
		I_ASSIGN(m_intermediateResultsGuiCompPtr, "IntermediateResultsGui", "GUI integrated into group 'Intermediate Results'", false, "IntermediateResultsGui");
		I_ASSIGN(m_showResultShapesAttrPtr, "ShowResultShapes", "Show result shapes in the image as circles", true, true);
	I_END_COMPONENT;

	CSearchBasedFeaturesSupplierGuiComp();

protected:
	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;
	virtual void OnSupplierParamsChanged();

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached() override;
	virtual void OnGuiModelDetached() override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnGuiCreated() override;
	virtual void OnGuiHidden();

	// reimplemented (icomp::IComponentBase)
	virtual void OnComponentDestroyed() override;

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

private:
	void ConnectShapes(iview::IShapeView& view);
	void DisconnectShapes(iview::IShapeView& view);
	void ClearResults();

private:
	I_REF(iqtgui::IGuiObject, m_intermediateResultsGuiCompPtr);
	I_ATTR(bool, m_showResultShapesAttrPtr);

	// result shapes
	typedef iview::TVisualObject<i2d::CCircle, iview::CCircleShape> VisualObject;
	typedef istd::TPointerVector<iview::IShape> VisualObjects;
	VisualObjects m_visualPositions;

	iview::IShapeView* m_lastViewPtr;
};


} // namespace iqtipr


#endif // !iqtipr_CSearchBasedFeaturesSupplierGuiComp_included


