#ifndef iqtmeas_CNumericParamsGuiComp_included
#define iqtmeas_CNumericParamsGuiComp_included


// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#else
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#endif

// ACF includes
#include "istd/TPointerVector.h"
#include "imod/TModelWrap.h"
#include "imod/CMultiModelDispatcherBase.h"
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
									imod::TSingleModelObserverBase<imeas::INumericValue> > >,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT
public:
	typedef ibase::TModelObserverCompWrap<
				iqtgui::TGuiObserverWrap<
							iqtgui::TGuiComponentBase<QWidget>,
							imod::TSingleModelObserverBase<imeas::INumericValue> > > BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	I_BEGIN_COMPONENT(CNumericParamsGuiComp);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_ASSIGN(m_isSliderVisibleAttrPtr, "SliderVisible", "Enables slider control", true, true);
		I_ASSIGN(m_isButtonsVisibleAttrPtr, "ButtonsVisible", "Enables min/max buttons", true, true);
		I_ASSIGN(m_isSingleRowAttrPtr, "SingleRow", "Single row layout", true, false);
		I_ASSIGN(m_inputPolicyAttrPtr, "InputSizePolicy", "0 - minimal width (default)\n1 - expanding input \n2 - label and input have same width", false, 0);
		I_ASSIGN(m_verticalLayoutAttrPtr, "VerticalLayout", "Vertical or horizontal layout for multiple widgets", true, true);
		I_ASSIGN(m_editorPrecisionAttrPtr, "EditorPrecision", "Precision of the spin editor (-1 - don't change, 0 - integer, 1 - one decimal etc.)", true, 2);
	I_END_COMPONENT;

	virtual ~CNumericParamsGuiComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

public Q_SLOTS:
	void OnValueChanged();

private:
	I_ATTR(bool, m_isSliderVisibleAttrPtr);
	I_ATTR(bool, m_isButtonsVisibleAttrPtr);
	I_ATTR(bool, m_isSingleRowAttrPtr);
	I_ATTR(int, m_inputPolicyAttrPtr);
	I_ATTR(bool, m_verticalLayoutAttrPtr);
	I_ATTR(int, m_editorPrecisionAttrPtr);

	istd::TPointerVector<CNumericValueWidget> m_valueWidgets;
};


} // namespace iqtmeas


#endif // !iqtmeas_CNumericParamsGuiComp_included


