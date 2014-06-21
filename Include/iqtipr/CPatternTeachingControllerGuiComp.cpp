#include "iqtipr/CPatternTeachingControllerGuiComp.h"


// Qt includes
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include "iqtgui/CGuiComponentDialog.h"


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


void CPatternTeachingControllerGuiComp::on_EditButton_clicked()
{
	iipr::IPatternController* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	const iser::ISerializable* patternPtr = objectPtr->GetPatternObject();
	if (patternPtr != NULL){
		istd::TDelPtr<istd::IChangeable> workingPatternPtr(patternPtr->CloneMe());
		imod::IModel* workingPatternModelPtr = dynamic_cast<imod::IModel*>(workingPatternPtr.GetPtr());
		if (workingPatternModelPtr != NULL){
			if (workingPatternModelPtr->AttachObserver(m_patternEditorObserverCompPtr.GetPtr())){
				int result = 0;
				{
					iqtgui::CGuiComponentDialog editorDialog(m_patternEditorCompPtr.GetPtr(), 
						QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

					editorDialog.resize(800,600);

					result = editorDialog.exec();
				}

				if (result == QDialog::Accepted){
					objectPtr->TeachPattern(workingPatternPtr.GetPtr());
				}
			}
		}
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CPatternTeachingControllerGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
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

	// view
	if (m_patternViewObserverCompPtr.IsValid()){
		imod::IModel* patternModelPtr = const_cast<imod::IModel*>(dynamic_cast<const imod::IModel*>(objectPtr->GetPatternObject()));
		if (patternModelPtr != NULL){
			if (!patternModelPtr->IsAttached(m_patternViewObserverCompPtr.GetPtr())){
				patternModelPtr->AttachObserver(m_patternViewObserverCompPtr.GetPtr());
			}

			istd::CChangeNotifier updateModelPtr(dynamic_cast<istd::IChangeable*>(patternModelPtr));
		}
	}
}


// reimplemented (iqt::CGuiObjectBase)

void CPatternTeachingControllerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (GetObjectPtr() == NULL){
		LearnButton->setVisible(false);
	}

	if (!m_patternEditorCompPtr.IsValid()){
		EditButton->setVisible(false);
	}

	if (m_patternViewCompPtr.IsValid()){
		m_patternViewCompPtr->CreateGui(PatternFrame);
	}
	else{
		PatternFrame->hide();
	}
}


void CPatternTeachingControllerGuiComp::OnGuiDestroyed()
{
	if (m_patternViewCompPtr.IsValid()){
		m_patternViewCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtipr


