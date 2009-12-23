#ifndef iqtmeas_CSamplesSequenceViewComp_included
#define iqtmeas_CSamplesSequenceViewComp_included


// Qt includes
#include <QWidget>
#include <QStandardItemModel>

// ACF includes
#include "istd/CRange.h"
#include "imod/TSingleModelObserverBase.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "imeas/ISamplesSequence.h"

#include "iqtmeas/Generated/ui_CSamplesSequenceViewComp.h"


namespace iqtmeas
{


class CSamplesSequenceViewComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CSamplesSequenceViewComp,
			imeas::ISamplesSequence>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSamplesSequenceViewComp,
				imeas::ISamplesSequence> BaseClass;

	I_BEGIN_COMPONENT(CSamplesSequenceViewComp);
		I_ASSIGN(m_showScalePanelAttrPtr, "IsScalePanelVisible", "Show or hide scale value panel", true, true);
		I_ASSIGN(m_showTimeSpanPanelAttrPtr, "IsTimeSpanPanelVisible", "Show or hide time span panel", true, true);
		I_ASSIGN(m_defaultMinValueAttrPtr, "DefaultMinValue", "Default minimum value if manual scaling is enabled", true, 0);
		I_ASSIGN(m_defaultMaxValueAttrPtr, "DefaultMaxValue", "Default maximum value if manual scaling is enabled", true, 1);
		I_ASSIGN(m_defaultScaledValueAttrPtr, "DefaultIsScaled", "Default value of automatic scaling flag", true, true);
	I_END_COMPONENT;

	CSamplesSequenceViewComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected:
	class DiagramWidget: public QWidget
	{
	public:
		DiagramWidget(QWidget* parentWidgetPtr, CSamplesSequenceViewComp* parentPtr);

	protected:
		// reimplemented (QWidget)
		virtual void paintEvent(QPaintEvent* event);

	private:
		CSamplesSequenceViewComp& m_parent;
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
	I_ATTR(int, m_defaultMinValueAttrPtr);
	I_ATTR(int, m_defaultMaxValueAttrPtr);
	I_ATTR(bool, m_defaultScaledValueAttrPtr);

	int m_lastChannelsCount;
	QStandardItemModel m_channelsSelectorModel;
};


} // namespace iqtmeas


#endif // !iqtmeas_CSamplesSequenceViewComp_included


