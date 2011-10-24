#ifndef isig_CPulseControllerParamsComp_included
#define isig_CPulseControllerParamsComp_included


// ACF includes
#include "imath/CVarVector.h"
#include "icomp/CComponentBase.h"

// QSF includes
#include "isig/IPulseParams.h"
#include "isig/IPulseConstraints.h"


namespace isig
{


/**
	Simple component implementation of IPulseParams interface.
*/
class CPulseParamsComp:
			public icomp::CComponentBase,
			virtual public isig::IPulseParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CPulseParamsComp);
		I_REGISTER_INTERFACE(isig::IPulseParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_pulseConstraintsCompPtr, "PulseConstraints", "Description of strobe parameter limits", false, "PulseConstraints");
		I_ASSIGN(m_delayTimeAttrPtr, "DelayTime", "Delay time from trigger to pulse, it is in seconds", true, 0);
		I_ASSIGN(m_pulseDurationAttrPtr, "PulseDuration", "Duration of signal puls in seconds", true, 0);
		I_ASSIGN(m_relaxingTimeAttrPtr, "RelaxingTime", "Relaxing time before new trigger is accepted after pulse", true, 0);
	I_END_COMPONENT;

	CPulseParamsComp();
	
	// reimplemented (isig::IPulseParams)
	virtual const IPulseConstraints* GetPulseConstraints() const;
	virtual double GetDelayTime() const;
	virtual void SetDelayTime(double time);
	virtual double GetPulseDuration() const;
	virtual void SetPulseDuration(double time);
	virtual double GetRelaxingTime() const;
	virtual void SetRelaxingTime(double time);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(IPulseConstraints, m_pulseConstraintsCompPtr);
	I_ATTR(double, m_delayTimeAttrPtr);
	I_ATTR(double, m_pulseDurationAttrPtr);
	I_ATTR(double, m_relaxingTimeAttrPtr);

	double m_delayTime;
	double m_pulseDuration;
	double m_relaxingTime;
};


} // namespace isig


#endif // !isig_CPulseControllerParamsComp_included



