#include <icam/CCameraSnapSupplierCompBase.h>


namespace icam
{


// protected methods


// reimplemented (CSnapBitmapSupplierCompBase)

int CCameraSnapSupplierCompBase::DoSnap(const iprm::IParamsSet* snapParamsPtr, iimg::IBitmap& snapBitmap) const
{
	if (!m_bitmapAcquisitionCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component architecture, 'BitmapAcquisition' component reference is not set");

		return iproc::IProcessor::TS_INVALID;
	}


	return m_bitmapAcquisitionCompPtr->DoProcessing(snapParamsPtr, NULL, &snapBitmap);
}


// reimplemented (iinsp::TSupplierCompWrap)

bool CCameraSnapSupplierCompBase::InitializeWork()
{
	if (m_bitmapAcquisitionCompPtr.IsValid()){
		m_bitmapAcquisitionCompPtr->InitProcessor(GetModelParametersSet());

		return true;
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CCameraSnapSupplierCompBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// initialize components
	m_bitmapAcquisitionCompPtr.EnsureInitialized();
}


} // namespace icam


