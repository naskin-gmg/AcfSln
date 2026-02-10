// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icmpstr_IRegistryConsistInfo_included
#define icmpstr_IRegistryConsistInfo_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtGui/QIcon>

// ACF includes
#include <istd/IPolymorphic.h>
#include <icomp/IRegistry.h>
#include <ilog/IMessageConsumer.h>

// ACF-Solutions includes
#include <icmpstr/icmpstr.h>


namespace icmpstr
{


class IRegistryConsistInfo: virtual public istd::IPolymorphic
{
public:
	enum QueryFlags
	{
		QF_NONE = 0,
		/**
			Supporting of any specified interfaces is enough to interprete component as compatible one.
		*/
		QF_ANY_INTERFACE = 1 << 0,
		/**
			If active, all undefined (without available meta information) elements will be also included.
		*/
		QF_INCLUDE_UNDEFINED = 1 << 1,
		/**
			If active, subelements will be also considered.
		*/
		QF_INCLUDE_SUBELEMENTS = 1 << 2,
		/**
			If active, subelements will be also considered.
		*/
		QF_DETACHED_FROM_CONTAINER = 1 << 3
	};

	/**
		Get list of compatible element names.
		\param	interfaceNames		list of interfaces must be implemented.
		\param	registry			registry will be looked in.
	*/
	virtual icomp::IRegistry::Ids GetCompatibleElements(
				const icomp::IElementStaticInfo::Ids& interfaceNames,
				const icomp::IRegistry& registry,
				int queryFlags = QF_NONE) const = 0;

	/**
		Check if registry is valid.
	 
		A registry is invalid when it contains unresolved component references.
 
		\param	registry			registry object.
		\param	ignoreUndef			if true, undefined elements will be ignored.
		\param	allReasons			if true, all reasons will be explained.
		\param	reasonConsumerPtr	optional consumer of messages explaining reason of calculated result.
	*/
	virtual bool IsRegistryValid(
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ilog::IMessageConsumer* reasonConsumerPtr) const = 0;

	/**
		Check if element is valid.
		\param	attributeName		name of registry element in specified registry.
		\param	registry			registry object.
		\param	ignoreUndef			if true, undefined elements will be ignored.
		\param	allReasons			if true, all reasons will be explained.
		\param	reasonConsumerPtr	optional consumer of messages explaining reason of calculated result.
	*/
	virtual bool IsElementValid(
				const QByteArray& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ilog::IMessageConsumer* reasonConsumerPtr) const = 0;

	/**
		Check if attribute is valid.
		\param	attributeName			name of attribute in specified registry element.
		\param	elementName				name of registry element in specified registry.
		\param	registry				registry object.
		\param	ignoreUndef				if true, undefined elements will be ignored.
		\param	allReasons				if true, all reasons will be explained.
		\param	reasonConsumerPtr		optional consumer of messages explaining reason of calculated result.
		\param	componentMetaInfoPtr	optional component meta info (e.g used for providing meta info of embedded components).
	*/
	virtual bool IsAttributeValid(
				const QByteArray& attributeName,
				const QByteArray& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ilog::IMessageConsumer* reasonConsumerPtr,
				const icomp::IComponentStaticInfo* componentMetaInfoPtr = NULL) const = 0;

	/**
		Check if attribute is valid.
		\param	refererenceGoal			text of reference value.
		\param	isFactory				true if the relationship type is factory.
		\param	registry				registry object.
		\param	ignoreUndef				if true, undefined elements will be ignored.
		\param	allReasons				if true, all reasons will be explained.
		\param	reasonConsumerPtr		optional consumer of messages explaining reason of calculated result.
		\param	componentMetaInfoPtr	optional component meta info (e.g used for providing meta info of embedded components).
	*/
	virtual bool IsAttributeValueValid(
				int attributeFlags,
				const QByteArray& attributeText,
				const icomp::IElementStaticInfo::Ids& interfaceNames,
				const icomp::IElementStaticInfo::Ids& optionalInterfaceNames,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				ilog::IMessageConsumer* reasonConsumerPtr) const = 0;

	/**
		Get icon of component.
	*/
	virtual QIcon GetComponentIcon(const icomp::CComponentAddress& address) const = 0;
};


} // namespace icmpstr


#endif // !icmpstr_IRegistryConsistInfo_included


