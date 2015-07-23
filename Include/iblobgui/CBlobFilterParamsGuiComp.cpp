#include "iblobgui/CBlobFilterParamsGuiComp.h"


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include "istd/CChangeNotifier.h"
#include "iqt/CSignalBlocker.h"

// QSF includes
#include "iblobgui/CBlobFilterGui.h"


namespace iblobgui
{


// reimplemented (imod::IModelEditor)

void CBlobFilterParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	iblob::IBlobFilterParams* paramsPtr = GetObjectPtr();
	Q_ASSERT(paramsPtr != NULL);

	istd::CChangeNotifier notifier(paramsPtr);

	paramsPtr->ResetFilters();

	int guiCount = FilterHolder->layout()->count();
	for (int i = 0; i < guiCount; i++){
		QWidgetItem* itemPtr = dynamic_cast<QWidgetItem*>(FilterHolder->layout()->itemAt(i));
		Q_ASSERT(itemPtr != NULL);

		CBlobFilterGui* filterGui = dynamic_cast<CBlobFilterGui*>(itemPtr->widget());
		Q_ASSERT(filterGui != NULL);

		iblob::IBlobFilterParams::Filter filterInfo = filterGui->GetFilterInfo();
		paramsPtr->AddFilter(filterInfo);
	}
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CBlobFilterParamsGuiComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	iblob::IBlobFilterParams* paramsPtr = GetObjectPtr();
	if (paramsPtr == NULL){
		return;
	}

	istd::CChangeNotifier notifier(paramsPtr);

	iqt::CSignalBlocker block(ActivateFilterCheckBox, true);

	int guiCount = FilterHolder->layout()->count();

	for (int i = 0; i < guiCount; i++){
		QWidgetItem* itemPtr = dynamic_cast<QWidgetItem*>(FilterHolder->layout()->itemAt(i));
		Q_ASSERT(itemPtr != NULL);

		CBlobFilterGui* filterGui = dynamic_cast<CBlobFilterGui*>(itemPtr->widget());
		Q_ASSERT(filterGui != NULL);

		iblob::IBlobFilterParams::Filter filterInfo = filterGui->GetFilterInfo();
		
		const iblob::BlobDescriptorInfoList* descriptorsListPtr = paramsPtr->GetSupportedDescriptorsList();
		if (descriptorsListPtr != NULL){
			filterGui->SetFeatures(*descriptorsListPtr);
		}
	}
}


// protected slots

void CBlobFilterParamsGuiComp::OnFilterParameterChanged()
{
	if (!IsUpdateBlocked() && GetModelPtr() != NULL){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


void CBlobFilterParamsGuiComp::on_ActivateFilterCheckBox_toggled(bool on)
{
	iblob::IBlobFilterParams* paramsPtr = GetObjectPtr();
	if (paramsPtr == NULL){
		return;
	}

	istd::CChangeNotifier notifier(paramsPtr);

	paramsPtr->SetFiltersEnabled(on);

	if (on){
		UpdateGui(istd::IChangeable::GetAllChanges());
	}
}


void CBlobFilterParamsGuiComp::on_AddFilterButton_clicked()
{
	iblob::IBlobFilterParams* paramsPtr = GetObjectPtr();
	if (paramsPtr == NULL){
		return;
	}

	istd::CChangeNotifier notifier(paramsPtr);

	paramsPtr->AddFilter(iblob::IBlobFilterParams::Filter());
}


void CBlobFilterParamsGuiComp::on_RemoveAllButton_clicked()
{
	iblob::IBlobFilterParams* paramsPtr = GetObjectPtr();
	if (paramsPtr == NULL){
		return;
	}

	if (QMessageBox::question(NULL, tr("Clear Filters"), tr("Do you really want to remove all the filters?"),
		QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Yes)
	{
		istd::CChangeNotifier notifier(paramsPtr);

		paramsPtr->ResetFilters();
	}
}


void CBlobFilterParamsGuiComp::OnRemoveFilter(QWidget* filterGui)
{
	if (!IsGuiCreated()){
		return;
	}

	iblob::IBlobFilterParams* paramsPtr = GetObjectPtr();
	if (paramsPtr == NULL){
		return;
	}

	int guiCount = FilterHolder->layout()->count();
	for (int i = 0; i < guiCount; i++){
		QWidgetItem* itemPtr = dynamic_cast<QWidgetItem*>(FilterHolder->layout()->itemAt(i));
		Q_ASSERT(itemPtr != NULL);

		if (filterGui == itemPtr->widget()){
			istd::CChangeNotifier notifier(paramsPtr);

			paramsPtr->RemoveFilterAt(i);

			return;
		}
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CBlobFilterParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	iblob::IBlobFilterParams* paramsPtr = GetObjectPtr();
	if (paramsPtr == NULL){
		return;
	}

	iqt::CSignalBlocker block(ActivateFilterCheckBox, true);

	int filtersCount = paramsPtr->GetFiltersCount();
	ActivateFilterCheckBox->setChecked(paramsPtr->IsFiltersEnabled());

	int guiCount = FilterHolder->layout()->count();
	if (guiCount < filtersCount){
		while (guiCount++ < filtersCount){
			CreateFilter();
		}
	}
	else if (guiCount > filtersCount){
		while (guiCount-- > filtersCount){
			QWidgetItem* itemPtr = dynamic_cast<QWidgetItem*>(FilterHolder->layout()->takeAt(0));
			Q_ASSERT(itemPtr != NULL);

			itemPtr->widget()->setParent(NULL);
			delete itemPtr->widget();

			delete itemPtr;
		}
	}

	for (int i = 0; i < filtersCount; i++){
		const iblob::IBlobFilterParams::Filter& filterInfo = paramsPtr->GetFilterAt(i);

		QWidgetItem* itemPtr = dynamic_cast<QWidgetItem*>(FilterHolder->layout()->itemAt(i));
		Q_ASSERT(itemPtr != NULL);

		CBlobFilterGui* filterGui = dynamic_cast<CBlobFilterGui*>(itemPtr->widget());
		Q_ASSERT(filterGui != NULL);

		filterGui->SetFilterInfo(filterInfo);
	}

	RemoveAllButton->setEnabled(filtersCount);
}


// reimplemented (iqt::CGuiObjectBase)

void CBlobFilterParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
}


void CBlobFilterParamsGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


// private methods

void CBlobFilterParamsGuiComp::CreateFilter()
{
	CBlobFilterGui* filterGui = new CBlobFilterGui();

	FilterHolder->layout()->addWidget(filterGui);

	iblob::IBlobFilterParams* paramsPtr = GetObjectPtr();
	if (paramsPtr != NULL){
		const iblob::BlobDescriptorInfoList* descriptorsListPtr = paramsPtr->GetSupportedDescriptorsList();
		if (descriptorsListPtr != NULL){
			filterGui->SetFeatures(*descriptorsListPtr);
		}
	}

	connect(filterGui, SIGNAL(FilterChanged()), 
		this, SLOT(OnFilterParameterChanged()));
	connect(filterGui, SIGNAL(RemoveFilter(QWidget*)), 
		this, SLOT(OnRemoveFilter(QWidget*)), Qt::QueuedConnection);
}


} // namespace iblobgui
