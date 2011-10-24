#ifndef isig_CTriggerParamsComp_included
#define isig_CTriggerParamsComp_included


#include "icomp/CComponentBase.h"

#include "isig/ITriggerConstraints.h"
#include "isig/ITriggerParams.h"


namespace isig
{


/**
	Implementatation of standard trigger parameters set.
*/
class CTriggerParamsComp: public icomp::CComponentBase, virtual public isig::ITriggerParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	CTriggerParamsComp();

	I_BEGIN_COMPONENT(CTriggerParamsComp);
		I_REGISTER_INTERFACE(isig::ITriggerParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_triggerConstraintsCompPtr, "TriggerConstraints", "Describing allowed parameter state, typically implemented in camera component", false, "TriggerConstraints");
		I_ASSIGN(m_triggerModeAttrPtr, "TriggerMode", "Set trigger mode:\n\t0 - None\n\t1 - Continuous\n\t2 - Rising edge\n\t3 - Falling edge\n\t4 - Positive level\n\t5 - Negative level\n\t6 - Software", true, 0);
	I_END_COMPONENT;

	// reimplmented (isig::ITriggerParams)
	virtual const ITriggerConstraints* GetTriggerConstraints() const;
	virtual int GetTriggerMode() const;
	virtual void SetTriggerMode(int triggerMode);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	int m_triggerMode;

	I_REF(isig::ITriggerConstraints, m_triggerConstraintsCompPtr);
	I_ATTR(int, m_triggerModeAttrPtr);
};


} // namespace isig


#endif // !isig_CTriggerParamsComp_included


