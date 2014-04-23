#ifndef iprop_CPropertiesEditorComp_included
#define iprop_CPropertiesEditorComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iwidgets/CItemDelegate.h"

// ACF-Solutions includes
#include "iprop/IPropertiesManager.h"

#include "GeneratedFiles/iprop/ui_CPropertiesEditorComp.h"


namespace iprop
{


/**
	Tree view based property editor component.
*/
class CPropertiesEditorComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CPropertiesEditorComp, IPropertiesManager>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CPropertiesEditorComp, IPropertiesManager> BaseClass;

	enum ColumnType
	{
		CT_PROPERTY = 0,
		CT_VALUE = 1
	};

	enum DataRole
	{
		DR_PROPERTY_ID = Qt::UserRole + 1
	};

	I_BEGIN_COMPONENT(CPropertiesEditorComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	class EditorDelegate: public iwidgets::CItemDelegate
	{
	public:
		typedef iwidgets::CItemDelegate BaseClass;

		// reimplemented (QItemDelegate)
		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const;
		virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
		virtual void setModelData(QWidget* editor,QAbstractItemModel* model, const QModelIndex& index) const;
	};

private:
	void UpdatePropertyEditor(
				const IProperty& objectProperty,
				const QString& propertyId,
				const QString& propertyDescription);

	QString GetPropertyId(const QTreeWidgetItem& propertyItem) const;

	QTreeWidgetItem* FindPropertyItem(const QString& propertyId) const;

	void SetDataToEditor(const IProperty& objectProperty, QTreeWidgetItem& propertyItem);
};


} // namespace iqtgui


#endif // !iprop_CPropertiesEditorComp_included


