#ifndef iqtcam_CSnapImageGuiComp_included
#define iqtcam_CSnapImageGuiComp_included


// Qt includes
#include <QTimer>


// ACF includes
#include "iser/IFileLoader.h"

#include "imod/IModel.h"
#include "imod/IObserver.h"

#include "iimg/IBitmap.h"

#include "icmm/IColorTransformation.h"

#include "iprm/IParamsSet.h"

#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiCompBase.h"
#include "iqt2d/TSceneExtenderCompBase.h"

#include "iproc/IBitmapAcquisition.h"

#include "iqtcam/iqtcam.h"

#include "iqtcam/Generated/ui_CSnapImageGuiComp.h"


namespace iqtcam
{


class CSnapImageGuiComp: public iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiCompBase<Ui::CSnapImageGuiComp, QWidget> >
{
	Q_OBJECT

public:
	typedef iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiCompBase<Ui::CSnapImageGuiComp, QWidget> > BaseClass;

	I_BEGIN_COMPONENT(CSnapImageGuiComp);
		I_ASSIGN(m_bitmapCompPtr, "Bitmap", "Bitmap will be shown", true, "Bitmap");
		I_ASSIGN_TO(m_bitmapModelCompPtr, m_bitmapCompPtr, true);
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquision obje for image snap", false, "BitmapAcquisition");
		I_ASSIGN(m_lookupTableCompPtr, "LookupTable", "Lookup table for pseudo colors in the image view", false, "LookupTable");
		I_ASSIGN(m_bitmapLoaderCompPtr, "BitmapLoader", "Saves bitmap to file", false, "BitmapLoader");
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN_TO(m_paramsSetModelCompPtr, m_paramsSetCompPtr, false);
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN_TO(m_paramsSetObserverCompPtr, m_paramsSetGuiCompPtr, false);
		I_ASSIGN_TO(m_paramsSetExtenderCompPtr, m_paramsSetGuiCompPtr, false);
		I_ASSIGN(m_liveIntervalAttrPtr, "LiveInterval", "Interval (in seconds) of acquisition in continuous mode", true, 0.04);
	I_END_COMPONENT;

	CSnapImageGuiComp();

	// reimplemented (iqt2d::ISceneExtender)
	virtual void AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr);

protected:
	bool SnapImage();

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected Q_SLOTS:
	void on_SnapImageButton_clicked();
	void on_LiveImageButton_toggled(bool checked);
	void on_SaveImageButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
	void OnTimerReady();

private:
	I_REF(iimg::IBitmap, m_bitmapCompPtr);
	I_REF(imod::IModel, m_bitmapModelCompPtr);
	I_REF(iproc::IBitmapAcquisition, m_bitmapAcquisitionCompPtr);
	I_REF(icmm::IColorTransformation, m_lookupTableCompPtr);

	I_REF(iser::IFileLoader, m_bitmapLoaderCompPtr);
	I_REF(iser::IFileLoader, m_paramsLoaderCompPtr);

	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_REF(iqtgui::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);
	I_REF(iqt2d::ISceneExtender, m_paramsSetExtenderCompPtr);

	I_ATTR(double, m_liveIntervalAttrPtr);

	QTimer m_timer;
};


} // namespace iqtcam


#endif // !iqtcam_CSnapImageGuiComp_included


