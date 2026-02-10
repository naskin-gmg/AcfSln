// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtinsp/CMessageBasedViewExtenderComp.h>


// ACF includes
#include <ilog/CExtMessage.h>
#include <iview/CShapeBase.h>
#include <iprm/IOptionsList.h>
#include <iview/IInteractiveShape.h>


namespace iqtinsp
{


// reimplemented (iqt2d::IViewExtender)

void CMessageBasedViewExtenderComp::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	if (m_slaveViewExtenderCompPtr.IsValid()){
		m_slaveViewExtenderCompPtr->AddItemsToScene(providerPtr, flags);
	}

	Q_ASSERT(providerPtr != NULL);
	iview::IShapeView* viewPtr = providerPtr->GetView();
	Q_ASSERT(viewPtr != NULL);

	if (!m_messageContainerCompPtr.IsValid() || !m_resultShapeFactoryCompPtr.IsValid()){
		return;
	}

	m_shapeMessages[providerPtr] = m_messageContainerCompPtr->GetMessages();

	const ilog::IMessageContainer::Messages& messages = m_shapeMessages[providerPtr];

	int messageIndex = int(messages.size()) - 1;
	for (ilog::IMessageContainer::Messages::const_iterator it = messages.begin(); it != messages.end(); it++) {
		const ilog::IMessageConsumer::MessagePtr messagePtr = *it;

		if (!IsMessageAccepted(*messagePtr)){
			messageIndex--;
			continue;
		}

		Shape shapeInfo;
		shapeInfo.messageIndex = messageIndex;
		messageIndex--;

		const ilog::CExtMessage* extMessagePtr = dynamic_cast<const ilog::CExtMessage*>(messagePtr.GetPtr());
		if (extMessagePtr != NULL){
			int objectsCount = extMessagePtr->GetAttachedObjectsCount();
			for (int objectIndex = 0; objectIndex < objectsCount; ++objectIndex){
				const iser::IObject* attachedObjectPtr = extMessagePtr->GetAttachedObject(objectIndex);
				ShapePtr shapePtr(CreateResultShape(m_resultShapeFactoryCompPtr.GetPtr(), attachedObjectPtr, extMessagePtr, true));
				if (shapePtr.IsValid()){
					iview::CShapeBase* shapeImplPtr = dynamic_cast<iview::CShapeBase*>(shapePtr.GetPtr());
					if (shapeImplPtr != NULL){
						shapeImplPtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);
					}
					viewPtr->ConnectShape(shapePtr.GetPtr());

					shapeInfo.shapePtr = shapePtr;

					const QString& objectDescription = extMessagePtr->GetAttachedObjectDescription(objectIndex);
					if (!objectDescription.isEmpty()) {
						shapePtr->SetToolTip(objectDescription);
					}

					m_shapes[providerPtr].push_back(shapeInfo);
				}
			}
		}
		else{
			const i2d::IObject2d* object2dPtr = dynamic_cast<const i2d::IObject2d*>(messagePtr.GetPtr());
			if (object2dPtr != NULL){
				ShapePtr shapePtr(CreateResultShape(m_resultShapeFactoryCompPtr.GetPtr(), object2dPtr, messagePtr.GetPtr(), true));
				if (shapePtr.IsValid()){
					iview::CShapeBase* shapeImplPtr = dynamic_cast<iview::CShapeBase*>(shapePtr.GetPtr());
					if (shapeImplPtr != NULL){
						shapeImplPtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);
					}
					viewPtr->ConnectShape(shapePtr.GetPtr());

					shapeInfo.shapePtr = shapePtr;

					m_shapes[providerPtr].push_back(shapeInfo);
				}
			}
		}
	}
}


void CMessageBasedViewExtenderComp::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	if (m_slaveViewExtenderCompPtr.IsValid()){
		m_slaveViewExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}

	iview::IShapeView* viewPtr = providerPtr->GetView();
	Q_ASSERT(viewPtr != NULL);

	ShapeList& shapes = m_shapes[providerPtr];

	for (int i = 0; i < shapes.count(); i++){
		iview::IShape* shapePtr = shapes[i].shapePtr.GetPtr();

		viewPtr->DisconnectShape(shapePtr);

		imod::CSingleModelObserverBase* observerPtr = dynamic_cast<imod::CSingleModelObserverBase*>(shapePtr);
		if (observerPtr != NULL){
			observerPtr->EnsureModelDetached();
		}
		else{
			I_CRITICAL();
		}
	}

	m_shapes.remove(providerPtr);

	m_shapeMessages.remove(providerPtr);
}


// reimplemented (icomp::CComponentBase)

void CMessageBasedViewExtenderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// component initialization
	m_resultShapeFactoryCompPtr.IsValid();
	m_messageContainerCompPtr.IsValid();
	m_slaveViewExtenderCompPtr.IsValid();

	if (m_messageSelectorCompPtr.IsValid() && m_messageSelectorModelCompPtr.IsValid()){
		RegisterModel(m_messageSelectorModelCompPtr.GetPtr());
	}
}


void CMessageBasedViewExtenderComp::OnComponentDestroyed()
{
	BaseClass2::UnregisterAllModels();

	m_shapes.clear();
	m_shapeMessages.clear();

	BaseClass::OnComponentDestroyed();
}


// protected methods

bool CMessageBasedViewExtenderComp::IsMessageAccepted(const istd::IInformationProvider& message) const
{
	QByteArray messageId = message.GetInformationSource().toUtf8(); // Extended istd::IInformationProvider with GetInformationSourceId method!

	if (!messageId.isEmpty() && m_messageFilterCompPtr.IsValid()){
		const iprm::IOptionsList* optionsListPtr = m_messageFilterCompPtr->GetSelectionConstraints();
		if (optionsListPtr != NULL){
			int selectedIndex = m_messageFilterCompPtr->GetSelectedOptionIndex();
			if (selectedIndex >= 0){
				QByteArray optionId = optionsListPtr->GetOptionId(selectedIndex);
				if (optionId != messageId){
					return false;
				}
			}
		}
	}

	return true;
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CMessageBasedViewExtenderComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	int selectedIndex = m_messageSelectorCompPtr->GetSelectedOptionIndex();

	for (ShapesMap::ConstIterator iter = m_shapes.constBegin(); iter != m_shapes.constEnd(); ++iter){
		const ShapeList& shapeList = *iter;

		for (ShapeList::ConstIterator shapeIter = shapeList.constBegin(); shapeIter != shapeList.constEnd(); ++shapeIter){
			const Shape& shape = *shapeIter;

			ShapePtr shapePtr = shape.shapePtr;
			iview::IInteractiveShape* interactiveShapePtr = dynamic_cast<iview::IInteractiveShape*>(shapePtr.GetPtr());
			if (interactiveShapePtr != NULL){
				interactiveShapePtr->SetSelected(shape.messageIndex == selectedIndex);
			}
		}
	}
}


} // namespace iqtinsp


