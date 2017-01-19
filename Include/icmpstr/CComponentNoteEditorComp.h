#ifndef icmpstr_CComponentNoteEditorComp_included
#define icmpstr_CComponentNoteEditorComp_included


// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include <imod/CMultiModelDispatcherBase.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <icmpstr/IElementSelectionInfo.h>

#include <GeneratedFiles/icmpstr/ui_CComponentNoteEditorComp.h>


namespace icmpstr
{


class CComponentNoteEditorComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CComponentNoteEditorComp, IElementSelectionInfo>,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CComponentNoteEditorComp, IElementSelectionInfo> BaseClass;

	I_BEGIN_COMPONENT(CComponentNoteEditorComp);
	I_END_COMPONENT;

	CComponentNoteEditorComp();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void on_NoteEditor_textChanged();

private:
	QTextCursor m_lastCursorPosition;
	bool m_textWasChanged;
};


} // namespace icmpstr


#endif // !icmpstr_CComponentNoteEditorComp_included

