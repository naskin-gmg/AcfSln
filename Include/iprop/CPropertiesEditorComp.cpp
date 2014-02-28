#include "iprop/CPropertiesEditorComp.h"


// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidgetItemIterator>
#else
#include <QtGui/QHeaderView>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidgetItemIterator>
#endif


// ACF includes
#include "istd/TChangeNotifier.h"

// ACF-Solutions includes
#include "iprop/TProperty.h"


namespace iprop
{


// reimplemented (iqtgui::TGuiObserverWrap)

void CPropertiesEditorComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	IPropertiesManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){

		int propertiesCount = objectPtr->GetPropertiesCount();
		
		for (int propertyIndex = 0; propertyIndex < propertiesCount; propertyIndex++){
			const IProperty* propertyPtr = dynamic_cast<const IProperty*>(objectPtr->GetProperty(propertyIndex));
			if (propertyPtr != NULL){

				QString propertyId = objectPtr->GetPropertyId(propertyIndex);
				QString propertyDescription = objectPtr->GetPropertyDescription(propertyIndex);

				UpdatePropertyEditor(*propertyPtr, propertyId, propertyDescription);
			}
		}
	}
}


// reimplemented (CGuiComponentBase)

void CPropertiesEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	PropertyTree->setSelectionMode(QAbstractItemView::SingleSelection);
	PropertyTree->setRootIsDecorated(false);

#if QT_VERSION >= 0x050000
	PropertyTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
	PropertyTree->header()->setResizeMode(QHeaderView::ResizeToContents);
#endif
	PropertyTree->setItemDelegate(new EditorDelegate());
}


// private methods

void CPropertiesEditorComp::UpdatePropertyEditor(
			const IProperty& objectProperty,
			const QString& propertyId,
			const QString& propertyDescription)
{
	QTreeWidgetItem* propertyItemPtr = FindPropertyItem(propertyId);
	if (propertyItemPtr == NULL){
		propertyItemPtr = new QTreeWidgetItem;
		propertyItemPtr->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);

		propertyItemPtr->setText(CT_PROPERTY, propertyId);
		propertyItemPtr->setData(CT_PROPERTY, DR_PROPERTY_ID, propertyId);
		propertyItemPtr->setStatusTip(CT_PROPERTY, propertyDescription);

		PropertyTree->addTopLevelItem(propertyItemPtr);
	}

	Q_ASSERT(propertyItemPtr != NULL);

	SetDataToEditor(objectProperty, *propertyItemPtr);
}


QString CPropertiesEditorComp::GetPropertyId(const QTreeWidgetItem& propertyItem) const
{
	return propertyItem.data(CT_PROPERTY, DR_PROPERTY_ID).toString(); 
}


QTreeWidgetItem* CPropertiesEditorComp::FindPropertyItem(const QString& propertyId) const
{
	QTreeWidgetItemIterator treeIter(PropertyTree);

	while (*treeIter){
		QTreeWidgetItem* propertyItemPtr = *treeIter;

		Q_ASSERT(propertyItemPtr != NULL);

		QString currentPropertyId = GetPropertyId(*propertyItemPtr);
		if (propertyId == currentPropertyId){
			return propertyItemPtr;
		}

		treeIter++;
	}

	return NULL;
}


void CPropertiesEditorComp::SetDataToEditor(const IProperty& objectProperty, QTreeWidgetItem& propertyItem)
{
	const CBoolProperty* boolPropertyPtr = dynamic_cast<const CBoolProperty*>(&objectProperty);
	if (boolPropertyPtr != NULL){
		bool value = boolPropertyPtr->GetValue();
		propertyItem.setText(CT_VALUE, value ? tr("true") : tr("false"));
	}

	const CIntProperty* intPropertyPtr = dynamic_cast<const CIntProperty*>(&objectProperty);
	if (intPropertyPtr != NULL){
		propertyItem.setText(CT_VALUE, QString("%1").arg(intPropertyPtr->GetValue()));
	}

	const CDoubleProperty* doublePropertyPtr = dynamic_cast<const CDoubleProperty*>(&objectProperty);
	if (doublePropertyPtr != NULL){
		propertyItem.setText(CT_VALUE, QString("%1").arg(doublePropertyPtr->GetValue(), 3, 'f', 3));
	}

	const CStringProperty* stringPropertyPtr = dynamic_cast<const CStringProperty*>(&objectProperty);
	if (stringPropertyPtr != NULL){
		propertyItem.setText(CT_VALUE, stringPropertyPtr->GetValue());
	}

	const CStdStringProperty* idPropertyPtr = dynamic_cast<const CStdStringProperty*>(&objectProperty);
	if (idPropertyPtr != NULL){
		propertyItem.setText(CT_VALUE, idPropertyPtr->GetValue());
	}
}


// reimplemented (QItemDelegate)

QWidget* CPropertiesEditorComp::EditorDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() != CT_VALUE){
		return NULL;
	}

	return BaseClass::createEditor(parent, option, index);
}


void CPropertiesEditorComp::EditorDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	if (index.column() != CT_VALUE){
		return;
	}

	BaseClass::setEditorData(editor, index);
}


void CPropertiesEditorComp::EditorDelegate::setModelData(QWidget* editor,QAbstractItemModel* model, const QModelIndex& index) const
{
	if (index.column() != CT_VALUE){
		return;
	}

	BaseClass::setModelData(editor, model, index);
}


} // namespace iprop


