#include "iqtipr/CPatternTeachingControllerGuiComp.h"


// Qt includes
#include <QtGui/QMessageBox>


namespace iqtipr
{


// protected slots

void CPatternTeachingControllerGuiComp::on_LearnButton_clicked()
{
	iipr::IPatternController* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	if (objectPtr->IsPatternValid()){
		int result = QMessageBox::question(
					GetQtWidget(), 
					tr("Replace pattern"),
					tr("The pattern data exists already. Do you want to replace them?"),
					QMessageBox::Ok | QMessageBox::Cancel);

		if (result != QMessageBox::Ok){
			return;
		}
	}

	if (!objectPtr->TeachPattern()){
		QMessageBox::critical(
					GetQtWidget(),
					tr("Error"), 
					tr("Cannot learn pattern"));

		return;
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CPatternTeachingControllerGuiComp::UpdateGui(int /*updateFlags*/)
{
	iipr::IPatternController* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	if (!objectPtr->IsPatternValid()){
		PatternInfoLabel->setText(tr("Reference pattern not defined"));
		PatternInfoLabel->setStyleSheet("color: red;");
	}
	else{
		PatternInfoLabel->setText(tr("Reference pattern defined"));		
		PatternInfoLabel->setStyleSheet("color: darkgreen;");
	}
}


// reimplemented (iqt::CGuiObjectBase)

void CPatternTeachingControllerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (GetObjectPtr() == NULL){
		LearnButton->setVisible(false);
	}
}


} // namespace iqtipr


