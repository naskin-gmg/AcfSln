#include <iqtinsp/CMessageBasedViewExtenderComp.h>


// ACF includes
#include <ilog/CExtMessage.h>
#include <iview/CShapeBase.h>
#include <iprm/IOptionsList.h>


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

	ilog::IMessageContainer::Messages& messages = m_shapeMessages[providerPtr];

	for (int i = 0; i < messages.count(); i++){
		const ilog::IMessageConsumer::MessagePtr& messagePtr = messages.at(i);

		if (!IsMessageAccepted(*messagePtr.GetPtr())){
			continue;
		}

		const ilog::CExtMessage* extMessagePtr = dynamic_cast<const ilog::CExtMessage*>(messagePtr.GetPtr());
		if (extMessagePtr != NULL){
			int objectsCount = extMessagePtr->GetAttachedObjectsCount();
			for (int i = 0; i < objectsCount; ++i){
				const iser::IObject* attachedObjectPtr = extMessagePtr->GetAttachedObject(i);
				ShapePtr shapePtr(CreateResultShape(m_resultShapeFactoryCompPtr.GetPtr(), attachedObjectPtr, extMessagePtr, true));
				if (shapePtr.IsValid()){
					iview::CShapeBase* shapeImplPtr = dynamic_cast<iview::CShapeBase*>(shapePtr.GetPtr());
					if (shapeImplPtr != NULL){
						shapeImplPtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);
					}
					viewPtr->ConnectShape(shapePtr.GetPtr());

					m_shapes[providerPtr].push_back(shapePtr);
				}
			}
		}
		else{
			const i2d::IObject2d* object2dPtr = dynamic_cast<const i2d::IObject2d*>(messagePtr.GetPtr());
			if (object2dPtr != NULL){
				ShapePtr  shapePtr(CreateResultShape(m_resultShapeFactoryCompPtr.GetPtr(), object2dPtr, messagePtr.GetPtr(), true));
				if (shapePtr.IsValid()){
					iview::CShapeBase* shapeImplPtr = dynamic_cast<iview::CShapeBase*>(shapePtr.GetPtr());
					if (shapeImplPtr != NULL){
						shapeImplPtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);
					}
					viewPtr->ConnectShape(shapePtr.GetPtr());

					m_shapes[providerPtr].push_back(shapePtr);
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
		iview::IShape* shapePtr = shapes[i].GetPtr();

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
}


void CMessageBasedViewExtenderComp::OnComponentDestroyed()
{
	m_shapes.clear();
	m_shapeMessages.clear();

	BaseClass::OnComponentDestroyed();
}


// protected methods

bool CMessageBasedViewExtenderComp::IsMessageAccepted(const istd::IInformationProvider& message) const
{
	QByteArray messageId = message.GetInformationSource().toUtf8(); // Extended istd::IInformationProvider with GetInformationSourceId method!

	if (!messageId.isEmpty() && m_shapeSelectorCompPtr.IsValid()){
		const iprm::IOptionsList* optionsListPtr = m_shapeSelectorCompPtr->GetSelectionConstraints();
		if (optionsListPtr != NULL){
			int selectedIndex = m_shapeSelectorCompPtr->GetSelectedOptionIndex();
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


} // namespace iqtinsp


