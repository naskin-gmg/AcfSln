#ifndef iqtmeas_CNumericParamsGuiComp_included
#define iqtmeas_CNumericParamsGuiComp_included


// Qt includes
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>


// ACF includes
#include "istd/TPointerVector.h"

#include "imod/TModelWrap.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"


// ACF-Solutions includes
#include "imeas/INumericValue.h"

#include "iqtmeas/CNumericValueWidget.h"


namespace iqtmeas
{


class CNumericParamsGuiComp:
			public ibase::TModelObserverCompWrap<
						iqtgui::TGuiObserverWrap<
									iqtgui::TGuiComponentBase<QWidget>,
									imod::TSingleModelObserverBase<imeas::INumericValue> > >
{
	Q_OBJECT
public:
	typedef ibase::TModelObserverCompWrap<
				iqtgui::TGuiObserverWrap<
							iqtgui::TGuiComponentBase<QWidget>,
							imod::TSingleModelObserverBase<imeas::INumericValue> > > BaseClass;

	I_BEGIN_COMPONENT(CNumericParamsGuiComp);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_ASSIGN(m_isSliderVisibleAttrPtr, "SliderVisible", "Enables slider control", true, true);
		I_ASSIGN(m_isButtonsVisibleAttrPtr, "ButtonsVisible", "Enables min/max buttons", true, true);
		I_ASSIGN(m_inputPolicyAttrPtr, "InputSizePolicy", "0 - minimal width (default)\n1 - expanding input \n2 - label and input have same width", true, 0);
	I_END_COMPONENT;

	virtual ~CNumericParamsGuiComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiDestroyed();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int changeFlags);

public Q_SLOTS:
	void OnValueChanged();

private:
	I_ATTR(bool, m_isSliderVisibleAttrPtr);
	I_ATTR(bool, m_isButtonsVisibleAttrPtr);
	I_ATTR(int, m_inputPolicyAttrPtr);

	istd::TPointerVector<CNumericValueWidget> m_valueWidgets;
};


} // namespace iqtmeas


#endif // !iqtmeas_CNumericParamsGuiComp_included


