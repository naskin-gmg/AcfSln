#ifndef iwin_CTimer_included
#define iwin_CTimer_included


#include "istd/istd.h"

#include "isys/ITimer.h"


namespace iwin
{


/**
	An timer implementation for the Windows plattform

	\ingroup Windows
*/
class CTimer: virtual public isys::ITimer
{
public:
	CTimer();

	bool IsVaild() const;

	double GetTimeTo(const CTimer& timer) const;

	I_QWORD GetNativeRepresentation() const;
	void SetNativeRepresentation(I_QWORD value);

	// reimplemented (isys::ITimer)
	virtual void Start();
	virtual double GetElapsed() const;
	virtual double GetTimeTo(const ITimer& timer) const;
	virtual void WaitTo(double time) const;
	virtual double GetTimerResolution() const;

private:
	long long m_startCounter;

	// static attributes
	static I_QWORD s_timerFrequence;
	static bool s_isTimerFrequenceValid;
};


// inline methods

inline I_QWORD CTimer::GetNativeRepresentation() const
{
	return m_startCounter;
}


inline void CTimer::SetNativeRepresentation(I_QWORD value)
{
	m_startCounter = value;
}


} // namespace iwin


#endif // !iwin_CTimer_included
