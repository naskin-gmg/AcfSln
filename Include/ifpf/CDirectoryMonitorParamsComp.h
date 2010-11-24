#ifndef ifpf_CDirectoryMonitorParamsComp_included
#define ifpf_CDirectoryMonitorParamsComp_included


// ACF includes
#include "icomp/CComponentBase.h"


// AcfSln includes
#include "ifpf/CDirectoryMonitorParams.h"


namespace ifpf
{


/**
	Implementation of directory parameters component.
	This implementation makes possible the definition of the default parameters at the component level.
*/
class CDirectoryMonitorParamsComp: public icomp::CComponentBase, public ifpf::CDirectoryMonitorParams
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef ifpf::CDirectoryMonitorParams BaseClass2;

	I_BEGIN_COMPONENT(ifpf::CDirectoryMonitorParamsComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ifpf::IDirectoryMonitorParams);
		I_ASSIGN(m_poolingIntervallAttrPtr, "PoolingIntervall", "Intervall for checking of directory changes in seconds", false, 5);
		I_ASSIGN(m_observedItemTypesAttrPtr, "ObservedItems", "Items to be observed.\n1 - Directories\n2 - Files\n4 - Drives", false, OI_FILES);
		I_ASSIGN(m_observedChangesAttrPtr, "ObservedChanges", "Item changes to be observed.\n1 - Add\n2 - Remove\n4 - Modified\n8 - Attributes changed", false, OC_ADD);
		I_ASSIGN_MULTI_0(m_acceptPatternsAttrPtr, "AcceptPatterns", "File name patterns for accepting of the file items", false);
		I_ASSIGN_MULTI_0(m_ignorePatternsAttrPtr, "IgnorePatterns", "File name patterns for ignoring of the file items", false);
	I_END_COMPONENT();

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(double, m_poolingIntervallAttrPtr);
	I_ATTR(int, m_observedItemTypesAttrPtr);
	I_ATTR(int, m_observedChangesAttrPtr);
	I_MULTIATTR(istd::CString, m_acceptPatternsAttrPtr);
	I_MULTIATTR(istd::CString, m_ignorePatternsAttrPtr);
};


} // namespace ifpf


#endif // !ifpf_CDirectoryMonitorParamsComp_included


