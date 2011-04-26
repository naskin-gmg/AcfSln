#ifndef iqwt_CHistogramViewComp_included
#define iqwt_CHistogramViewComp_included


// Qwt includes
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_picker.h"


// ACF includes
#include "istd/TPointerVector.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "imeas/IDataSequence.h"

#include "iqwt/Generated/ui_CHistogramViewComp.h"


namespace iqwt
{


class CHistogramViewComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CHistogramViewComp,
			imeas::IDataSequence>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CHistogramViewComp,
				imeas::IDataSequence> BaseClass;

	I_BEGIN_COMPONENT(CHistogramViewComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private Q_SLOTS:
	void on_ChannelCombo_currentIndexChanged(int index);

private:
	void ClearPlot();

private:
	class HistogramPlotPicker: public QwtPlotPicker
	{
	public:
		typedef QwtPlotPicker BaseClass;

		HistogramPlotPicker(CHistogramViewComp& parent, int xAxis, int yAxis, QwtPlotCanvas* canvasPtr);

		// reimplemented (QwtPlotPicker)
	    virtual QwtText trackerText(const QPoint& position) const;

	private:
		CHistogramViewComp& m_parent;
	};

private:
	istd::TDelPtr<QwtPlot> m_plotPtr;
	istd::TPointerVector<QwtPlotCurve> m_channelCurves;
};


} // namespace iqwt


#endif // !iqwt_CHistogramViewComp_included


