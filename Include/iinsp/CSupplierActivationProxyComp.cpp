#include <iinsp/CSupplierActivationProxyComp.h>


// ACF includes
#include <iprm/IParamsSet.h>
#include <ilog/IMessageContainer.h>


namespace iinsp
{


// public methods

CSupplierActivationProxyComp::CSupplierActivationProxyComp()
	:m_updateBridge(this)
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
	return *m_isStateFixedAttrPtr;
}


// reimplemented (iinsp::ISupplier)

int CSupplierActivationProxyComp::GetWorkStatus() const
{
	int retVal = IsSupplierEnabled() ? WS_OK : WS_INVALID;

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
	if (m_slaveSupplierCompPtr.IsValid()){
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
}


void CSupplierActivationProxyComp::OnComponentDestroyed()
{
	m_updateBridge.EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace iinsp


