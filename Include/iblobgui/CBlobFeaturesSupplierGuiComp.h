/********************************************************************************
**
**	Copyright (C) 2007-2017 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF-Solutions Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org or write info@imagingtools.de for further
** 	information about the ACF.
**
********************************************************************************/


#pragma once

// ACF includes
#include <imod/IObserver.h>
#include <imod/TModelWrap.h>
#include <imod/CSingleModelObserverBase.h>
#include <i2d/CPolygon.h>
#include <iqtgui/IGuiObject.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <iview/TVisualObjects.h>
#include <iview/IShapeFactory.h>
#include <iview/IViewEventObserver.h>
#include <iview/CPolygonShape.h>
#include <iview/CStarShape.h>
#include <iqtinsp/TSupplierGuiCompBase.h>
#include <iqtipr/iqtipr.h>

#include <GeneratedFiles/iblobgui/ui_CBlobFeaturesSupplierGuiComp.h>


namespace iblobgui
{


class CBlobFeaturesSupplierGuiComp: 
			public iqtinsp::TSupplierGuiCompBase<Ui::CBlobFeaturesSupplierGuiComp>,
			virtual public iview::IViewEventObserver
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CBlobFeaturesSupplierGuiComp> BaseClass;

	enum ColumnType
	{
		CT_ID,
		CT_POSITION,
		CT_AREA,
		CT_DIAMETER,
		CT_CIRCULARITY,
		CT_PERIMETER,
		CT_ANGLE,
		CT_LAST = CT_ANGLE
	};

	I_BEGIN_COMPONENT(CBlobFeaturesSupplierGuiComp);
		I_REGISTER_INTERFACE(iview::IViewEventObserver);
		I_ASSIGN(m_intermediateResultsGuiCompPtr, "IntermediateResultsGui", "GUI integrated into group 'Intermediate Results'", false, "IntermediateResultsGui");
		I_ASSIGN(m_showResultShapesAttrPtr, "ShowResultShapes", "Show result shapes in the image as circles", true, true);
		I_ASSIGN(m_paramsBoxHeightAttrPtr, "ParamsMaxHeight", "Max height of params box", false, 400);
	I_END_COMPONENT;

	CBlobFeaturesSupplierGuiComp();

Q_SIGNALS:
	void OnSupplierParamsChangedSignal();

protected:
	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;
	virtual void OnSupplierParamsChanged();

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

	// reimplemented (iview::IViewEventObserver)
	virtual bool OnSelectChange(const iview::IShapeView& view, const istd::CIndex2d& position, const iview::IInteractiveShape& shape, bool state) override;
	virtual bool OnViewMouseButton(const iview::IShapeView& view, const istd::CIndex2d& position, Qt::MouseButton buttonType, bool state, const iview::IInteractiveShape* shapePtr) override;
	virtual bool OnViewMouseMove(
		const iview::IShapeView& view,
		const istd::CIndex2d& position) override;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnGuiCreated();
	virtual void OnGuiHidden();
	virtual void OnGuiDesignChanged();

	// reimplemented (icomp::IComponentBase)
	virtual void OnComponentDestroyed();

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
	void on_ResultsList_itemSelectionChanged();

private:
	void ConnectShapes(iview::IShapeView& view);
	void DisconnectShapes(iview::IShapeView& view);
	void ClearResults();

private:
	I_REF(iqtgui::IGuiObject, m_intermediateResultsGuiCompPtr);
	I_ATTR(bool, m_showResultShapesAttrPtr);
	I_ATTR(int, m_paramsBoxHeightAttrPtr);

	// result shapes
	typedef iview::TPointVisualObject<i2d::CPolygon, iview::CPolygonShape> VisualObject;
	typedef istd::TPointerVector<iview::IShape> VisualObjects;
	VisualObjects m_visualPositions;

	QMap<VisualObject*, QTreeWidgetItem*> m_shapeToItemMap;

	iview::IShapeView* m_lastViewPtr = nullptr;
};


} // namespace iblobgui
