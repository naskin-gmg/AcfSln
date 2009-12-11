#ifndef iqtmeas_CSamplesSequenceViewComp_included
#define iqtmeas_CSamplesSequenceViewComp_included


// Qt includes
#include <QWidget>

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
	I_END_COMPONENT;

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
};


} // namespace iqtmeas


#endif // !iqtmeas_CSamplesSequenceViewComp_included


