// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iqtipr_CLineProjectionSupplierGuiComp_included
#define iqtipr_CLineProjectionSupplierGuiComp_included


// ACF includes
#include <ifile/IFilePersistence.h>
#include <imod/IObserver.h>
#include <imod/TModelWrap.h>
#include <i2d/CPosition2d.h>
#include <iqtgui/IGuiObject.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <imeas/IDataSequenceProvider.h>
#include <imeas/CGeneralDataSequence.h>

#include <iqtinsp/TSupplierGuiCompBase.h>

#include <iqtipr/iqtipr.h>

#include <GeneratedFiles/iqtipr/ui_CLineProjectionSupplierGuiComp.h>


namespace iqtipr
{


class CLineProjectionSupplierGuiComp: public iqtinsp::TSupplierGuiCompBase<Ui::CLineProjectionSupplierGuiComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CLineProjectionSupplierGuiComp> BaseClass;

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

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached() override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;

private:
	imod::TModelWrap<imeas::CGeneralDataSequence> m_projectionData;

	I_REF(imod::IObserver, m_projectionObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_projectionObserverGuiCompPtr);
};


} // namespace iqtipr


#endif // !iqtipr_CLineProjectionSupplierGuiComp_included


