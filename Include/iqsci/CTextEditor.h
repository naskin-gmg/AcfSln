#ifndef iqsci_CTextEditor_included
#define iqsci_CTextEditor_included


// ACF includes
#include "istd/TDelPtr.h"

#include "ibase/ICommandsProvider.h"

#include "iqtgui/CHierarchicalCommand.h"


// Project includes
#include "iqsci/Generated/ui_CTextEditor.h"


class QsciScintilla;
class QsciLexer;


namespace iqsci
{


class CTextEditor: public QWidget, public Ui::CTextEditor, virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef QWidget BaseClass;

	CTextEditor(QWidget* parentWidget = NULL);

	virtual QString GetText() const;
	virtual void SetText(const QString& text);

	virtual void OnRetranslate();

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

public Q_SLOTS:
	void SetFoldingEnabled(bool useFoldingEnabled);
	void SetLineNumberEnabled(bool useFoldingEnabled);
	void SetLanguage(const QString& language);
	void SetReadOnly(bool readOnly = true);

protected Q_SLOTS:
	virtual void OnSelectionChanged();
	virtual void OnTextChanged();
	virtual void OnToLowercase();
	virtual void OnToUppercase();

Q_SIGNALS:
	void DataChanged();

private:
	typedef std::map<QString, QsciLexer*> LexerMap;

	LexerMap m_languages;

private:
	void RegisterLexers();

private:
	enum MenuFlags
	{
		MF_VIEW = 0x9977,
		MF_EDIT
	};

	iqtgui::CHierarchicalCommand m_rootCommand;
	iqtgui::CHierarchicalCommand m_editorCommand;
	iqtgui::CHierarchicalCommand m_viewCommand;
	iqtgui::CHierarchicalCommand m_languageCommand;
	iqtgui::CHierarchicalCommand m_lowercaseCommand;
	iqtgui::CHierarchicalCommand m_uppercaseCommand;

	// view commands
	iqtgui::CHierarchicalCommand m_useIdentGuideCommand;
	iqtgui::CHierarchicalCommand m_useFoldingCommand;
	iqtgui::CHierarchicalCommand m_showLineNumberCommand;

	QsciScintilla* m_scintilla;
};


} // namespace iqsci


#endif // !iqsci_CTextEditor_included



