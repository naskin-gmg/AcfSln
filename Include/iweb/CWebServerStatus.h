#ifndef iweb_CWebServerStatus_included
#define iweb_CWebServerStatus_included


// ACF includes
#include <icomp/CComponentBase.h>

// ACF-Solutions includes
#include <iweb/IWebServerStatus.h>


namespace iweb
{


class CWebServerStatus:
			public icomp::CComponentBase,
			virtual public IWebServerStatus
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CWebServerStatus);
		I_REGISTER_INTERFACE(IWebServerStatus);
	I_END_COMPONENT;

	CWebServerStatus();

	// reimplemented (iweb::IWebServerStatus)
	virtual ServerStatus GetServerStatus() const;
	virtual void SetServerStatus(ServerStatus status);

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual bool IsEqual(const IChangeable& object) const;
	virtual IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	ServerStatus m_serverStatus;
};


} // namespace iweb


#endif // !iweb_CWebServerStatus_included


