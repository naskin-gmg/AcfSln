#ifndef iprop_CPropertiesEditorComp_included
#define iprop_CPropertiesEditorComp_included


// Qt includes
#include <QtGui/QTreeWidget>


// ACF includes
#include "iprop/IPropertiesManager.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iprop/Generated/ui_CPropertiesEditorComp.h"


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


