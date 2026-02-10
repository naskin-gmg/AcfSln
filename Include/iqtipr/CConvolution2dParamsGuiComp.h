// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iqtipr_CConvolution2dParamsGuiComp_included
#define iqtipr_CConvolution2dParamsGuiComp_included


// ACF includes
#include <iipr/IConvolutionKernel2d.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <GeneratedFiles/iqtipr/ui_CConvolution2dParamsGuiComp.h>


namespace iqtipr
{


class CConvolution2dParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CConvolution2dParamsGuiComp,
			iipr::IConvolutionKernel2d>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CConvolution2dParamsGuiComp,
				iipr::IConvolutionKernel2d> BaseClass;

	I_BEGIN_COMPONENT(CConvolution2dParamsGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;

protected Q_SLOTS:
	void on_KernelWidthSpin_valueChanged(int value);
	void on_KernelHeightSpin_valueChanged(int value);
	void on_KernelTable_itemChanged(QTableWidgetItem* item);

private:
	void UpdateTableSize();
	void UpdateKernelSize() const;
};


} // namespace iqtipr


#endif // !iqtipr_CConvolution2dParamsGuiComp_included


