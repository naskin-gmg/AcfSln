#ifndef iqtipr_CMultiLineProjectionSupplierGuiComp_included
#define iqtipr_CMultiLineProjectionSupplierGuiComp_included


// ACF includes
#include <iprm/ISelectionParam.h>

// ACF-Solutions includes
#include <imeas/IMultiDataSequenceProvider.h>
#include <imeas/CGeneralDataSequence.h>
#include <iqtinsp/TSupplierGuiCompBase.h>

#include <GeneratedFiles/iqtipr/ui_CMultiLineProjectionSupplierGuiComp.h>


namespace iqtipr
{


class CMultiLineProjectionSupplierGuiComp: public iqtinsp::TSupplierGuiCompBase<Ui::CMultiLineProjectionSupplierGuiComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CMultiLineProjectionSupplierGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CMultiLineProjectionSupplierGuiComp);
		I_ASSIGN(m_projectionObserverCompPtr, "ProjectionObserver", "Observer for the generated line projections", false, "ProjectionObserver");
		I_ASSIGN_TO(m_projectionObserverGuiCompPtr, m_projectionObserverCompPtr, false);
		I_ASSIGN(m_projectionSelectorCompPtr, "ProjectionSelector", "External selection of the current projection", false, "ProjectionSelector");
	I_END_COMPONENT;

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_ProjectionSlider_valueChanged(int value);
	void on_ProjectionSpin_valueChanged(int value);

protected:
	// reimplemented (iqtgui::IGuiObject)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiHidden();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual void OnSupplierParamsChanged();
	virtual QWidget* GetParamsWidget() const;

private:
	void OnProjectionIndexChanged(int index);

private:
	imod::TModelWrap<imeas::CGeneralDataSequence> m_projectionData;

	I_REF(imod::IObserver, m_projectionObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_projectionObserverGuiCompPtr);
	I_REF(iprm::ISelectionParam, m_projectionSelectorCompPtr);
};


} // namespace iqtipr


#endif // !iqtipr_CMultiLineProjectionSupplierGuiComp_included
