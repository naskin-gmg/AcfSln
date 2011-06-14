#ifndef iqtmeas_CDataSequenceViewComp_included
#define iqtmeas_CDataSequenceViewComp_included


// Qt includes
#include <QWidget>
#include <QStandardItemModel>

// ACF includes
#include "istd/CRange.h"
#include "imod/TSingleModelObserverBase.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "imeas/IDataSequence.h"

#include "iqtmeas/Generated/ui_CDataSequenceViewComp.h"


namespace iqtmeas
{


class CDataSequenceViewComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CDataSequenceViewComp,
			imeas::IDataSequence>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CDataSequenceViewComp,
				imeas::IDataSequence> BaseClass;

	I_BEGIN_COMPONENT(CDataSequenceViewComp);
		I_ASSIGN(m_showScalePanelAttrPtr, "IsScalePanelVisible", "Show or hide scale value panel", true, true);
		I_ASSIGN(m_showTimeSpanPanelAttrPtr, "IsTimeSpanPanelVisible", "Show or hide time span panel", true, true);
		I_ASSIGN(m_showChannelPanelAttrPtr, "IsChannelPanelVisible", "Show or hide channel selector panel", true, true);
		I_ASSIGN(m_minValueAttrPtr, "MinValue", "Default minimum value if manual scaling is enabled", true, 0);
		I_ASSIGN(m_maxValueAttrPtr, "MaxValue", "Default maximum value if manual scaling is enabled", true, 1);
		I_ASSIGN(m_isScaledValueAttrPtr, "IsScaledValue", "Default value of automatic value scaling flag", true, true);
		I_ASSIGN(m_minTimeSpanAttrPtr, "MinTimeSpan", "Default begin of time span if manual scaling of time span is enabled", true, 0);
		I_ASSIGN(m_maxTimeSpanAttrPtr, "MaxTimeSpan", "Default end of time span if manual scaling of time span is enabled", true, 1);
		I_ASSIGN(m_isScaledTimeSpanAttrPtr, "IsScaledTimeSpan", "Default value of automatic time span scaling flag", true, true);
	I_END_COMPONENT;

	CDataSequenceViewComp();

	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

protected:
	class DiagramWidget: public QWidget
	{
	public:
		DiagramWidget(QWidget* parentWidgetPtr, CDataSequenceViewComp* parentPtr);

	protected:
		// reimplemented (QWidget)
		virtual void paintEvent(QPaintEvent* event);

	private:
		CDataSequenceViewComp& m_parent;
	};

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected slots:
	void on_TimeSpanCB_toggled(bool state);
	void on_ScaleCB_toggled(bool state);
	void on_TimeBeginSB_valueChanged(double value);
	void on_TimeEndSB_valueChanged(double value);
	void on_ValueMinSB_valueChanged(double value);
	void on_ValueMaxSB_valueChanged(double value);
	void on_ChannelSelectorCB_currentIndexChanged(int index);
	void on_ZoomInButton_clicked();
	void on_ZoomOutButton_clicked();
	void on_PrevButton_clicked();
	void on_NextButton_clicked();

private:
	I_ATTR(bool, m_showScalePanelAttrPtr);
	I_ATTR(bool, m_showTimeSpanPanelAttrPtr);
	I_ATTR(bool, m_showChannelPanelAttrPtr);
	I_ATTR(double, m_minValueAttrPtr);
	I_ATTR(double, m_maxValueAttrPtr);
	I_ATTR(bool, m_isScaledValueAttrPtr);
	I_ATTR(double, m_minTimeSpanAttrPtr);
	I_ATTR(double, m_maxTimeSpanAttrPtr);
	I_ATTR(bool, m_isScaledTimeSpanAttrPtr);

	int m_lastChannelsCount;
	QStandardItemModel m_channelsSelectorModel;
};


} // namespace iqtmeas


#endif // !iqtmeas_CDataSequenceViewComp_included


