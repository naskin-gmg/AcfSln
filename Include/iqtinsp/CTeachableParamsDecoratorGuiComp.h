// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iqtinsp_CTeachableParamsDecoratorGuiComp_included
#define iqtinsp_CTeachableParamsDecoratorGuiComp_included


// ACF includes
#include <iprm/IParamsSet.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <iinsp/IModelTeachController.h>

#include <GeneratedFiles/iqtinsp/ui_CTeachableParamsDecoratorGuiComp.h>


namespace iqtinsp
{


class CTeachableParamsDecoratorGuiComp: public iqtgui::TDesignerGuiObserverCompBase<Ui::CTeachableParamsDecoratorGuiComp, iprm::IParamsSet>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CTeachableParamsDecoratorGuiComp, iprm::IParamsSet> BaseClass;

	I_BEGIN_COMPONENT(CTeachableParamsDecoratorGuiComp);
		I_ASSIGN(m_slaveEditorCompPtr, "SlaveEditor", "Slave editor will be displayed inside", true, "SlaveEditor");
		I_ASSIGN_TO(m_slaveGuiCompPtr, m_slaveEditorCompPtr, true);
		I_ASSIGN_TO(m_slaveObserverCompPtr, m_slaveEditorCompPtr, true);
		I_ASSIGN(m_teachControllerCompPtr, "TeachController", "Teach controller for observed parameters", true, "TeachController");
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(const istd::IChangeable::ChangeSet& changeSet);
	virtual void UpdateModelFromEditor() const;
	virtual bool IsReadOnly() const;
	virtual void SetReadOnly(bool state);

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
	virtual bool OnModelDetached(imod::IModel* modelPtr);

protected:
	void UpdateButtonsState();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;

protected Q_SLOTS:
	void on_TeachModelButton_clicked();
	void on_ResetModelButton_clicked();

private:
	I_REF(imod::IModelEditor, m_slaveEditorCompPtr);
	I_REF(iqtgui::IGuiObject, m_slaveGuiCompPtr);
	I_REF(imod::IObserver, m_slaveObserverCompPtr);
	I_REF(iinsp::IModelTeachController, m_teachControllerCompPtr);
};


} // namespace iqtinsp


#endif // !iqtinsp_CTeachableParamsDecoratorGuiComp_included


