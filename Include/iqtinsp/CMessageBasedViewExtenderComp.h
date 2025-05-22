#pragma once


// ACF includes
#include <istd/TPointerVector.h>
#include <icomp/CComponentBase.h>
#include <imod/CMultiModelDispatcherBase.h>
#include <iprm/ISelectionParam.h>
#include <ilog/IMessageContainer.h>
#include <iview/IShapeView.h>
#include <iview/IShapeFactory.h>
#include <iqt2d/IViewExtender.h>
#include <iqt2d/IViewProvider.h>
#include <iinsp/ISupplier.h>
#include <iqtinsp/TResultShapeCreatorWrap.h>


namespace iqtinsp
{


/**
	Basic view extender showing geometric messages as inactive shapes.
	\c ResultShapeFactory is used for shape creation (i.e. must be set).
	\c MessageContainer is used as message source (e.g the task log of the supplier will be used)
*/
class CMessageBasedViewExtenderComp:
			public iqtinsp::TResultShapeCreatorWrap<icomp::CComponentBase>,
			virtual public iqt2d::IViewExtender,
			private imod::CMultiModelDispatcherBase
{
public:
	typedef iqtinsp::TResultShapeCreatorWrap<icomp::CComponentBase> BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	I_BEGIN_COMPONENT(CMessageBasedViewExtenderComp);
		I_REGISTER_INTERFACE(iqt2d::IViewExtender);
		I_ASSIGN(m_resultShapeFactoryCompPtr, "ResultShapeFactory", "Creates shapes to display", true, "ResultShapeFactory");
		I_ASSIGN(m_messageContainerCompPtr, "MessageContainer", "Shape messages provider", true, "MessageContainer");
		I_ASSIGN(m_slaveViewExtenderCompPtr, "SlaveViewExtender", "Slave view extender", false, "ViewExtender");
		I_ASSIGN(m_messageFilterCompPtr, "MessageFilter", "Optional filter for the messages used for shape genertion. Only messages with source-ID corresponding to the given selection will be processed", false, "MessageFilter");
		I_ASSIGN(m_messageSelectorCompPtr, "MessageSelector", "", false, "MessageSelector");
		I_ASSIGN_TO(m_messageSelectorModelCompPtr, m_messageSelectorCompPtr, false);
	I_END_COMPONENT;

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	virtual bool IsMessageAccepted(const istd::IInformationProvider& message) const;

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

protected:
	I_REF(iview::IShapeFactory, m_resultShapeFactoryCompPtr);
	I_REF(ilog::IMessageContainer, m_messageContainerCompPtr);
	I_REF(iqt2d::IViewExtender, m_slaveViewExtenderCompPtr);
	I_REF(iprm::ISelectionParam, m_messageFilterCompPtr);
	I_REF(iprm::ISelectionParam, m_messageSelectorCompPtr);
	I_REF(imod::IModel, m_messageSelectorModelCompPtr);

	typedef istd::TSharedInterfacePtr<iview::IShape> ShapePtr;

	struct Shape
	{
		ShapePtr shapePtr;
		int messageIndex;
	};

	typedef QList<Shape> ShapeList;
	typedef QMap<iqt2d::IViewProvider*, ShapeList> ShapesMap;
	typedef QMap<iqt2d::IViewProvider*, ilog::IMessageContainer::Messages> MessagesMap;

	ShapesMap m_shapes;
	MessagesMap m_shapeMessages;
};


} // namespace iqtinsp


