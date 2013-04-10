#ifndef iauth_CRightsBasedEnablerComp_included
#define iauth_CRightsBasedEnablerComp_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include "icomp/CComponentBase.h"
#include "imod/TSingleModelObserverBase.h"
#include "iprm/IEnableableParam.h"

// ACF-Solutions includes
#include "iauth/IRightsProvider.h"


namespace iauth
{


/**
	State component implementation based on underlaying rights provider.
*/
class CRightsBasedEnablerComp:
			public icomp::CComponentBase,
			virtual public iprm::IEnableableParam,
			protected imod::TSingleModelObserverBase<iauth::IRightsProvider>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<iauth::IRightsProvider> BaseClass2;

	I_BEGIN_COMPONENT(CRightsBasedEnablerComp);
		I_REGISTER_INTERFACE(IRightsProvider);
		I_ASSIGN(m_rightsProviderCompPtr, "RightsProvider", "Slave rights provider", true, "RightsProvider");
		I_ASSIGN_TO(m_rightsProviderModelCompPtr, m_rightsProviderCompPtr, true);
		I_ASSIGN_MULTI_0(m_rightIdsAttrPtr, "RightIds", "List of right IDs to be checked for calculation of the entire enabling state", true);
	I_END_COMPONENT;

	CRightsBasedEnablerComp();

	// reimplemented (iprm::IEnableableParam)
	virtual bool IsEnabled() const;
	virtual bool IsEnablingAllowed() const;
	virtual bool SetEnabled(bool isEnabled = true);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	/**
		Underlaying rights provider.
	*/
	I_REF(IRightsProvider, m_rightsProviderCompPtr);
	I_REF(imod::IModel, m_rightsProviderModelCompPtr);

	/**
		List if the right IDs to be checked for calculation of the final enabling state.
	*/
	I_MULTIATTR(QByteArray, m_rightIdsAttrPtr);

	/**
		Current enabling state.
	*/
	bool m_isEnabled;
};


} // namespace iauth


#endif // !iauth_CRightsBasedEnablerComp_included


