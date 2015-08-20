#ifndef iqtcam_CSnapImageGuiComp_included
#define iqtcam_CSnapImageGuiComp_included


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
#include "ifile/IFilePersistence.h"
#include "imod/IModel.h"
#include "imod/IObserver.h"
#include "iimg/IBitmap.h"
#include "icmm/IColorTransformation.h"
#include "iprm/IParamsSet.h"
#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiCompBase.h"
#include "iqt2d/TViewExtenderCompBase.h"

// ACF-Solutions includes
#include "icam/IBitmapAcquisition.h"

#include "iqtcam/iqtcam.h"

#include "GeneratedFiles/iqtcam/ui_CSnapImageGuiComp.h"


namespace iqtcam
{


class CSnapImageGuiComp: public iqt2d::TViewExtenderCompBase<iqtgui::TDesignerGuiCompBase<Ui::CSnapImageGuiComp, QWidget> >
{
	Q_OBJECT

public:
	typedef iqt2d::TViewExtenderCompBase<iqtgui::TDesignerGuiCompBase<Ui::CSnapImageGuiComp, QWidget> > BaseClass;

	I_BEGIN_COMPONENT(CSnapImageGuiComp);
		I_ASSIGN(m_bitmapCompPtr, "Bitmap", "Bitmap will be shown", true, "Bitmap");
		I_ASSIGN_TO(m_bitmapModelCompPtr, m_bitmapCompPtr, true);
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquision obje for image snap", false, "BitmapAcquisition");
		I_ASSIGN(m_lookupTableCompPtr, "LookupTable", "Lookup table for pseudo colors in the image view", false, "LookupTable");
		I_ASSIGN(m_bitmapLoaderCompPtr, "BitmapLoader", "Saves or loads bitmap to/from file", false, "BitmapLoader");
		I_ASSIGN(m_allowBitmapLoadAttrPtr, "AllowBitmapLoad", "If true loading of images is allowed", true, false);
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN_TO(m_paramsSetModelCompPtr, m_paramsSetCompPtr, false);
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN_TO(m_paramsSetObserverCompPtr, m_paramsSetGuiCompPtr, false);
		I_ASSIGN_TO(m_paramsSetExtenderCompPtr, m_paramsSetGuiCompPtr, false);
		I_ASSIGN(m_liveIntervalAttrPtr, "LiveInterval", "Interval (in seconds) of acquisition in continuous mode", true, 0.04);
	I_END_COMPONENT;

	CSnapImageGuiComp();

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

protected:
	class ParamsObserver: public imod::CSingleModelObserverBase
	{
	public:
		ParamsObserver(CSnapImageGuiComp* parentPtr);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	private:
		CSnapImageGuiComp& m_parent;
	};

	bool SnapImage();
	void UpdateButtonsState();

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiRetranslate();
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiHidden();

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
	I_REF(istd::IChangeable, m_bitmapCompPtr);
	I_REF(imod::IModel, m_bitmapModelCompPtr);
	I_REF(icam::IBitmapAcquisition, m_bitmapAcquisitionCompPtr);
	I_REF(icmm::IColorTransformation, m_lookupTableCompPtr);

	I_REF(ifile::IFilePersistence, m_bitmapLoaderCompPtr);
	I_ATTR(bool, m_allowBitmapLoadAttrPtr);
	I_REF(ifile::IFilePersistence, m_paramsLoaderCompPtr);

	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_REF(iqtgui::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);
	I_REF(iqt2d::IViewExtender, m_paramsSetExtenderCompPtr);

	I_ATTR(double, m_liveIntervalAttrPtr);

	QTimer m_timer;

	ParamsObserver m_paramsObserver;

	QAction m_intervalSnapAction;
	QAction m_snapOnChangesAction;
	QMenu m_snapButtonMenu;
};


} // namespace iqtcam


#endif // !iqtcam_CSnapImageGuiComp_included


