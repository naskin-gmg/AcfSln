#ifndef iqtmeas_CChartBaseDataSequenceViewComp_included
#define iqtmeas_CChartBaseDataSequenceViewComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050800
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#endif

// ACF includes
#include <istd/TRange.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <imeas/IDataSequence.h>
#include <GeneratedFiles/iqtmeas/ui_CChartBaseDataSequenceViewComp.h>


namespace iqtmeas
{


class CChartBaseDataSequenceViewComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CChartBaseDataSequenceViewComp,
			imeas::IDataSequence>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CChartBaseDataSequenceViewComp,
				imeas::IDataSequence> BaseClass;

	I_BEGIN_COMPONENT(CChartBaseDataSequenceViewComp);
	I_END_COMPONENT;

	CChartBaseDataSequenceViewComp();

protected:
	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);
	virtual void OnGuiModelAttached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
#if QT_VERSION >= 0x050800
	QtCharts::QChart m_chart;
#endif
};


} // namespace iqtmeas


#endif // !iqtmeas_CChartBaseDataSequenceViewComp_included


