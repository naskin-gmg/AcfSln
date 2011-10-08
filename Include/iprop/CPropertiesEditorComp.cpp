// Qt includes
#include <QHeaderView>
#include <QToolButton>
#include <QDesktopServices>
#include <QUrl>
#include <QTreeWidgetItemIterator>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprop/TProperty.h"
#include "iprop/CPropertiesEditorComp.h"


namespace iprop
{


// reimplemented (iqtgui::TGuiObserverWrap)

void CPropertiesEditorComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iprop::IPropertiesManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){

		int propertiesCount = objectPtr->GetPropertiesCount();
		
		for(int propertyIndex = 0; propertyIndex < propertiesCount; propertyIndex++){
			const iprop::IProperty* propertyPtr = dynamic_cast<const iprop::IProperty*>(objectPtr->GetProperty(propertyIndex));
			if (propertyPtr != NULL){

				istd::CString propertyId = objectPtr->GetPropertyId(propertyIndex);
				istd::CString propertyDescription = objectPtr->GetPropertyDescription(propertyIndex);

				UpdatePropertyEditor(*propertyPtr, iqt::GetQString(propertyId), iqt::GetQString(propertyDescription));
			}
		}
	}
}


// reimplemented (CGuiComponentBase)

void CPropertiesEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	PropertyTree->setSelectionMode(QAbstractItemView::SingleSelection);
}


// private methods

void CPropertiesEditorComp::UpdatePropertyEditor(
			const iprop::IProperty& objectProperty,
			const QString& propertyId,
			const QString& propertyDescription)
{
	QTreeWidgetItem* propertyItemPtr = FindPropertyItem(propertyId);
	if (propertyItemPtr == NULL){
		propertyItemPtr = new QTreeWidgetItem;

		propertyItemPtr->setText(CT_PROPERTY, propertyId);
		propertyItemPtr->setData(CT_PROPERTY, DR_PROPERTY_ID, propertyId);
		propertyItemPtr->setStatusTip(CT_PROPERTY, propertyDescription);
	}

	I_ASSERT(propertyItemPtr != NULL);

	SetDataToEditor(objectProperty, *propertyItemPtr);
}


QString CPropertiesEditorComp::GetPropertyId(const QTreeWidgetItem& propertyItem) const
{
	return propertyItem.data(CT_PROPERTY, DR_PROPERTY_ID).toString(); 
}


QTreeWidgetItem* CPropertiesEditorComp::FindPropertyItem(const QString propertyId) const
{
	QTreeWidgetItemIterator treeIter(PropertyTree);

	while (*treeIter){
		QTreeWidgetItem* propertyItemPtr = *treeIter;

		I_ASSERT(propertyItemPtr != NULL);

		QString currentPropertyId = GetPropertyId(*propertyItemPtr);
		if (propertyId == currentPropertyId){
			return propertyItemPtr;
		}

		treeIter++;
	}

	return NULL;
}


void CPropertiesEditorComp::SetDataToEditor(const iprop::IProperty& objectProperty, QTreeWidgetItem& propertyItem)
{
	const iprop::CBoolAttribute* boolPropertyPtr = dynamic_cast<const iprop::CBoolAttribute*>(&objectProperty);
	if (boolPropertyPtr != NULL){
		bool value = boolPropertyPtr->GetValue();
		propertyItem.setText(CT_VALUE, value ? tr("true") : tr("false"));
	}

	const iprop::CIntAttribute* intPropertyPtr = dynamic_cast<const iprop::CIntAttribute*>(&objectProperty);
	if (intPropertyPtr != NULL){
		propertyItem.setText(CT_VALUE, QString("%1").arg(intPropertyPtr->GetValue()));
	}

	const iprop::CDoubleAttribute* doublePropertyPtr = dynamic_cast<const iprop::CDoubleAttribute*>(&objectProperty);
	if (doublePropertyPtr != NULL){
		propertyItem.setText(CT_VALUE, QString("%1").arg(doublePropertyPtr->GetValue(), 3, 'f', 3));
	}

	const iprop::CStringAttribute* stringPropertyPtr = dynamic_cast<const iprop::CStringAttribute*>(&objectProperty);
	if (stringPropertyPtr != NULL){
		propertyItem.setText(CT_VALUE, iqt::GetQString(stringPropertyPtr->GetValue()));
	}
}


} // namespace iprop


