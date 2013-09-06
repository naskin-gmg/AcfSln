#ifndef iqtinsp_CGeneralSupplierGuiComp_included
#define iqtinsp_CGeneralSupplierGuiComp_included


// ACF includes
#include "iproc/ISupplier.h"

// ACF-Solutions includes
#include "iqtinsp/TSupplierGuiCompBase.h"

#include "GeneratedFiles/iqtinsp/ui_CGeneralSupplierGuiComp.h"


namespace iqtinsp
{


class CGeneralSupplierGuiComp:
			public TSupplierGuiCompBase<Ui::CGeneralSupplierGuiComp, iproc::ISupplier>
{
	Q_OBJECT

public:
	typedef TSupplierGuiCompBase<Ui::CGeneralSupplierGuiComp, iproc::ISupplier> BaseClass;

	I_BEGIN_COMPONENT(CGeneralSupplierGuiComp);
	I_END_COMPONENT;

	CGeneralSupplierGuiComp();

protected Q_SLOTS:
	void OnAutoTest();
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

Q_SIGNALS:
	void DoAutoTest();

protected:
	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;
	virtual void OnSupplierParamsChanged();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiHidden();
	virtual void UpdateGui(int updateFlags = 0);
};


} // namespace iqtinsp


#endif // !iqtinsp_CGeneralSupplierGuiComp_included


