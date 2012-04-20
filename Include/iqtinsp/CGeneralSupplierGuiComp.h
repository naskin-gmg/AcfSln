#ifndef iqtinsp_CGeneralSupplierGuiComp_included
#define iqtinsp_CGeneralSupplierGuiComp_included


// ACF includes
#include "iproc/ISupplier.h"
#include "iqtgui/IVisualStatusProvider.h"

// ACF-Solutions includes
#include "iqtinsp/TSupplierGuiCompBase.h"

#include "iqtinsp/Generated/ui_CGeneralSupplierGuiComp.h"


namespace iqtinsp
{


class CGeneralSupplierGuiComp:
			public TSupplierGuiCompBase<Ui::CGeneralSupplierGuiComp, iproc::ISupplier>,
			virtual public iqtgui::IVisualStatusProvider
{
	Q_OBJECT

public:
	typedef TSupplierGuiCompBase<Ui::CGeneralSupplierGuiComp, iproc::ISupplier> BaseClass;

	I_BEGIN_COMPONENT(CGeneralSupplierGuiComp);
		I_REGISTER_INTERFACE(iqtgui::IVisualStatusProvider);
	I_END_COMPONENT;

	// reimplemented (istd::IVisualStatusProvider)
	virtual QIcon GetStatusIcon() const;
	virtual QString GetStatusText() const;

protected Q_SLOTS:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(int updateFlags = 0);

private:
	QIcon m_statusIcon;
	QString m_statusText;
};


} // namespace iqtinsp


#endif // !iqtinsp_CGeneralSupplierGuiComp_included


