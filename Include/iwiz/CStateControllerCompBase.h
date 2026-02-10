// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iwiz_CStateControllerCompBase_included
#define iwiz_CStateControllerCompBase_included


// ACF includes
#include <icomp/CComponentBase.h>

#include <iproc/IStateController.h>


namespace iwiz
{


class CStateControllerCompBase:
			public icomp::CComponentBase,
			virtual public iproc::IStateController
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CStateControllerCompBase);
		I_REGISTER_INTERFACE(iproc::IStateController);
		I_REGISTER_INTERFACE(istd::IChangeable);
	I_END_COMPONENT;

	CStateControllerCompBase();

	// reimplemented (iproc::IStateController)
	virtual bool IsStateEnabled() const;
	virtual bool IsStateActive() const;
	virtual bool IsEnterAllowed(bool isActionAllowed = true, const IStateController* prevStatePtr = NULL) const;
	virtual bool IsLeaveAllowed(bool isActionAllowed = true, const IStateController* nextStatePtr = NULL) const;
	virtual bool TryEnterState(bool isActionAllowed = true, const IStateController* prevStatePtr = NULL);
	virtual bool TryLeaveState(bool isActionAllowed = true, const IStateController* nextStatePtr = NULL);
	virtual bool IsResetAllowed() const;

protected:
	mutable bool m_isStateActive;
	mutable bool m_isStateEnabled;
	mutable bool m_isEnterAllowed;
	mutable bool m_isLeaveAllowed;
};


} // namespace iwiz


#endif // !iwiz_CStateControllerCompBase_included


