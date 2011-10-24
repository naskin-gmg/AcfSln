#ifndef iqtcam_CBitmapSupplierGuiComp_included
#define iqtcam_CBitmapSupplierGuiComp_included


// ACF includes
#include "iser/IFileLoader.h"
#include "imod/IObserver.h"
#include "imod/TModelWrap.h"
#include "iqt/CBitmap.h"
#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iipr/IBitmapProvider.h"

#include "iqtinsp/TSupplierGuiCompBase.h"

#include "iqtcam/iqtcam.h"

#include "iqtcam/Generated/ui_CBitmapSupplierGuiComp.h"


namespace iqtcam
{


class CBitmapSupplierGuiComp: public iqtinsp::TSupplierGuiCompBase<Ui::CBitmapSupplierGuiComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CBitmapSupplierGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CBitmapSupplierGuiComp);
		I_ASSIGN(m_bitmapLoaderCompPtr, "BitmapLoader", "Saves bitmap to file", false, "BitmapLoader");
	I_END_COMPONENT;

protected Q_SLOTS:
	void on_SnapImageButton_clicked();
	void on_SaveImageButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	I_REF(iser::IFileLoader, m_bitmapLoaderCompPtr);

	imod::TModelWrap<iqt::CBitmap> m_bitmap;
};


} // namespace iqtcam


#endif // !iqtcam_CBitmapSupplierGuiComp_included


