// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtinsp/CTeachableParamsDecoratorGuiComp.h>


namespace iqtinsp
{


// reimplemented (imod::IModelEditor)

void CTeachableParamsDecoratorGuiComp::UpdateEditor(const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(IsGuiCreated());

	UpdateButtonsState();

	if (m_slaveEditorCompPtr.IsValid()){
		m_slaveEditorCompPtr->UpdateEditor(changeSet);
	}
}


void CTeachableParamsDecoratorGuiComp::UpdateModelFromEditor() const
{
	if (m_slaveEditorCompPtr.IsValid()){
		m_slaveEditorCompPtr->UpdateModelFromEditor();
	}
}


bool CTeachableParamsDecoratorGuiComp::IsReadOnly() const
{
	if (m_slaveEditorCompPtr.IsValid()){
		return m_slaveEditorCompPtr->IsReadOnly();
	}

	return true;
}


void CTeachableParamsDecoratorGuiComp::SetReadOnly(bool state)
{
	if (m_slaveEditorCompPtr.IsValid()){
		m_slaveEditorCompPtr->SetReadOnly(state);
	}
}


// reimplemented (imod::IObserver)

bool CTeachableParamsDecoratorGuiComp::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	bool retVal = BaseClass::OnModelAttached(modelPtr, changeMask);

	if (retVal){
		if (m_slaveObserverCompPtr.IsValid()){
			modelPtr->AttachObserver(m_slaveObserverCompPtr.GetPtr());
		}
	}

	return retVal;
}


bool CTeachableParamsDecoratorGuiComp::OnModelDetached(imod::IModel* modelPtr)
{
	bool retVal = BaseClass::OnModelDetached(modelPtr);

	if (retVal){
		if (m_slaveObserverCompPtr.IsValid() && m_slaveObserverCompPtr->IsModelAttached(modelPtr)){
			modelPtr->DetachObserver(m_slaveObserverCompPtr.GetPtr());
		}

		if (IsGuiCreated()){
			UpdateButtonsState();
		}
	}

	return retVal;
}


// protected methods

void CTeachableParamsDecoratorGuiComp::UpdateButtonsState()
{
	Q_ASSERT(IsGuiCreated());

	bool isTeachAllowed = false;
	bool isModelValid = false;

	iprm::IParamsSet* objectPtr = GetObservedObject();
	if (m_teachControllerCompPtr.IsValid() && (objectPtr != NULL)){
		isTeachAllowed = m_teachControllerCompPtr->IsModelTeachable(*objectPtr, objectPtr);
		isModelValid = m_teachControllerCompPtr->IsModelDataValid(*objectPtr);
	}

	TeachModelButton->setEnabled(isTeachAllowed);
	ResetModelButton->setEnabled(isModelValid);
}


// reimplemented (iqtgui::CGuiComponentBase)

void CTeachableParamsDecoratorGuiComp::OnGuiCreated()
{
	if (m_slaveGuiCompPtr.IsValid()){
		m_slaveGuiCompPtr->CreateGui(ParamsFrame);
	}

	BaseClass::OnGuiCreated();
}


void CTeachableParamsDecoratorGuiComp::OnGuiDestroyed()
{
	if (m_slaveGuiCompPtr.IsValid()){
		m_slaveGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// protected slots

void CTeachableParamsDecoratorGuiComp::on_TeachModelButton_clicked()
{
	iprm::IParamsSet* objectPtr = GetObservedObject();
	if (m_teachControllerCompPtr.IsValid() && (objectPtr != NULL)){
		m_teachControllerCompPtr->DoModelTeach(*objectPtr, *objectPtr);
	}

	UpdateButtonsState();
}


void CTeachableParamsDecoratorGuiComp::on_ResetModelButton_clicked()
{
	iprm::IParamsSet* objectPtr = GetObservedObject();
	if (m_teachControllerCompPtr.IsValid() && (objectPtr != NULL)){
		m_teachControllerCompPtr->ResetModelData(*objectPtr);
	}

	UpdateButtonsState();
}


} // namespace iqtinsp


