#ifndef iinsp_CSupplierActivationProxyComp_included
#define iinsp_CSupplierActivationProxyComp_included


// ACF includes
#include <imod/CModelUpdateBridge.h>
#include <icomp/CComponentBase.h>
#include <iprm/IEnableableParam.h>

// ACF-Solutions includes
#include <iinsp/IEnableableSupplier.h>


namespace iinsp
{


/**
	Supplier activation proxy.
	This component can be used to add enable/disable logic to any suppler.
*/
class CSupplierActivationProxyComp:
			public icomp::CComponentBase,
			virtual public iinsp::IEnableableSupplier
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSupplierActivationProxyComp);
		I_REGISTER_INTERFACE(iinsp::ISupplier);
		I_REGISTER_INTERFACE(iinsp::IEnableableSupplier);
		I_ASSIGN(m_slaveSupplierCompPtr, "SlaveSupplier", "Source supplier to be activated/deactivated", true, "SlaveSupplier");
		I_ASSIGN(m_supplierEnabledParamCompPtr, "SupplierActivator", "Enabling parameter used for supplier activation", true, "SupplierActivator");
		I_ASSIGN_TO(m_supplierEnabledParamModelCompPtr, m_supplierEnabledParamCompPtr, true);
		I_ASSIGN(m_isStateFixedAttrPtr, "StateFixed", "If enabled, the activation state of suppler cannot be modified from outside", true, false);
	I_END_COMPONENT;

	CSupplierActivationProxyComp();

	// reimplemented (iinsp::IEnableableSupplier)
	virtual bool SetSupplierEnabled(bool isEnabled = true);
	virtual bool IsSupplierEnabled() const;
	virtual bool IsStateFixed() const;

	// reimplemented (iinsp::ISupplier)
	virtual int GetWorkStatus() const;
	virtual void InvalidateSupplier();
	virtual void EnsureWorkInitialized();
	virtual void EnsureWorkFinished();
	virtual void ClearWorkResults();
	virtual const ilog::IMessageContainer* GetWorkMessages(int messageType) const;
	virtual iprm::IParamsSet* GetModelParametersSet() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iinsp::ISupplier, m_slaveSupplierCompPtr);
	I_REF(iprm::IEnableableParam, m_supplierEnabledParamCompPtr);
	I_REF(imod::IModel, m_supplierEnabledParamModelCompPtr);
	I_ATTR(bool, m_isStateFixedAttrPtr);

	imod::CModelUpdateBridge m_updateBridge;
};


} // namespace iinsp


#endif // !iinsp_CSupplierActivationProxyComp_included


