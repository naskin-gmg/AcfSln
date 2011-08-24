#ifndef iqtmeas_CNumericParamsGuiComp_included
#define iqtmeas_CNumericParamsGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"

// ACF-Solutions includes
#include "imeas/INumericParams.h"

#include "iqtmeas/Generated/ui_CNumericParamsGuiComp.h"


namespace iqtmeas
{


class CNumericParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CNumericParamsGuiComp,
			imeas::INumericParams>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CNumericParamsGuiComp,
				imeas::INumericParams> BaseClass;

	I_BEGIN_COMPONENT(CNumericParamsGuiComp)
	I_END_COMPONENT

	CNumericParamsGuiComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	void UpdateLabel();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(int updateFlags = 0);

protected Q_SLOTS:
	void on_FilterWidthSlider_valueChanged(int value);
	void on_FilterHeightSlider_valueChanged(int value);

private:
	double m_widthScaleFactor;
	double m_heightScaleFactor;
};


} // namespace iqtmeas


#endif // !iqtmeas_CNumericParamsGuiComp_included


