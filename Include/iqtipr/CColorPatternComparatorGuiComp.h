#ifndef iqtipr_CColorPatternComparatorGuiComp_included
#define iqtipr_CColorPatternComparatorGuiComp_included


// ACF includes
#include <istd/IInformationProvider.h>

// ACF-Solutions includes
#include <imeas/CGeneralDataSequence.h>
#include <iipr/IFeaturesProvider.h>
#include <iqtinsp/TSupplierGuiCompBase.h>

#include <GeneratedFiles/iqtipr/ui_CColorPatternComparatorGuiComp.h>


namespace iqtipr
{


class CColorPatternComparatorGuiComp: public iqtinsp::TSupplierGuiCompBase<Ui::CColorPatternComparatorGuiComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CColorPatternComparatorGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CColorPatternComparatorGuiComp);
	I_END_COMPONENT;

	CColorPatternComparatorGuiComp();

signals:
	void OnSupplierParamsChangedSignal();

protected Q_SLOTS:
	void on_TestButton_clicked();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual void OnSupplierParamsChanged();
	virtual QWidget* GetParamsWidget() const;
};


} // namespace iqtipr


#endif // !iqtipr_CColorPatternComparatorGuiComp_included


