#ifndef iqtipr_CColorPatternComparatorGuiComp_included
#define iqtipr_CColorPatternComparatorGuiComp_included


// ACF includes
#include "istd/IInformationProvider.h"

// ACF-Solutions includes
#include "imeas/CGeneralDataSequence.h"
#include "imeas/INumericValueProvider.h"
#include "iqtinsp/TSupplierGuiCompBase.h"

#include "GeneratedFiles/iqtipr/ui_CColorPatternComparatorGuiComp.h"


namespace iqtipr
{


class CColorPatternComparatorGuiComp:
			public iqtinsp::TSupplierGuiCompBase<
						Ui::CColorPatternComparatorGuiComp,
						istd::IInformationProvider>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<
				Ui::CColorPatternComparatorGuiComp,
				istd::IInformationProvider> BaseClass;

	I_BEGIN_COMPONENT(CColorPatternComparatorGuiComp);
		I_ASSIGN(m_taskEnabledIdAttrPtr, "TaskEnabledId", "ID of the task enabled attribute", true, "CheckEnabled");
	I_END_COMPONENT;

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_TaskEnabledCB_toggled(bool);

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiCreated();
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual void OnSupplierParamsChanged();
	virtual QWidget* GetParamsWidget() const;

private:
	I_ATTR(QByteArray, m_taskEnabledIdAttrPtr);
};


} // namespace iqtipr


#endif // !iqtipr_CColorPatternComparatorGuiComp_included


