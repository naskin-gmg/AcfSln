#ifndef iweb_CWebServerStatus_included
#define iweb_CWebServerStatus_included


// ACF-Solutions includes
#include <iweb/IWebServerStatus.h>


namespace iweb
{


class CWebServerStatus: virtual public IWebServerStatus
{
public:
	CWebServerStatus();

	// reimplemented (iweb::IWebServerStatus)
	virtual ServerStatus GetServerStatus() const override;
	virtual void SetServerStatus(ServerStatus status) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;
	virtual IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

private:
	ServerStatus m_serverStatus;
};


} // namespace iweb


#endif // !iweb_CWebServerStatus_included


