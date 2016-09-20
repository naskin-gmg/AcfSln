#include <iauth/CStaticRightsProviderComp.h>


namespace iauth
{


// reimplemented (iauth::IRightsProvider)

bool CStaticRightsProviderComp::HasRight(
			const QByteArray& operationId,
			bool beQuiet) const
{
	if (m_rightsOn.find(operationId) != m_rightsOn.end()){
		return true;
	}

	if (m_rightsOff.find(operationId) != m_rightsOff.end()){
		return false;
	}

	if (m_slaveProviderIfPtr.IsValid()){
		return m_slaveProviderIfPtr->HasRight(operationId, beQuiet);
	}

	return m_defaultRightAttrPtr.IsValid() && *m_defaultRightAttrPtr;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CStaticRightsProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_rightsOn.clear();
	m_rightsOff.clear();

	if (m_rightsOnAttrPtr.IsValid()){
		int attrCount = int(m_rightsOnAttrPtr.GetCount());
		for (int i = 0; i < attrCount; ++i){
			m_rightsOn.insert(m_rightsOnAttrPtr[i].toLocal8Bit());
		}
	}

	if (m_rightsOffAttrPtr.IsValid()){
		int attrCount = int(m_rightsOffAttrPtr.GetCount());
		for (int i = 0; i < attrCount; ++i){
			m_rightsOff.insert(m_rightsOffAttrPtr[i].toLocal8Bit());
		}
	}
}


} // namespace iauth


