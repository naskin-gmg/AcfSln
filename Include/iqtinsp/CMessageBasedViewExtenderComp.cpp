#include <iqtinsp/CMessageBasedViewExtenderComp.h>


// ACF includes
#include <ilog/CExtMessage.h>
#include <iview/CShapeBase.h>


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

	m_shapeMessages = m_messageContainerCompPtr->GetMessages();
	for (int i = 0; i < m_shapeMessages.count(); i++){
		const ilog::IMessageConsumer::MessagePtr& messagePtr = m_shapeMessages.at(i);

		const ilog::CExtMessage* extMessagePtr = dynamic_cast<const ilog::CExtMessage*>(messagePtr.GetPtr());
		if (extMessagePtr != NULL){
			int objectsCount = extMessagePtr->GetAttachedObjectsCount();
			for (int i = 0; i < objectsCount; ++i){
				const iser::IObject* attachedObjectPtr = extMessagePtr->GetAttachedObject(i);
				iview::IShape* shapePtr = CreateResultShape(m_resultShapeFactoryCompPtr.GetPtr(), attachedObjectPtr, extMessagePtr, true);
				if (shapePtr != NULL){
					iview::CShapeBase* shapeImplPtr = dynamic_cast<iview::CShapeBase*>(shapePtr);
					if (shapeImplPtr != NULL){
						shapeImplPtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);
					}
					viewPtr->ConnectShape(shapePtr);

					m_shapes.PushBack(shapePtr);
				}
			}
		}
		else{
			const i2d::IObject2d* object2dPtr = dynamic_cast<const i2d::IObject2d*>(messagePtr.GetPtr());
			if (object2dPtr != NULL){
				iview::IShape* shapePtr = CreateResultShape(m_resultShapeFactoryCompPtr.GetPtr(), object2dPtr, messagePtr.GetPtr(), true);
				if (shapePtr != NULL){
					iview::CShapeBase* shapeImplPtr = dynamic_cast<iview::CShapeBase*>(shapePtr);
					if (shapeImplPtr != NULL){
						shapeImplPtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);
					}
					viewPtr->ConnectShape(shapePtr);

					m_shapes.PushBack(shapePtr);
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

	for (int i = 0; i < m_shapes.GetCount(); i++){
		iview::IShape* shapePtr = m_shapes.GetAt(i);

		viewPtr->DisconnectShape(shapePtr);

		imod::CSingleModelObserverBase* observerPtr = dynamic_cast<imod::CSingleModelObserverBase*>(shapePtr);
		if (observerPtr != NULL){
			observerPtr->EnsureModelDetached();
		}
		else{
			I_CRITICAL();
		}
	}

	m_shapes.Reset();
	m_shapeMessages.clear();
}


// reimplemented (icomp::CComponentBase)

void CMessageBasedViewExtenderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// component initialization
	m_resultShapeFactoryCompPtr.IsValid();
	m_messageContainerCompPtr.IsValid();
	m_supplierMessagesCompPtr.IsValid();
	m_slaveViewExtenderCompPtr.IsValid();
}


void CMessageBasedViewExtenderComp::OnComponentDestroyed()
{
	m_shapes.Reset();
	m_shapeMessages.clear();

	BaseClass::OnComponentDestroyed();
}


} // namespace iqtinsp
