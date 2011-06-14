#ifndef iqwt_CHistogramViewComp_included
#define iqwt_CHistogramViewComp_included


// Qwt includes
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_picker.h"


// ACF includes
#include "istd/TPointerVector.h"

#include "imod/CModelProxy.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "imeas/IDataSequence.h"
#include "imeas/IDataSequenceStatistics.h"

#include "iqwt/Generated/ui_CHistogramViewComp.h"


namespace iqwt
{


/**
	Component for visualization iof histogram data.
*/
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
		I_ASSIGN(m_dataStatisticsGuiCompPtr, "StatisticsGui", "GUI for the data statistics", false, "StatisticsGui");
		I_ASSIGN_TO(m_dataStatisticsObserverCompPtr, m_dataStatisticsGuiCompPtr, false);
		I_ASSIGN(m_dataSequenceStatisticsCompPtr, "DataSequenceStatistics", "Data sequence statistics", false, "DataSequenceStatistics");
	I_END_COMPONENT;

	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

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
	
	imod::CModelProxy m_curentChannelStatisticsModel;

	I_REF(iqtgui::IGuiObject, m_dataStatisticsGuiCompPtr);
	I_REF(imod::IObserver, m_dataStatisticsObserverCompPtr);
	I_REF(imeas::IDataSequenceStatistics, m_dataSequenceStatisticsCompPtr);
};


} // namespace iqwt


#endif // !iqwt_CHistogramViewComp_included


