#include <iinsp/CSupplierActivationProxyComp.h>


// ACF includes
#include <iprm/IParamsSet.h>
#include <ilog/IMessageContainer.h>


namespace iinsp
{


// public methods

CSupplierActivationProxyComp::CSupplierActivationProxyComp()
	:m_updateBridge(this, imod::CModelUpdateBridge::UF_SOURCE)
{
}


// reimplemented (iinsp::IEnableableSupplier)

bool CSupplierActivationProxyComp::SetSupplierEnabled(bool isEnabled)
{
	if (!*m_isStateFixedAttrPtr && m_supplierEnabledParamCompPtr.IsValid()){
		return m_supplierEnabledParamCompPtr->SetEnabled(isEnabled);
	}

	return false;
}


bool CSupplierActivationProxyComp::IsSupplierEnabled() const
{
	if (m_supplierEnabledParamCompPtr.IsValid()){
		return m_supplierEnabledParamCompPtr->IsEnabled();
	}

	return false;
}


bool CSupplierActivationProxyComp::IsStateFixed() const
{
	bool isStateFixed = *m_isStateFixedAttrPtr;

	if (m_supplierEnabledParamCompPtr.IsValid()){
		isStateFixed = isStateFixed || !m_supplierEnabledParamCompPtr->IsEnablingAllowed();
	}

	return isStateFixed;
}


// reimplemented (iinsp::ISupplier)

int CSupplierActivationProxyComp::GetWorkStatus() const
{
	int retVal = WS_OK;

	if (m_slaveSupplierCompPtr.IsValid() && IsSupplierEnabled()){
		retVal = m_slaveSupplierCompPtr->GetWorkStatus();
	}

	return retVal;
}


void CSupplierActivationProxyComp::InvalidateSupplier()
{
	if (m_slaveSupplierCompPtr.IsValid() && IsSupplierEnabled()){
		m_slaveSupplierCompPtr->InvalidateSupplier();
	}
}


void CSupplierActivationProxyComp::EnsureWorkInitialized()
{
	if (m_slaveSupplierCompPtr.IsValid() && IsSupplierEnabled()){
		m_slaveSupplierCompPtr->EnsureWorkInitialized();
	}
}


void CSupplierActivationProxyComp::EnsureWorkFinished()
{
	if (m_slaveSupplierCompPtr.IsValid() && IsSupplierEnabled()){
		m_slaveSupplierCompPtr->EnsureWorkFinished();
	}
}


void CSupplierActivationProxyComp::ClearWorkResults()
{
	if (m_slaveSupplierCompPtr.IsValid() && IsSupplierEnabled()){
		m_slaveSupplierCompPtr->ClearWorkResults();
	}
}


const ilog::IMessageContainer* CSupplierActivationProxyComp::GetWorkMessages(int containerType) const
{
	if (m_slaveSupplierCompPtr.IsValid() && IsSupplierEnabled()){
		return m_slaveSupplierCompPtr->GetWorkMessages(containerType);
	}

	return NULL;
}


iprm::IParamsSet* CSupplierActivationProxyComp::GetModelParametersSet() const
{
	if (m_slaveSupplierCompPtr.IsValid()){
		return m_slaveSupplierCompPtr->GetModelParametersSet();
	}

	return NULL;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CSupplierActivationProxyComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_supplierEnabledParamModelCompPtr.IsValid()){
		m_supplierEnabledParamModelCompPtr->AttachObserver(&m_updateBridge);
	}

	if (m_slaveSupplierModelCompPtr.IsValid()){
		m_slaveSupplierModelCompPtr->AttachObserver(&m_updateBridge);
	}
}


void CSupplierActivationProxyComp::OnComponentDestroyed()
{
	m_updateBridge.EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace iinsp


