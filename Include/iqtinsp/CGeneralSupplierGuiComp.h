#ifndef iqtinsp_CGeneralSupplierGuiComp_included
#define iqtinsp_CGeneralSupplierGuiComp_included


// ACF includes
#include "iinsp/ISupplier.h"

// ACF-Solutions includes
#include "iqtinsp/TSupplierGuiCompBase.h"

#include "GeneratedFiles/iqtinsp/ui_CGeneralSupplierGuiComp.h"


namespace iqtinsp
{


class CGeneralSupplierGuiComp:
			public TSupplierGuiCompBase<Ui::CGeneralSupplierGuiComp, iinsp::ISupplier>
{
	Q_OBJECT

public:
	typedef TSupplierGuiCompBase<Ui::CGeneralSupplierGuiComp, iinsp::ISupplier> BaseClass;

	I_BEGIN_COMPONENT(CGeneralSupplierGuiComp);
	I_END_COMPONENT;

	CGeneralSupplierGuiComp();

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;
	virtual void OnSupplierParamsChanged();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiHidden();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);
};


} // namespace iqtinsp


#endif // !iqtinsp_CGeneralSupplierGuiComp_included


