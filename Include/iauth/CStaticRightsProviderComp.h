#ifndef iauth_CStaticRightsProviderComp_included
#define iauth_CStaticRightsProviderComp_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QSet>

// ACF includes
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "iauth/IRightsProvider.h"


namespace iauth
{


class CStaticRightsProviderComp:
			public icomp::CComponentBase,
			public IRightsProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CStaticRightsProviderComp);
		I_REGISTER_INTERFACE(IRightsProvider);
		I_ASSIGN(m_slaveProviderIfPtr, "SlaveProvider", "Slave rights provider", false, "SlaveProvider");
		I_ASSIGN(m_defaultRightAttrPtr, "DefaultRight", "Default rights enabled", false, false);
		I_ASSIGN_MULTI_0(m_rightsOnAttrPtr, "RightsOn", "List of enabled rights", false);
		I_ASSIGN_MULTI_0(m_rightsOffAttrPtr, "RightsOff", "List of disabled rights", false);
	I_END_COMPONENT;

	// reimeplemented (IRightsProvider)
	virtual bool HasRight(
					const QByteArray& operationId,
					bool beQuiet = false) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(IRightsProvider, m_slaveProviderIfPtr);
	I_ATTR(bool, m_defaultRightAttrPtr);
	I_MULTIATTR(QString, m_rightsOnAttrPtr);
	I_MULTIATTR(QString, m_rightsOffAttrPtr);

	typedef QSet<QByteArray> RightsList;

	RightsList m_rightsOn;
	RightsList m_rightsOff;
};



} // namespace iauth



#endif // !iauth_CStaticRightsProviderComp_included



