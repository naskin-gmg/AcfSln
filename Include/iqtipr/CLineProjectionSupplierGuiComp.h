#ifndef iqtipr_CLineProjectionSupplierGuiComp_included
#define iqtipr_CLineProjectionSupplierGuiComp_included


// ACF includes
#include "iser/IFileLoader.h"
#include "imod/IObserver.h"
#include "imod/TModelWrap.h"
#include "i2d/CPosition2d.h"
#include "iproc/IValueProvider.h"
#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

// ACF-Solutions includes
#include "imeas/CGeneralDataSequence.h"

#include "iipr/IDataSequenceProvider.h"

#include "iqtinsp/TSupplierGuiCompBase.h"

#include "iqtipr/iqtipr.h"

#include "iqtipr/Generated/ui_CLineProjectionSupplierGuiComp.h"


namespace iqtipr
{


class CLineProjectionSupplierGuiComp: public iqtinsp::TSupplierGuiCompBase<
			Ui::CLineProjectionSupplierGuiComp,
			iipr::IDataSequenceProvider>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<
				Ui::CLineProjectionSupplierGuiComp,
				iipr::IDataSequenceProvider> BaseClass;

	I_BEGIN_COMPONENT(CLineProjectionSupplierGuiComp);
		I_ASSIGN(m_projectionObserverCompPtr, "ProjectionObserver", "Observer for the generated line projection", true, "ProjectionObserver");
		I_ASSIGN_TO(m_projectionObserverGuiCompPtr, m_projectionObserverCompPtr, true);
	I_END_COMPONENT;

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
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

private:
	imod::TModelWrap<imeas::CGeneralDataSequence> m_projectionData;

	I_REF(imod::IObserver, m_projectionObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_projectionObserverGuiCompPtr);
};


} // namespace iqtipr


#endif // !iqtipr_CLineProjectionSupplierGuiComp_included


