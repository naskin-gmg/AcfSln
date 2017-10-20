#pragma once


// ACF includes
#include <istd/TPointerVector.h>
#include <icomp/CComponentBase.h>
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
			virtual public iqt2d::IViewExtender
{
public:
	typedef iqtinsp::TResultShapeCreatorWrap<icomp::CComponentBase> BaseClass;

	I_BEGIN_COMPONENT(CMessageBasedViewExtenderComp);
		I_REGISTER_INTERFACE(iqt2d::IViewExtender);
		I_ASSIGN(m_resultShapeFactoryCompPtr, "ResultShapeFactory", "Creates shapes to display", true, "ResultShapeFactory");
		I_ASSIGN(m_messageContainerCompPtr, "MessageContainer", "Shape messages provider", true, "MessageContainer");
		I_ASSIGN(m_slaveViewExtenderCompPtr, "SlaveViewExtender", "Slave view extender", false, "ViewExtender");
	I_END_COMPONENT;

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iview::IShapeFactory, m_resultShapeFactoryCompPtr);
	I_REF(ilog::IMessageContainer, m_messageContainerCompPtr);
	I_REF(iqt2d::IViewExtender, m_slaveViewExtenderCompPtr);

	istd::TPointerVector<iview::IShape> m_shapes;
	ilog::IMessageContainer::Messages m_shapeMessages;
};


} // namespace iqtinsp

