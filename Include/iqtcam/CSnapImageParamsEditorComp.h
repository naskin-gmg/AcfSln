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


#ifndef iqtcam_CSnapImageParamsEditorComp_included
#define iqtcam_CSnapImageParamsEditorComp_included


// Qt includes
#include <QtCore/QTimer>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#else
#include <QtGui/QAction>
#include <QtGui/QMenu>
#endif

// ACF includes
#include <ifile/IFilePersistence.h>
#include <imod/IModel.h>
#include <imod/IObserver.h>
#include <iimg/IBitmap.h>
#include <icmm/IColorTransformation.h>
#include <iprm/IParamsSet.h>
#include <iqtgui/IGuiObject.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <iqt2d/TViewExtenderCompBase.h>

// ACF-Solutions includes
#include <icam/IBitmapAcquisition.h>

#include <iqtcam/iqtcam.h>

#include <GeneratedFiles/iqtcam/ui_CSnapImageParamsEditorComp.h>


namespace iqtcam
{


/**
	Delegator of parameters integrating snap functionality.
*/
class CSnapImageParamsEditorComp: public iqt2d::TViewExtenderCompBase<
			iqtgui::TDesignerGuiObserverCompBase<Ui::CSnapImageParamsEditorComp, iprm::IParamsSet> >
{
	Q_OBJECT

public:
	typedef iqt2d::TViewExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<Ui::CSnapImageParamsEditorComp, iprm::IParamsSet> > BaseClass;

	I_BEGIN_COMPONENT(CSnapImageParamsEditorComp);
		I_ASSIGN(m_bitmapCompPtr, "Bitmap", "Bitmap will be shown", true, "Bitmap");
		I_ASSIGN_TO(m_bitmapModelCompPtr, m_bitmapCompPtr, true);
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquision obje for image snap", false, "BitmapAcquisition");
		I_ASSIGN(m_lookupTableCompPtr, "LookupTable", "Lookup table for pseudo colors in the image view", false, "LookupTable");
		I_ASSIGN(m_bitmapLoaderCompPtr, "BitmapLoader", "Saves or loads bitmap to/from file", false, "BitmapLoader");
		I_ASSIGN(m_allowBitmapLoadAttrPtr, "AllowBitmapLoad", "If true loading of images is allowed", true, false);
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN_TO(m_paramsSetObserverCompPtr, m_paramsSetGuiCompPtr, false);
		I_ASSIGN_TO(m_paramsSetExtenderCompPtr, m_paramsSetGuiCompPtr, false);
		I_ASSIGN(m_liveIntervalAttrPtr, "LiveInterval", "Interval (in seconds) of acquisition in continuous mode", true, 0.04);
		I_ASSIGN(m_allowSnapOnChangeAttrPtr, "AllowSnapOnChange", "If true then shows action for snap on parameters changed", false, false);
	I_END_COMPONENT;

	CSnapImageParamsEditorComp();

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

protected:
	class ParamsObserver: public imod::CSingleModelObserverBase
	{
	public:
		ParamsObserver(CSnapImageParamsEditorComp* parentPtr);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	private:
		CSnapImageParamsEditorComp& m_parent;
	};

	bool SnapImage();
	void UpdateButtonsState();

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiHidden();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected Q_SLOTS:
	void on_SnapImageButton_clicked();
	void on_LoadImageButton_clicked();
	void on_SaveImageButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
	void OnIntervalSnap(bool checked);
	void OnSnapOnChanges(bool checked);
	void OnTimerReady();

private:
	I_REF(iimg::IBitmap, m_bitmapCompPtr);
	I_REF(imod::IModel, m_bitmapModelCompPtr);
	I_REF(icam::IBitmapAcquisition, m_bitmapAcquisitionCompPtr);
	I_REF(icmm::IColorTransformation, m_lookupTableCompPtr);

	I_REF(ifile::IFilePersistence, m_bitmapLoaderCompPtr);
	I_ATTR(bool, m_allowBitmapLoadAttrPtr);
	I_REF(ifile::IFilePersistence, m_paramsLoaderCompPtr);

	I_REF(iqtgui::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);
	I_REF(iqt2d::IViewExtender, m_paramsSetExtenderCompPtr);

	I_ATTR(double, m_liveIntervalAttrPtr);
	I_ATTR(bool, m_allowSnapOnChangeAttrPtr);

	QTimer m_timer;

	ParamsObserver m_paramsObserver;
};


} // namespace iqtcam


#endif // !iqtcam_CSnapImageParamsEditorComp_included


