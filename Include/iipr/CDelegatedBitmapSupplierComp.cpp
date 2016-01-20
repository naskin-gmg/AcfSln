#include "iipr/CDelegatedBitmapSupplierComp.h"


namespace iipr
{


CDelegatedBitmapSupplierComp::CDelegatedBitmapSupplierComp()
:	BaseClass2(this)
{
}


// reimplemented (iimg::IBitmapProvider)

const iimg::IBitmap* CDelegatedBitmapSupplierComp::GetBitmap() const
{
	if (m_bitmapProviderCompPtr.IsValid()){
		return m_bitmapProviderCompPtr->GetBitmap();
	}

	if (m_bitmapCompPtr.IsValid()){
		return m_bitmapCompPtr.GetPtr();
	}

	return NULL;
}

	
// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CDelegatedBitmapSupplierComp::GetCalibration() const
{
	if (m_calibrationProviderCompPtr.IsValid()){
		return m_calibrationProviderCompPtr->GetCalibration();
	}

	if (m_calibrationCompPtr.IsValid()){
		return m_calibrationCompPtr.GetPtr();
	}

	return NULL;
}


// reimplemented (iinsp::ISupplier)

int CDelegatedBitmapSupplierComp::GetWorkStatus() const
{
	if (m_bitmapSupplierCompPtr.IsValid()){
		return m_bitmapSupplierCompPtr->GetWorkStatus();
	}

	if (m_bitmapCompPtr.IsValid()){
		return WS_OK;
	}

	return WS_CRITICAL;
}


void CDelegatedBitmapSupplierComp::InvalidateSupplier()
{
	if (m_bitmapSupplierCompPtr.IsValid()){
		m_bitmapSupplierCompPtr->InvalidateSupplier();
	}
}


void CDelegatedBitmapSupplierComp::EnsureWorkInitialized()
{
	if (m_bitmapSupplierCompPtr.IsValid()){
		m_bitmapSupplierCompPtr->EnsureWorkInitialized();
	}
}


void CDelegatedBitmapSupplierComp::EnsureWorkFinished()
{
	if (m_bitmapSupplierCompPtr.IsValid()){
		m_bitmapSupplierCompPtr->EnsureWorkFinished();
	}
}


void CDelegatedBitmapSupplierComp::ClearWorkResults()
{
	if (m_bitmapSupplierCompPtr.IsValid()){
		m_bitmapSupplierCompPtr->ClearWorkResults();
	}
}


const ilog::IMessageContainer* CDelegatedBitmapSupplierComp::GetWorkMessages(int containerType) const
{
	if (m_bitmapSupplierCompPtr.IsValid()){
		return m_bitmapSupplierCompPtr->GetWorkMessages(containerType);
	}

	return NULL;
}


iprm::IParamsSet* CDelegatedBitmapSupplierComp::GetModelParametersSet() const
{
	if (m_bitmapSupplierCompPtr.IsValid()){
		return m_bitmapSupplierCompPtr->GetModelParametersSet();
	}

	return NULL;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CDelegatedBitmapSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderCompPtr.IsValid()){
		if (m_bitmapProviderModelCompPtr.IsValid()){
			m_bitmapProviderModelCompPtr->AttachObserver(this);
		}
	}
	else{
		if (m_bitmapModelCompPtr.IsValid()){
			m_bitmapModelCompPtr->AttachObserver(this);
		}

		if (m_calibrationModelCompPtr.IsValid()){
			m_calibrationModelCompPtr->AttachObserver(this);
		}
	}
}


void CDelegatedBitmapSupplierComp::OnComponentDestroyed()
{
	BaseClass2::EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace iipr


