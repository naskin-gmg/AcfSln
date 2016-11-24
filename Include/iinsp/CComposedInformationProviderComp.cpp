#include <iinsp/CComposedInformationProviderComp.h>


namespace iinsp
{


// public methods

CComposedInformationProviderComp::CComposedInformationProviderComp()
:	m_updateBridge(this)
{
}


// reimplemented (iinsp::IInformationProvider)

QDateTime CComposedInformationProviderComp::GetInformationTimeStamp() const
{
	QDateTime retVal;

	for (int index = 0; index < m_slaveInformationProvidersCompPtr.GetCount(); ++index){
		const istd::IInformationProvider* infoProviderPtr = m_slaveInformationProvidersCompPtr[index];
		if (infoProviderPtr != NULL){
			QDateTime timeStamp = infoProviderPtr->GetInformationTimeStamp();

			if (!retVal.isValid()){
				retVal = timeStamp; 
			}
			else if(retVal < timeStamp){
				retVal = timeStamp;
			}
		}
	}

	return retVal;
}


istd::IInformationProvider::InformationCategory CComposedInformationProviderComp::GetInformationCategory() const
{
	InformationCategory retVal = IC_NONE;

	for (int index = 0; index < m_slaveInformationProvidersCompPtr.GetCount(); ++index){
		const istd::IInformationProvider* infoProviderPtr = m_slaveInformationProvidersCompPtr[index];
		if (infoProviderPtr != NULL){
			InformationCategory informationCategory = infoProviderPtr->GetInformationCategory();
			if (informationCategory > retVal){
				retVal = informationCategory;
			}
		}
	}

	return retVal;
}


int CComposedInformationProviderComp::GetInformationId() const
{
	return -1;
}


QString CComposedInformationProviderComp::GetInformationDescription() const
{
	return *m_defaultDescriptionAttrPtr;
}


QString CComposedInformationProviderComp::GetInformationSource() const
{
	return *m_defaultSourceAttrPtr;
}


int CComposedInformationProviderComp::GetInformationFlags() const
{
	return 0;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CComposedInformationProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int providerModelsCount = m_slaveInformationProviderModelsCompPtr.GetCount();

	for (int index = 0; index < providerModelsCount; ++index){
		imod::IModel* infoProviderModelPtr = m_slaveInformationProviderModelsCompPtr[index];
		if (infoProviderModelPtr != NULL){
			infoProviderModelPtr->AttachObserver(&m_updateBridge);
		}
	}
}


void CComposedInformationProviderComp::OnComponentDestroyed()
{
	m_updateBridge.EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}




} // namespace iinsp


