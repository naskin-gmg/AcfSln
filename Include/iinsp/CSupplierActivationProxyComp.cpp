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

	return true;
}


bool CSupplierActivationProxyComp::IsStateFixed() const
{
	return *m_isStateFixedAttrPtr;
}


// reimplemented (iinsp::ISupplier)

int CSupplierActivationProxyComp::GetWorkStatus() const
{
	int retVal = WS_INVALID;

	if (m_supplierEnabledParamCompPtr.IsValid() && m_slaveSupplierCompPtr.IsValid()){
		if (m_supplierEnabledParamCompPtr->IsEnabled()){
			retVal = m_slaveSupplierCompPtr->GetWorkStatus();
		}
	}

	return retVal;
}


void CSupplierActivationProxyComp::InvalidateSupplier()
{
	if (m_supplierEnabledParamCompPtr.IsValid() && m_slaveSupplierCompPtr.IsValid()){
		if (m_supplierEnabledParamCompPtr->IsEnabled()){
			m_slaveSupplierCompPtr->InvalidateSupplier();
		}
	}
}


void CSupplierActivationProxyComp::EnsureWorkInitialized()
{
	if (m_supplierEnabledParamCompPtr.IsValid() && m_slaveSupplierCompPtr.IsValid()){
		if (m_supplierEnabledParamCompPtr->IsEnabled()){
			m_slaveSupplierCompPtr->EnsureWorkInitialized();
		}
	}
}


void CSupplierActivationProxyComp::EnsureWorkFinished()
{
	if (m_supplierEnabledParamCompPtr.IsValid() && m_slaveSupplierCompPtr.IsValid()){
		if (m_supplierEnabledParamCompPtr->IsEnabled()){
			m_slaveSupplierCompPtr->EnsureWorkFinished();
		}
	}
}


void CSupplierActivationProxyComp::ClearWorkResults()
{
	if (m_supplierEnabledParamCompPtr.IsValid() && m_slaveSupplierCompPtr.IsValid()){
		if (m_supplierEnabledParamCompPtr->IsEnabled()){
			m_slaveSupplierCompPtr->ClearWorkResults();
		}
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


