#ifndef iwizgui_CWizardOptionsListGuiComp_included
#define iwizgui_CWizardOptionsListGuiComp_included


// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QComboBox>
#include <QtWidgets/QRadioButton>
#else
#include <QtGui/QComboBox>
#include <QtGui/QRadioButton>
#endif

// ACF includes
#include <istd/TPointerVector.h>

#include <iwiz/IParamsManagerWizard.h>

#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/iwizgui/ui_CWizardOptionsListGuiComp.h>


namespace iwizgui
{


/**
	An editor for an exclusive option selection implemented as a list of options.
*/
class CWizardOptionsListGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CWizardOptionsListGuiComp, iwiz::IParamsManagerWizard>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CWizardOptionsListGuiComp, iwiz::IParamsManagerWizard> BaseClass;

	enum DataRole
	{
		DR_PAGE_INDEX = Qt::UserRole + 1,
	};

	I_BEGIN_COMPONENT(CWizardOptionsListGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (CGuiComponentBase)
	void OnGuiCreated();

protected Q_SLOTS:
	void on_OptionsList_itemSelectionChanged();

private:
	void CreateOptionsTree(const iwiz::IParamsManagerWizard* paramsManagerPtr, QTreeWidgetItem* parentItemPtr = NULL);
};


} // namespace iwizgui


#endif // !iwizgui_CWizardOptionsListGuiComp_included


